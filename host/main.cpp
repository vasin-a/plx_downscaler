#include "args-parser.hpp"

#include <downscaler/Pixmap.hpp>
#include <downscaler/PixmapConverter.hpp>
#include <downscaler/Transform.hpp>

#include <future>
#include <iostream>
#include <string>
#include <sstream>
#include <variant>
#include <vector>

template<typename... Args>
static void log(Args&& ...args)
{
	std::stringstream stream;

	stream << "[" << std::chrono::system_clock::now().time_since_epoch().count() << "] ";

	(stream << ... << std::forward<Args>(args));

	std::cout << stream.str();
}

static std::variant<downscaler::Pixmap1ub, downscaler::Pixmap3ub, downscaler::Pixmap4ub> LoadImage(const std::filesystem::path& srcPath)
{
	int width, height, channels;
	const auto data = stbi_load(srcPath.string().c_str(), &width, &height, &channels, 0);

	switch (channels)
	{
	case 1:
	{
		auto pixmap = downscaler::Pixmap1ub(
			static_cast<unsigned>(width),
			static_cast<unsigned>(height),
			reinterpret_cast<const glm::u8vec1*>(data));

		stbi_image_free(data);
		return pixmap;
	}
	case 3:
	{
		auto pixmap = downscaler::Pixmap3ub(
			static_cast<unsigned>(width),
			static_cast<unsigned>(height),
			reinterpret_cast<const glm::u8vec3*>(data));

		stbi_image_free(data);
		return pixmap;
	}
	case 4:
	{
		auto pixmap = downscaler::Pixmap4ub(
			static_cast<unsigned>(width),
			static_cast<unsigned>(height),
			reinterpret_cast<const glm::u8vec4*>(data));

		stbi_image_free(data);
		return pixmap;
	}
	default:
		throw std::runtime_error("channels number is not supported yet");
	}
}

void ProcessImage(const std::map<std::string, std::any>& config, const std::filesystem::path& srcPath)
{
	using namespace downscaler;

	const auto gamma = std::any_cast<float>(config.at("gamma"));

	log("Loading image ", srcPath.string(), '\n');
	auto linearImg = std::visit([&](auto img) {
		log("Linearizing image ", srcPath.filename().string(), '\n');
		return ToLinearImage(img, gamma);
	}, LoadImage(srcPath));

	log("Transforming image ", srcPath.filename().string(), '\n');
	linearImg = ScaleTransform(
		linearImg,
		std::any_cast<ScalingAlgorithm>(config.at("method")),
		glm::vec2(std::any_cast<float>(config.at("scale"))),
		std::any_cast<float>(config.at("lod-bias"))
	);

	log("Correcting gamma for ", srcPath.filename().string(), '\n');
	auto gammaImg = ToGammaImage<Pixmap4ub>(linearImg, 1.0f / gamma);

	auto dstPath = std::any_cast<const std::filesystem::path&>(config.at("dst"));
	std::filesystem::create_directory(dstPath);

	dstPath = dstPath / srcPath.filename().replace_extension("png");

	log("Saving image ", srcPath.filename().string(), '\n');
	const auto width = static_cast<int>(gammaImg.width());
	const auto height = static_cast<int>(gammaImg.height());
	stbi_write_png(dstPath.string().c_str(), width, height, 4, gammaImg.data(), width * 4);
}


int main(int argc, char** argv)
{
	try
	{
		const auto config = GetConfig(argc, argv);
		const auto& srcDir = std::any_cast<const std::filesystem::path&>(config.at("src"));

		auto tasks = std::vector<std::unique_ptr<std::future<void>>>();
		for (const auto& dirEntry : std::filesystem::directory_iterator{ srcDir })
		{
			tasks.emplace_back(std::make_unique<std::future<void>>(std::async(ProcessImage, config, dirEntry.path())));
		}

		while (!tasks.empty())
		{
			log("Waiting for ", tasks.size(), " tasks...\n");

			const auto desiredUpdatePeriod = std::chrono::nanoseconds(1'000'000'000ull);
			const auto sampleStep = desiredUpdatePeriod / tasks.size();

			tasks.erase(std::remove_if(tasks.begin(), tasks.end(), [&](auto& t)
			{
				try
				{
					const auto status = t->wait_for(sampleStep);
					if (status == std::future_status::timeout)
					{
						return false;
					}

					t->get();
				}
				catch (const std::exception& ex)
				{
					std::cerr << ex.what() << '\n';
				}
				return true;
			}), tasks.end());
		}

		log("Done\n");
	}
	catch (const std::exception& ex)
	{
		std::cerr << ex.what() << '\n';
	}
}

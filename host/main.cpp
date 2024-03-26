#include "args-parser.hpp"

#include <downscaler/Pixmap.hpp>
#include <downscaler/PixmapConverter.hpp>
#include <downscaler/Transform.hpp>

#include <iostream>
#include <future>
#include <variant>
#include <vector>

std::variant<downscaler::Pixmap3ub, downscaler::Pixmap4ub> LoadImage(const std::filesystem::path& srcPath)
{
	int width, height, channels;
	const auto data = stbi_load(srcPath.string().c_str(), &width, &height, &channels, 0);

	switch (channels)
	{
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

	auto linearImg = std::visit([](auto img) {
		return ToLinearImage(img);
	}, LoadImage(srcPath));

	linearImg = ScaleTransform(linearImg, std::any_cast<ScalingAlgorithm>(config.at("method")), glm::vec2(std::any_cast<float>(config.at("scale"))));

	auto gammaImg = ToGammaImage<Pixmap4ub>(linearImg);

	auto dstPath = std::any_cast<const std::filesystem::path&>(config.at("dst"));
	std::filesystem::create_directory(dstPath);

	dstPath = dstPath / srcPath.filename().replace_extension("png");

	const auto width = static_cast<int>(gammaImg.width());
	const auto height = static_cast<int>(gammaImg.height());
	stbi_write_png(dstPath.string().c_str(), width, height, 4, gammaImg.data(), width * 4);
}


int main(int argc, char** argv)
{
	const auto config = GetConfig(argc, argv);
	const auto& srcDir = std::any_cast<const std::filesystem::path&>(config.at("src"));

	auto tasks = std::vector<std::future<void>>();
	for (const auto& dirEntry : std::filesystem::directory_iterator{ srcDir })
	{
		tasks.emplace_back(std::async(ProcessImage, config, dirEntry.path()));
	}

	for (auto& t : tasks)
	{
		try {
			t.get();
		}
		catch (const std::exception& ex)
		{
			std::cerr << ex.what() << '\n';
		}
	}
}

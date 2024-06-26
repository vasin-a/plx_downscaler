#include "args-parser.hpp"

#include <downscaler/AlphaPremultiply.hpp>
#include <downscaler/ScalingAlgorithm.hpp>

#include <filesystem>
#include <stdexcept>
#include <regex>

static std::filesystem::path MakeDir(const std::string& str)
{
	auto path = std::filesystem::path(str);
	if (std::filesystem::exists(path) && !std::filesystem::is_directory(path))
	{
		throw std::invalid_argument("not a directory");
	}
	return path;
}

static float ParseGamma(const std::string& str)
{
	return std::stof(str);
}

static float ParseLodBias(const std::string& str)
{
	return std::stof(str);
}

static downscaler::AlphaPremultiply ParsePremultiply(const std::string& str)
{
	if (str == "i" || str == "ignore" || str == "leave")
	{
		return downscaler::AlphaPremultiply::Ignore;
	}

	if (str == "p" || str == "mult" || str == "premultiply")
	{
		return downscaler::AlphaPremultiply::Premultiply;
	}

	if (str == "u" || str == "div" || str == "unpremultiply")
	{
		return downscaler::AlphaPremultiply::Unpremultiply;
	}

	throw std::invalid_argument("invalid premultiply value");
}

static downscaler::ScalingAlgorithm ParseMethod(const std::string& str)
{
	return downscaler::ScalingAlgorithmFromString(str);
}

static float ParseScale(const std::string& str)
{
	const auto value = std::stof(str);

	if (!(0.0f < value && value <= 1.0f))
	{
		throw std::invalid_argument("scale must be in (0..1]");
	}

	return value;
}

template<typename TFunc>
static void Parse(const std::string& name, const std::map<std::string, std::string>& arguments, std::map<std::string, std::any>& config, TFunc&& f)
{
	try
	{
		auto it = arguments.find(name);
		if (it == arguments.cend())
			throw std::invalid_argument("missing expected argument");
		if (config.count(name))
			throw std::invalid_argument("argument redefinition");

		config.emplace(name, f(it->second));
	}
	catch (const std::invalid_argument& ex)
	{
		throw std::invalid_argument("\"" + name + "\"" + " argument is invalid: " + ex.what());
	}
}

template<typename TFunc>
static bool TryParse(const std::string& name, const std::map<std::string, std::string>& arguments, std::map<std::string, std::any>& config, TFunc&& f, const std::string& value)
{
	try
	{
		if (config.count(name))
			throw std::invalid_argument("argument redefinition");

		auto it = arguments.find(name);
		if (it != arguments.cend())
		{
			config.emplace(name, f(it->second));
			return true;
		}
		else
		{
			config.emplace(name, f(value));
			return false;
		}
	}
	catch (const std::invalid_argument& ex)
	{
		throw std::invalid_argument("\"" + name + "\"" + " argument is invalid: " + ex.what());
	}
}

static std::map<std::string, std::string> ParseArguments(int argc, char** argv)
{
	static const auto regex = std::regex("--(\\w[\\w-]*)=(.+)");
	auto result = std::map<std::string, std::string>();

	std::for_each(argv + 1, argv + argc, [&](const char* arg)
	{
		auto match = std::cmatch();
		if (!std::regex_match(arg, match, regex))
			return;

		result[match[1]] = match[2];
	});

	return result;
}

std::map<std::string, std::any> GetConfig(int argc, char** argv)
{
	auto stringArguments = ParseArguments(argc, argv);

	auto result = std::map<std::string, std::any>();

	TryParse("src", stringArguments, result, MakeDir, "./img");
	TryParse("dst", stringArguments, result, MakeDir, "./out");
	TryParse("gamma", stringArguments, result, ParseGamma, "2.2");
	TryParse("lod-bias", stringArguments, result, ParseLodBias, "0.0");
	TryParse("src-alpha", stringArguments, result, ParsePremultiply, "ignore");
	TryParse("dst-alpha", stringArguments, result, ParsePremultiply, "ignore");

	Parse("method", stringArguments, result, ParseMethod);
	Parse("scale", stringArguments, result, ParseScale);

	return result;
}


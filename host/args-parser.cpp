#include "args-parser.hpp"

#include <filesystem>
#include <stdexcept>
#include <regex>

static std::filesystem::path make_dir(const std::string& str)
{
	auto path = std::filesystem::path(str);
	if (std::filesystem::exists(path) && !std::filesystem::is_directory(path))
	{
		throw std::invalid_argument("not a directory");
	}
	return path;
}

//static std::filesystem::path make_path(const std::string& str)
//{
//	auto path = std::filesystem::path(str);
//	if (!std::filesystem::exists(path) || std::filesystem::is_directory(path))
//	{
//		throw std::invalid_argument("no such file");
//	}
//	return path;
//}

//static int stoi(const std::string& str) { return std::stoi(str); }

static float stof(const std::string& str) { return std::stof(str); }

static std::string scopy(const std::string& str) { return str; }

template<typename TFunc>
static void parse(const std::string& name, const std::map<std::string, std::string>& arguments, std::map<std::string, std::any>& config, TFunc&& f)
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
static bool tryParse(const std::string& name, const std::map<std::string, std::string>& arguments, std::map<std::string, std::any>& config, TFunc&& f, const std::string& value)
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

static std::map<std::string, std::string> parseArguments(int argc, char** argv)
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

std::map<std::string, std::any> getConfig(int argc, char** argv)
{
	auto stringArguments = parseArguments(argc, argv);

	auto result = std::map<std::string, std::any>();

	tryParse("src", stringArguments, result, make_dir, ".");
	tryParse("dst", stringArguments, result, make_dir, "out");
	parse("method", stringArguments, result, scopy);
	parse("scale", stringArguments, result, stof);

	return result;
}


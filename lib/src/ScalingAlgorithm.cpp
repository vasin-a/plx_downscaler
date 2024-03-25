#include <downscaler/ScalingAlgorithm.hpp>

#include <stdexcept>
#include <utility>

namespace downscaler
{

ScalingAlgorithm ScalingAlgorithmFromString(std::string_view str)
{
	static constexpr std::pair<std::string_view, ScalingAlgorithm> kMap[] =
	{
		{"gauss", GaussBlurSubsample},
		{"box", BoxBlurMipMap},
	};

	auto it = std::find_if(std::begin(kMap), std::end(kMap), [&](auto pair)
	{
		return pair.first == str;
	});

	if (it == std::end(kMap))
	{
		throw std::invalid_argument("cannot resolve ScalingAlgorithm argument");
	}

	return it->second;
}

}

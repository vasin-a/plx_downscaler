#pragma once

#include <string_view>
#include <downscaler/Pixmap.hpp>

namespace downscaler
{

enum class ScalingAlgorithm
{
	GaussBlurSubsample,
	BoxBlurMipMap
};

ScalingAlgorithm ScalingAlgorithmFromString(std::string_view str);

}

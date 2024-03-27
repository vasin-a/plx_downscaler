#pragma once

#include <string_view>
#include <downscaler/Pixmap.hpp>

namespace downscaler
{

enum class ScalingAlgorithm
{
	GaussianDownsample,
	BoxBlurMipMap,
	Lanczos
};

ScalingAlgorithm ScalingAlgorithmFromString(std::string_view str);

}

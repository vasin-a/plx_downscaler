#include "GaussianSubsample.hpp"

#include <downscaler/Transform.hpp>

namespace downscaler
{

Pixmap4f ScaleTransform(const Pixmap4f& src, ScalingAlgorithm algorithm, glm::vec2 scale)
{
	switch (algorithm)
	{
	case ScalingAlgorithm::GaussBlurSubsample:
		return GaussianSubsample(src, scale);
	case ScalingAlgorithm::BoxBlurMipMap:
		return src;
	default:
		std::terminate();
	}
}

}

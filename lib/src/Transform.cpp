#include <downscaler/Transform.hpp>

namespace downscaler
{

Pixmap4f GaussianDownsample(const Pixmap4f& in, glm::vec2 scale);

Pixmap4f ScaleTransform(const Pixmap4f& src, ScalingAlgorithm algorithm, glm::vec2 scale)
{
	switch (algorithm)
	{
	case ScalingAlgorithm::GaussianDownsample:
		return GaussianDownsample(src, scale);
	case ScalingAlgorithm::BoxBlurMipMap:
		return src;
	default:
		std::terminate();
	}
}

}

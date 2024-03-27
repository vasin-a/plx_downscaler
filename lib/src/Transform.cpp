#include <downscaler/Transform.hpp>

namespace downscaler
{

Pixmap4f Gaussian(const Pixmap4f& in, glm::vec2 scale);
Pixmap4f BoxBlurMipMap(const Pixmap4f& in, glm::vec2 scale);
Pixmap4f Lanczos(const Pixmap4f& in, glm::vec2 scale);

Pixmap4f ScaleTransform(const Pixmap4f& src, ScalingAlgorithm algorithm, glm::vec2 scale)
{
	switch (algorithm)
	{
	case ScalingAlgorithm::GaussianDownsample:
		return Gaussian(src, scale);
	case ScalingAlgorithm::BoxBlurMipMap:
		return BoxBlurMipMap(src, scale);
	case ScalingAlgorithm::Lanczos:
		return Lanczos(src, scale);
	default:
		std::terminate();
	}
}

}

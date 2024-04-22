#include <downscaler/Transform.hpp>

namespace downscaler
{

Pixmap4f Gaussian(Pixmap4f in, glm::vec2 scale);
Pixmap4f BoxBlurMipMap(Pixmap4f in, glm::vec2 scale, float bias);
Pixmap4f BoxBlurMipMapFloor(Pixmap4f in, glm::vec2 scale, float bias);
Pixmap4f Lanczos(Pixmap4f in, glm::vec2 scale);

Pixmap4f ScaleTransform(Pixmap4f src, ScalingAlgorithm algorithm, glm::vec2 scale, float bias)
{
	switch (algorithm)
	{
	case ScalingAlgorithm::GaussianDownsample:
		return Gaussian(std::move(src), scale);
	case ScalingAlgorithm::BoxBlurMipMap:
		return BoxBlurMipMap(std::move(src), scale, bias);
	case ScalingAlgorithm::BoxBlurMipMapFloor:
		return BoxBlurMipMapFloor(std::move(src), scale, bias);
	case ScalingAlgorithm::Lanczos:
		return Lanczos(std::move(src), scale);
	default:
		std::terminate();
	}
}

Pixmap4f PremultiplyAlpha(Pixmap4f img)
{
	for (auto& pixel : img)
	{
		const auto rgb = pixel.rgb() * pixel.a;

		pixel = glm::vec4(rgb, pixel.a);
	}

	return img;
}

Pixmap4f UnpremultiplyAlpha(Pixmap4f img)
{
	for (auto& pixel : img)
	{
		if (glm::round(pixel.a * 255.0f) == 0.0f)
		{
			pixel = glm::vec4(0.0f);
		}
		else
		{
			auto rgb = pixel.rgb() / pixel.a;

			if (const auto maxValue = glm::compMax(rgb); maxValue > 1.0f)
			{
				rgb /= maxValue;
			}

			pixel = glm::vec4(rgb, pixel.a);
		}
	}

	return img;
}
}

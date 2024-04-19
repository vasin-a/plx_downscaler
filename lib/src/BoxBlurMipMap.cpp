#include "Sampler.hpp"

namespace downscaler
{

template <typename Sampler>
static Pixmap4f BoxBlurMipMapImpl(const Pixmap4f& in, glm::vec2 scale, float bias)
{
	const auto newDim = glm::round(glm::vec2(in.dim()) * scale);

	const auto sampler = Sampler(in, scale, bias);

	auto result = Pixmap4f(glm::uvec2(newDim));
	result.ForEachPixelWrite([&](glm::ivec2 dstPos)
	{
		const auto normalizedTC = glm::vec2(dstPos) / newDim;
		return sampler.SampleClampToBorder(normalizedTC);
	});
	return result;
}

Pixmap4f BoxBlurMipMap(const Pixmap4f& in, glm::vec2 scale, float bias)
{
	return BoxBlurMipMapImpl<TrilinearMipMapSampler>(in, scale, bias);
}

Pixmap4f BoxBlurMipMapFloor(const Pixmap4f& in, glm::vec2 scale, float bias)
{
	return BoxBlurMipMapImpl<LinearMipMapFloorSampler>(in, scale, bias);
}

}

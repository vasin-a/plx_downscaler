#include "Sampler.hpp"

namespace downscaler
{

Pixmap4f BoxBlurMipMap(const Pixmap4f& in, glm::vec2 scale)
{
	const auto newDim = glm::round(glm::vec2(in.dim()) * scale);

	const auto sampler = TrilinearMipMapSampler(in, scale);

	auto result = Pixmap4f(glm::uvec2(newDim));
	result.ForEachPixelWrite([&](glm::ivec2 dstPos)
	{
		const auto normalizedTC = glm::vec2(dstPos) / newDim;
		return sampler.SampleClampToBorder(normalizedTC);
	});
	return result;
}

}
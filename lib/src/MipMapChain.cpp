#include "MipMapChain.hpp"

namespace downscaler
{

static Pixmap4f BoxBlurDownsample(const Pixmap4f& in)
{
	glm::uvec2 size{ (glm::max)(in.width(), 2u) / 2u, (glm::max)(in.height(), 2u) / 2u };

	Pixmap4f result(size);

	for (unsigned y = 0; y < in.height(); y = y + 2)
	{
		for (unsigned x = 0; x < in.width(); x = x + 2)
		{
			const glm::ivec2 sampleCoords[] = { glm::ivec2(x, y) , glm::ivec2(x + 1, y), glm::ivec2(x, y + 1), glm::ivec2(x + 1, y + 1) };

			glm::vec4 avg{ 0.0f };
			for (glm::ivec2 tc : sampleCoords)
			{
				tc = glm::min(tc, glm::ivec2(in.width() - 1, in.height() - 1));
				avg += in.get(tc);
			}
			avg *= 0.25f;

			result.get({ x / 2, y / 2 }) = avg;
		}
	}

	return result;
}

MipMapChain::MipMapChain(Pixmap4f img)
{
	const auto maxDim = glm::max(img.width(), img.height());
	const auto totalLevels = glm::log2(glm::ceilPowerOfTwo(maxDim + 1));

	_chain.reserve(totalLevels);
	_chain.emplace_back(std::move(img));
}

glm::uvec2 MipMapChain::Dim(std::size_t level) const
{
	return _chain.at(level).dim();
}

const Pixmap4f& MipMapChain::Original() const noexcept
{
	return _chain.front();
}

const Pixmap4f& MipMapChain::At(unsigned level) const
{
	return _chain.at(level);
}

void MipMapChain::GenerateMipMaps(float upToLod) const
{
	std::size_t lod = static_cast<std::size_t>(glm::ceil(upToLod));

	for (std::size_t i = _chain.size(), len = _chain.capacity(); i < len && i <= lod; ++i)
	{
		_chain.emplace_back(BoxBlurDownsample(_chain.back()));
	}
}

}

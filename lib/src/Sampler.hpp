#pragma once

#include "MipMapChain.hpp"

namespace downscaler
{

// GL_NEAREST
class NearestSampler
{
public:
	NearestSampler(const Pixmap4f& storage) : _storage(storage)
	{
	}

	glm::vec4 SampleClampToBorderNormalized(glm::vec2 uv) const noexcept
	{
		return SampleClampToBorder(uv * glm::vec2(_storage.dim()));
	}

	glm::vec4 SampleClampToBorder(glm::vec2 uv) const noexcept
	{
		const auto tc = glm::clamp(glm::ivec2(glm::floor(uv)), glm::ivec2(0), glm::ivec2(_storage.dim()) - glm::ivec2(1));

		return _storage.get(tc);
	};

private:
	const Pixmap4f& _storage;
};

// GL_LINEAR
class BilinearSampler
{
public:
	BilinearSampler(const Pixmap4f& storage) : _storage(storage)
	{
	}

	glm::vec4 SampleClampToBorderNormalized(glm::vec2 uv) const noexcept
	{
		return SampleClampToBorder(uv * glm::vec2(_storage.dim()));
	}

	glm::vec4 SampleClampToBorder(glm::vec2 uv) const noexcept
	{
		const auto size = glm::vec2(_storage.dim());

		const auto SW = glm::clamp(glm::floor(uv), glm::vec2(0.0f), size - 1.0f);
		const auto NE = glm::clamp(glm::floor(uv) + glm::vec2(1.0f), glm::vec2(0.0f), size - 1.0f);
		const auto NW = glm::vec2{ SW.x, NE.y };
		const auto SE = glm::vec2{ NE.x, SW.y };

		const auto valueSW = _storage.get(glm::ivec2(SW));
		const auto valueNE = _storage.get(glm::ivec2(NE));
		const auto valueNW = _storage.get(glm::ivec2(NW));
		const auto valueSE = _storage.get(glm::ivec2(SE));

		const auto fraction = glm::fract(uv);

		return glm::mix
		(
			glm::mix(valueSW, valueSE, fraction.x),
			glm::mix(valueNW, valueNE, fraction.x),
			fraction.y
		);

	};

private:
	const Pixmap4f& _storage;
};

// GL_LINEAR_MIPMAP_LINEAR
class TrilinearMipMapSampler
{
public:
	TrilinearMipMapSampler(Pixmap4f storage, glm::vec2 scale, float bias) :
		_mipMapChain(std::move(storage))
	{
		_lod = glm::log2(glm::compMax(1.0f / scale));
		_baseLevel = static_cast<std::size_t>(glm::max(glm::floor(_lod + bias), 0.0f));
		_nextLevel = static_cast<std::size_t>(glm::max(glm::ceil(_lod + bias), 0.0f));
		_mipMapChain.GenerateMipMaps(_nextLevel);
	}

	glm::vec4 SampleClampToBorder(glm::vec2 normalizedTC) const noexcept
	{
		normalizedTC = glm::clamp(normalizedTC, glm::vec2(0.0f), glm::vec2(1.0f));

		const auto samplerBase = BilinearSampler(_mipMapChain.At(_baseLevel));
		const auto samplerNext = BilinearSampler(_mipMapChain.At(_nextLevel));

		if (_baseLevel != _nextLevel)
		{
			const auto colorBase = samplerBase.SampleClampToBorderNormalized(normalizedTC);
			const auto colorNext = samplerNext.SampleClampToBorderNormalized(normalizedTC);
			return glm::mix(colorBase, colorNext, glm::fract(_lod));
		}
		else
		{
			return samplerBase.SampleClampToBorderNormalized(normalizedTC);
		}
	}

private:
	MipMapChain _mipMapChain;
	float _lod;
	std::size_t _baseLevel;
	std::size_t _nextLevel;
};

// GL_LINEAR_MIPMAP_NEAREST analogue, except floor is used to select the mip level
class LinearMipMapFloorSampler
{
public:
	LinearMipMapFloorSampler(Pixmap4f storage, glm::vec2 scale, float bias) :
		_mipMapChain(std::move(storage))
	{
		const auto lod = glm::log2(glm::compMax(1.0f / scale));
		_level = static_cast<std::size_t>(glm::max(glm::floor(lod + bias), 0.0f));

		_mipMapChain.GenerateMipMaps(_level);
	}

	glm::vec4 SampleClampToBorder(glm::vec2 normalizedTC) const noexcept
	{
		normalizedTC = glm::clamp(normalizedTC, glm::vec2(0.0f), glm::vec2(1.0f));

		const auto sampler = BilinearSampler(_mipMapChain.At(_level));

		return sampler.SampleClampToBorderNormalized(normalizedTC);
	}

private:
	MipMapChain _mipMapChain;
	std::size_t _level;
};

}

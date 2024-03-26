#pragma once

#include <downscaler/Pixmap.hpp>

#include <downscaler/glm.hpp>

namespace downscaler
{

class NearestSampler
{
public:
	NearestSampler(const Pixmap4f& storage) : _storage(storage)
	{
	}

	glm::vec4 SampleClampToBorder(glm::vec2 uv) const noexcept
	{
		const auto tc = glm::clamp(glm::ivec2(glm::floor(uv)), glm::ivec2(0), glm::ivec2(_storage.dim()) - glm::ivec2(1));

		return _storage.get(tc);
	};

private:
	Pixmap4f _storage;
};

class BilinearSampler
{
public:
	BilinearSampler(const Pixmap4f& storage) : _storage(storage)
	{
	}

	glm::vec4 SampleClampToBorder(glm::vec2 uv) const noexcept
	{
		const auto size = glm::vec2(_storage.width(), _storage.height());

		const auto SW = glm::clamp(glm::floor(uv - glm::vec2(0.5f)), glm::vec2(0.0f), size - 1.0f);
		const auto NE = glm::clamp(glm::floor(uv - glm::vec2(0.5f)) + glm::vec2(1.0f), glm::vec2(0.0f), size - 1.0f);
		const auto NW = glm::vec2{ SW.x, NE.y };
		const auto SE = glm::vec2{ NE.x, SW.y };

		const auto valueSW = _storage.get(glm::ivec2(SW));
		const auto valueNE = _storage.get(glm::ivec2(NE));
		const auto valueNW = _storage.get(glm::ivec2(NW));
		const auto valueSE = _storage.get(glm::ivec2(SE));

		const auto fraction = glm::fract(uv - glm::vec2(0.5f));

		return glm::mix
		(
			glm::mix(valueSW, valueSE, fraction.x),
			glm::mix(valueNW, valueNE, fraction.x),
			fraction.y
		);

	};

private:
	Pixmap4f _storage;
};

}

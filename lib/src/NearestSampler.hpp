#pragma once

#include <downscaler/Pixmap.hpp>

namespace downscaler
{

class NearestSampler
{
public:
	NearestSampler(const Pixmap4f& storage) : _storage(storage)
	{
	}

	glm::vec4 sampleClampToBorder(glm::ivec2 tc)
	{
		tc = glm::clamp(tc, glm::ivec2(0), glm::ivec2(_storage.dim()) - glm::ivec2(1));

		return _storage[tc.y * _storage.width() + tc.x];
	};

private:
	Pixmap4f _storage;
};

}

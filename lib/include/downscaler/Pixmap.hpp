#pragma once

#include <downscaler/glm.hpp>

#include <functional>
#include <vector>

#include <cstdint>

namespace downscaler
{

template <typename T>
class BasicPixmap
{
public:
	using pixel_type = T;
	using component_type = typename T::value_type;

	BasicPixmap(std::size_t width, std::size_t height, const T* srcData) :
		_width(width),
		_height(height),
		_storage(srcData, srcData + width * height)
	{
		
	}

	BasicPixmap(glm::uvec2 dim) :
		_width(dim.x),
		_height(dim.y),
		_storage(_width * _height)
	{

	}

	glm::uvec2 dim() const noexcept
	{
		return { _width, _height };
	}

	template<typename Func> void ForEachPixelRead(const Func& callback) const
	{
		for (std::size_t i = 0; i != _height; ++i)
		{
			for (std::size_t j = 0; j != _width; ++j)
			{
				callback(glm::ivec2(j, i), _storage[i * _width + j]);
			}
		}
	}

	template<typename Func> void ForEachPixelWrite(const Func& callback)
	{
		for (std::size_t i = 0; i != _height; ++i)
		{
			for (std::size_t j = 0; j != _width; ++j)
			{
				_storage[i * _width + j] = callback(glm::ivec2(j, i));
			}
		}
	}

	BasicPixmap<T> Transpose() const
	{
		auto result = BasicPixmap<T>(glm::uvec2(_height, _width));

		ForEachPixelRead([&](glm::ivec2 pos, T pixel)
		{
			result[pos.x * _height + pos.y] = T;
		});

		return result;
	}

	auto begin() noexcept { return _storage.begin(); }
	auto begin() const noexcept { return _storage.begin(); }
	auto cbegin() const noexcept { return _storage.cbegin(); }

	auto end() noexcept { return _storage.end(); }
	auto end() const noexcept { return _storage.end(); }
	auto cend() const noexcept { return _storage.cend(); }

	auto width() const noexcept { return _width; }
	auto height() const noexcept { return _height; }

	auto data() noexcept { return _storage.data(); }
	auto data() const noexcept { return _storage.data(); }

	auto& operator[] (std::size_t pos) noexcept { return _storage[pos]; }
	const auto& operator[] (std::size_t pos) const noexcept { return _storage[pos]; }
	
	auto& get(glm::ivec2 tc) noexcept { return _storage[_width * tc.y + tc.x]; }
	const auto& get(glm::ivec2 tc) const noexcept { return _storage[_width * tc.y + tc.x]; }

private:
	std::size_t _width;
	std::size_t _height;
	std::vector<T> _storage;
};

using Pixmap4f = BasicPixmap<glm::vec4>;
using Pixmap3f = BasicPixmap<glm::vec3>;
using Pixmap2f = BasicPixmap<glm::vec2>;
using Pixmap1f = BasicPixmap<glm::vec1>;

using Pixmap4ub = BasicPixmap<glm::u8vec4>;
using Pixmap3ub = BasicPixmap<glm::u8vec3>;
using Pixmap2ub = BasicPixmap<glm::u8vec2>;
using Pixmap1ub = BasicPixmap<glm::u8vec1>;

}

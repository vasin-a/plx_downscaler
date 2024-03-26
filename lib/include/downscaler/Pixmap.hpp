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

	template<typename Func> void ForEachPixel(Func&& calback) const;
	BasicPixmap<T> Transpose() const;

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

private:
	std::size_t _width;
	std::size_t _height;
	std::vector<T> _storage;
};

template <typename T>
template<typename Func> void BasicPixmap<T>::ForEachPixel(Func&& callback) const
{
	for (std::size_t i = 0; i != _height; ++i)
	{
		for (std::size_t j = 0; j != _width; ++j)
		{
			std::invoke(std::forward<Func>(callback), glm::ivec2(j, i));
		}
	}
}

template <typename T>
BasicPixmap<T> BasicPixmap<T>::Transpose() const
{
	auto result = BasicPixmap<T>(glm::uvec2(_height, _width));

	ForEachPixel([&](glm::ivec2 pos)
	{
		result[pos.x * _height + pos.y] = _storage[pos.y * _width + pos.x];
	});

	return result;
}

using Pixmap4f = BasicPixmap<glm::vec4>;
using Pixmap3f = BasicPixmap<glm::vec3>;
using Pixmap2f = BasicPixmap<glm::vec2>;
using Pixmap1f = BasicPixmap<glm::vec1>;

using Pixmap4ub = BasicPixmap<glm::u8vec4>;
using Pixmap3ub = BasicPixmap<glm::u8vec3>;
using Pixmap2ub = BasicPixmap<glm::u8vec2>;
using Pixmap1ub = BasicPixmap<glm::u8vec1>;

}

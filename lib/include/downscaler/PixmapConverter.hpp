#pragma once

#include <downscaler/Pixmap.hpp>

#include <algorithm>

namespace downscaler
{

namespace detail
{
template<typename>
struct from_vec4_norm;

template<typename T, glm::qualifier Q>
struct from_vec4_norm<glm::vec<1, T, Q>>
{
	using result_type = glm::vec<1, T, Q>;
	constexpr static result_type invoke(glm::vec4 orig)
	{
		if constexpr (!std::is_same_v<float, T>)
		{
			orig = glm::min(orig, glm::vec4{ 1.0f });
			if constexpr (std::is_unsigned_v<T>)
			{
				orig = glm::max(orig, glm::vec4{ 0.0f });
			}

			const auto x = static_cast<T>(glm::round(orig.x * static_cast<float>(std::numeric_limits<T>::max())));
			return result_type{ x };
		}
		else
		{
			return result_type{ orig.x };
		}
	}
};

template<typename T, glm::qualifier Q>
struct from_vec4_norm<glm::vec<2, T, Q>>
{
	using result_type = glm::vec<2, T, Q>;
	constexpr static result_type invoke(glm::vec4 orig)
	{
		if constexpr (!std::is_same_v<float, T>)
		{
			orig = glm::min(orig, glm::vec4{ 1.0f });
			if constexpr (std::is_unsigned_v<T>)
			{
				orig = glm::max(orig, glm::vec4{ 0.0f });
			}

			const auto x = static_cast<T>(glm::round(orig.x * static_cast<float>(std::numeric_limits<T>::max())));
			const auto y = static_cast<T>(glm::round(orig.y * static_cast<float>(std::numeric_limits<T>::max())));
			return result_type{ x, y };
		}
		else
		{
			return result_type{ orig.x, orig.y };
		}
	}
};

template<typename T, glm::qualifier Q>
struct from_vec4_norm<glm::vec<3, T, Q>>
{
	using result_type = glm::vec<3, T, Q>;
	constexpr static result_type invoke(glm::vec4 orig)
	{
		if constexpr (!std::is_same_v<float, T>)
		{
			orig = glm::min(orig, glm::vec4{ 1.0f });
			if constexpr (std::is_unsigned_v<T>)
			{
				orig = glm::max(orig, glm::vec4{ 0.0f });
			}

			const auto x = static_cast<T>(glm::round(orig.x * static_cast<float>(std::numeric_limits<T>::max())));
			const auto y = static_cast<T>(glm::round(orig.y * static_cast<float>(std::numeric_limits<T>::max())));
			const auto z = static_cast<T>(glm::round(orig.z * static_cast<float>(std::numeric_limits<T>::max())));
			return result_type{ x, y, z };
		}
		else
		{
			return result_type{ orig.x, orig.y, orig.z };
		}
	}
};

template<typename T, glm::qualifier Q>
struct from_vec4_norm<glm::vec<4, T, Q>>
{
	using result_type = glm::vec<4, T, Q>;
	constexpr static result_type invoke(glm::vec4 orig)
	{
		if constexpr (!std::is_same_v<float, T>)
		{
			orig = glm::min(orig, glm::vec4{ 1.0f });
			if constexpr (std::is_unsigned_v<T>)
			{
				orig = glm::max(orig, glm::vec4{ 0.0f });
			}

			const auto x = static_cast<T>(glm::round(orig.x * static_cast<float>(std::numeric_limits<T>::max())));
			const auto y = static_cast<T>(glm::round(orig.y * static_cast<float>(std::numeric_limits<T>::max())));
			const auto z = static_cast<T>(glm::round(orig.z * static_cast<float>(std::numeric_limits<T>::max())));
			const auto w = static_cast<T>(glm::round(orig.w * static_cast<float>(std::numeric_limits<T>::max())));
			return result_type{ x, y, z, w };
		}
		else
		{
			return result_type{ orig.x, orig.y, orig.z, orig.w };
		}
	}
};

}

template<typename T, glm::qualifier Q>
constexpr glm::vec4 ToVec4Norm(const glm::vec<1, T, Q>& orig)
{
	auto x = static_cast<float>(orig.x);
	if constexpr (!std::is_same_v<float, T>)
	{
		x = x / static_cast<float>(std::numeric_limits<T>::max());
	}

	return { x, 0.0f, 0.0f, 1.0f };
}

template<typename T, glm::qualifier Q>
constexpr glm::vec4 ToVec4Norm(const glm::vec<2, T, Q>& orig)
{
	auto x = static_cast<float>(orig.x);
	auto y = static_cast<float>(orig.y);
	if constexpr (!std::is_same_v<float, T>)
	{
		x = x / static_cast<float>(std::numeric_limits<T>::max());
		y = y / static_cast<float>(std::numeric_limits<T>::max());
	}

	return { x, y, 0.0f, 1.0f };
}

template<typename T, glm::qualifier Q>
constexpr glm::vec4 ToVec4Norm(const glm::vec<3, T, Q>& orig)
{
	auto x = static_cast<float>(orig.x);
	auto y = static_cast<float>(orig.y);
	auto z = static_cast<float>(orig.z);
	if constexpr (!std::is_same_v<float, T>)
	{
		x = x / static_cast<float>(std::numeric_limits<T>::max());
		y = y / static_cast<float>(std::numeric_limits<T>::max());
		z = z / static_cast<float>(std::numeric_limits<T>::max());
	}

	return { x, y, z, 1.0f };
}

template<typename T, glm::qualifier Q>
constexpr glm::vec4 ToVec4Norm(const glm::vec<4, T, Q>& orig)
{
	auto x = static_cast<float>(orig.x);
	auto y = static_cast<float>(orig.y);
	auto z = static_cast<float>(orig.z);
	auto w = static_cast<float>(orig.w);
	if constexpr (!std::is_same_v<float, T>)
	{
		x = x / static_cast<float>(std::numeric_limits<T>::max());
		y = y / static_cast<float>(std::numeric_limits<T>::max());
		z = z / static_cast<float>(std::numeric_limits<T>::max());
		w = w / static_cast<float>(std::numeric_limits<T>::max());
	}

	return { x, y, z, w };
}

template<typename T>
constexpr T FromVec4Norm(const glm::vec4& orig)
{
	return detail::from_vec4_norm<T>::invoke(orig);
}

template <typename T>
Pixmap4f ToLinearImage(const BasicPixmap<T>& pixmap)
{
	auto result = Pixmap4f(pixmap.dim());

	std::transform(pixmap.cbegin(), pixmap.cend(), result.begin(), [](T pixel)
	{
		return glm::convertSRGBToLinear(ToVec4Norm(pixel));
	});

	return result;
}

template <typename T>
T ToGammaImage(const Pixmap4f& pixmap)
{
	auto result = T(pixmap.dim());

	std::transform(pixmap.cbegin(), pixmap.cend(), result.begin(), [](glm::vec4 pixel)
	{
		return FromVec4Norm<T::pixel_type>(glm::convertLinearToSRGB(pixel));
	});

	return result;
}

}

#pragma once

#include "NearestSampler.hpp"

#include <vector>
#include <numeric>

namespace downscaler
{
namespace gaussian_blur
{

inline float kernel_radius(float sigma)
{
	const auto threeSigmas = glm::max(sigma, 0.0f) * 3.0f;
	return glm::floor(threeSigmas);
}

inline bool calculate_weights(std::vector<float>& weights, float sigma)
{
	const auto radius = kernel_radius(sigma);
	const auto samples = static_cast<std::size_t>(radius * 2.0f + 1.0f);

	if (weights.size() == samples)
	{
		return false;
	}

	weights.resize(0);
	weights.reserve(samples);

	for (auto x = -radius; x <= radius; x++)
	{
		weights.emplace_back(glm::exp(-0.5f * (x / sigma) * (x / sigma)));
	}

	const auto invSum = 1.0f / std::accumulate(weights.begin(), weights.end(), 0.0f);
	std::for_each(weights.begin(), weights.end(), [&](auto& v) { v *= invSum; });

	return true;
}

}

Pixmap4f GaussianSubsample(const Pixmap4f& in, glm::vec2 scale)
{
	auto blurredHorizontal = in;

	in.ForEachPixel([](glm::ivec2 pos)
	{
	});
}

}

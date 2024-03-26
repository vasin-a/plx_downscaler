#pragma once

#include "Sampler.hpp"

#include <vector>
#include <numeric>

namespace downscaler
{

static float KernelOffset(unsigned width)
{
	const auto sixSigma = static_cast<float>(width);
	const auto begin = -(sixSigma - 1.0f) * 0.5f;
	return begin;
}

static float Gaussian(float sigma, float x)
{
	return glm::exp(-0.5f * (x / sigma) * (x / sigma));
}

static std::vector<float> BuildKernel(unsigned width)
{
	const auto sixSigma = static_cast<float>(width);
	const auto sigma = sixSigma / 6.0f;
	const auto begin = KernelOffset(width);

	auto weights = std::vector<float>(width);
	std::generate(weights.begin(), weights.end(), [x = begin, sigma]() mutable
	{
		return Gaussian(sigma, x++);
	});


	const auto invSum = 1.0f / std::accumulate(weights.begin(), weights.end(), 0.0f);
	std::for_each(weights.begin(), weights.end(), [&](auto& v) { v *= invSum; });

	return weights;
}

Pixmap4f GaussianDownsample(const Pixmap4f& in, glm::vec2 scale)
{
	const auto newDim = glm::round(glm::vec2(in.dim()) * scale);
	const auto kernelSize = glm::uvec2(glm::round(1.0f / scale));
	const auto kernelOffset = glm::vec2(KernelOffset(kernelSize.x), KernelOffset(kernelSize.y));

	const auto horizontalKernel = BuildKernel(kernelSize.x);
	const auto verticalKernel = BuildKernel(kernelSize.y);

	const auto horizontalSampler = NearestSampler(in);
	auto blurredHorizontal = Pixmap4f(glm::uvec2(static_cast<unsigned>(newDim.x), in.dim().y));
	blurredHorizontal.ForEachPixelWrite([&](glm::ivec2 dstPos)
	{
		auto srcPos = glm::vec2(dstPos) / glm::vec2(scale.x, 1.0f) + glm::vec2(kernelOffset.x, 0.0f);
		auto sum = glm::vec4(0.0f);

		for (auto weight : horizontalKernel)
		{
			sum += horizontalSampler.SampleClampToBorder(srcPos) * weight;
			srcPos.x++;
		}
		return sum;
	});

	const auto verticalSampler = NearestSampler(blurredHorizontal);
	auto blurredVertical = Pixmap4f(glm::uvec2(newDim));
	blurredVertical.ForEachPixelWrite([&](glm::ivec2 dstPos)
	{
		auto srcPos = glm::vec2(dstPos) / scale + glm::vec2(0.0f, kernelOffset.y);
		auto sum = glm::vec4(0.0f);

		for (auto weight : horizontalKernel)
		{
			sum += horizontalSampler.SampleClampToBorder(srcPos) * weight;
			srcPos.y++;
		}
		return sum;
	});

	return blurredVertical;
}

}

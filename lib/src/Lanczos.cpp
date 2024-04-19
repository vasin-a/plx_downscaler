#pragma once

#include "Sampler.hpp"

#include <vector>
#include <numeric>

namespace downscaler
{

static constexpr unsigned kLobes = 2; // Kernel is within interval [-Lobes..Lobes]

static float sinc(float x)
{
	const auto sinArg = glm::pi<float>() * x;

	if (glm::abs(sinArg) < 1e-6f)
	{
		return 1.0f;
	}

	return glm::sin(sinArg) / sinArg;
}

static float Lanczos(float x)
{
	if (glm::abs(x) >= kLobes)
	{
		return 0.0;
	}

	return sinc(x) * sinc(x / kLobes);
}

static float KernelCenterOffset(unsigned width)
{
	const auto begin = (static_cast<float>(width) - 1.0f) * 0.5f;
	return begin;
}


static std::vector<float> BuildKernel(unsigned width)
{
	const auto kernelWindow = static_cast<float>(width) * 0.5f;
	const auto begin = -KernelCenterOffset(width);

	auto weights = std::vector<float>(width);
	std::generate(weights.begin(), weights.end(), [=, x = begin]() mutable
	{
		const auto arg = ((x++) / kernelWindow) * kLobes;
		return Lanczos(arg);
	});


	const auto invSum = 1.0f / std::accumulate(weights.begin(), weights.end(), 0.0f);
	std::for_each(weights.begin(), weights.end(), [&](auto& v) { v *= invSum; });

	return weights;
}

Pixmap4f Lanczos(Pixmap4f in, glm::vec2 scale)
{
	const auto newDim = glm::round(glm::vec2(in.dim()) * scale);
	const auto kernelSize = glm::uvec2(glm::round(2.0f / scale));
	const auto kernelCenterOffset = glm::vec2(KernelCenterOffset(kernelSize.x), KernelCenterOffset(kernelSize.y));

	const auto horizontalKernel = BuildKernel(kernelSize.x);
	const auto verticalKernel = BuildKernel(kernelSize.y);

	const auto horizontalSampler = BilinearSampler(in);
	auto blurredHorizontal = Pixmap4f(glm::uvec2(static_cast<unsigned>(newDim.x), in.dim().y));
	blurredHorizontal.ForEachPixelWrite([&](glm::ivec2 dstPos)
	{
		auto srcPos = glm::vec2(dstPos) / glm::vec2(scale.x, 1.0f) - glm::vec2(kernelCenterOffset.x, 0.0f);
		auto sum = glm::vec4(0.0f);

		for (auto weight : horizontalKernel)
		{
			sum += horizontalSampler.SampleClampToBorder(srcPos) * weight;
			srcPos.x++;
		}
		return sum;
	});

	const auto verticalSampler = BilinearSampler(blurredHorizontal);
	auto blurredVertical = Pixmap4f(glm::uvec2(newDim));
	blurredVertical.ForEachPixelWrite([&](glm::ivec2 dstPos)
	{
		auto srcPos = glm::vec2(dstPos) / glm::vec2(1.0f, scale.y) - glm::vec2(0.0f, kernelCenterOffset.y);
		auto sum = glm::vec4(0.0f);

		for (auto weight : verticalKernel)
		{
			sum += verticalSampler.SampleClampToBorder(srcPos) * weight;
			srcPos.y++;
		}
		return sum;
	});

	return blurredVertical;
}

}

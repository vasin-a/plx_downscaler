#pragma once

#include <downscaler/Pixmap.hpp>
#include <downscaler/ScalingAlgorithm.hpp>
#include <downscaler/glm.hpp>

namespace downscaler
{
Pixmap4f ScaleTransform(const Pixmap4f& src, ScalingAlgorithm algorithm, glm::vec2 scale, float bias);
}

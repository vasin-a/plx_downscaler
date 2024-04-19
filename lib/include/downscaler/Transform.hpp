#pragma once

#include <downscaler/Pixmap.hpp>
#include <downscaler/ScalingAlgorithm.hpp>
#include <downscaler/glm.hpp>

namespace downscaler
{
Pixmap4f ScaleTransform(Pixmap4f src, ScalingAlgorithm algorithm, glm::vec2 scale, float bias);
Pixmap4f PremultiplyAlpha(Pixmap4f img);
Pixmap4f UnpremultiplyAlpha(Pixmap4f img);
}

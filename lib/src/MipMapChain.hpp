#pragma once

#include <downscaler/Pixmap.hpp>
#include <vector>

namespace downscaler
{

class MipMapChain
{
public:
	MipMapChain(Pixmap4f);
	virtual glm::uvec2 Dim(std::size_t level = 0) const;

	const Pixmap4f& Original() const noexcept;
	const Pixmap4f& At(std::size_t level) const;

	void GenerateMipMaps(std::size_t upToLod);
private:
	mutable std::vector<Pixmap4f> _chain;
};

}

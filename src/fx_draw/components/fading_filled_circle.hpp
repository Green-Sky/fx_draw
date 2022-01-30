#pragma once

#include <glm/vec2.hpp>
#include <glm/vec4.hpp>

namespace fx_draw::Components {

struct FadingFilledCircle {
	//glm::vec2 center;
	float radius = 1.f;

	glm::vec4 color{1,1,1,1};
};

} // fx_draw::Components


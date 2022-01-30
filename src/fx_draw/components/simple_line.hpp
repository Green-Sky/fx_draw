#pragma once

#include <glm/vec2.hpp>
#include <glm/vec4.hpp>

namespace fx_draw::Components {

struct SimpleLine {
	glm::vec2 p1;
	glm::vec2 p2;

	glm::vec4 color{1,1,1,1};
};

} // fx_draw::Components


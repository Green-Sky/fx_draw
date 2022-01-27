#pragma once

#include <glm/vec2.hpp>
#include <glm/vec4.hpp>

namespace Components::fx_draw {

struct simple_line {
	glm::vec2 p1;
	glm::vec2 p2;

	glm::vec4 color{1,1,1,1};
};

} // Components::fx_draw


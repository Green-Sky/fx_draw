#pragma once

#include <glm/vec2.hpp>
#include <glm/vec4.hpp>

namespace Components::fx_draw {

// uses MM::Components::Transform
struct simple_rectangle {
	glm::vec2 upper_left;
	glm::vec2 lower_right;

	glm::vec4 color{1,1,1,1};
};

} // Components::fx_draw


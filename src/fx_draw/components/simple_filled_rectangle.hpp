#pragma once

#include <glm/vec2.hpp>
#include <glm/vec4.hpp>

namespace fx_draw::Components {

// uses MM::Components::Position2D
struct SimpleFilledRectangle {
	glm::vec2 upper_left;
	glm::vec2 lower_right;

	glm::vec4 color{1,1,1,1};
};

} // fx_draw::Components


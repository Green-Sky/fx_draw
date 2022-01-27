#pragma once

#include <glm/vec2.hpp>
#include <glm/vec4.hpp>

namespace Components::fx_draw {

struct simple_filled_circle {
	//glm::vec2 center;
	float radius = 1.f;

	glm::vec4 color{1,1,1,1};
};

} // Components::fx_draw


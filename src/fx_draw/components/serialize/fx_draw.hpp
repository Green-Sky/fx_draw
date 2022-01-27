#pragma once

#include "../fading_filled_circle.hpp"
#include "../simple_line.hpp"
#include "../simple_circle.hpp"
#include "../simple_rectangle.hpp"
#include "../simple_filled_circle.hpp"
#include "../simple_filled_rectangle.hpp"
//#include "../fx_timer.hpp"

#include <nlohmann/json.hpp>
#include <mm/components/serialize/json_glm.hpp>

namespace Components::fx_draw {

NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(fading_filled_circle, color, radius)
NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(simple_line, color, p1, p2)
NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(simple_circle, color, radius)
NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(simple_rectangle, color, upper_left, lower_right)
NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(simple_filled_circle, color, radius)
NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(simple_filled_rectangle, color, upper_left, lower_right)

} // Components::fx_draw


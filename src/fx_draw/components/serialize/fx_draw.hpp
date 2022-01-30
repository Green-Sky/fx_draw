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

namespace fx_draw::Components {

NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(FadingFilledCircle, color, radius)
NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(SimpleLine, color, p1, p2)
NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(SimpleCircle, color, radius)
NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(SimpleRectangle, color, upper_left, lower_right)
NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(SimpleFilledCircle, color, radius)
NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(SimpleFilledRectangle, color, upper_left, lower_right)

} // fx_draw::Components


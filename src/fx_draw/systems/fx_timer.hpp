#pragma once

#include "../components/fx_timer.hpp"

#include <mm/engine_fwd.hpp>

#include <mm/components/time_delta.hpp>

namespace Systems::fx_draw {

void FXTimer(MM::Scene& scene, entt::view<entt::get_t<Components::fx_draw::fx_timer>> view, const MM::Components::TimeDelta& td);

} // Systems::fx_draw


#pragma once

#include "../components/fx_timer.hpp"

#include <mm/engine_fwd.hpp>

#include <mm/components/time_delta.hpp>

namespace fx_draw::Systems {

void fx_timer(MM::Scene& scene, entt::view<entt::get_t<Components::FXTimer>> view, const MM::Components::TimeDelta& td);

} // fx_draw::Systems


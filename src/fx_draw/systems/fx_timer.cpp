#include "./fx_timer.hpp"

#include <mm/services/scene_service_interface.hpp>

#include <entt/entity/registry.hpp>

#include <tracy/Tracy.hpp>

namespace fx_draw::Systems {

void fx_timer(MM::Scene& scene, entt::view<entt::get_t<Components::FXTimer>> view, const MM::Components::TimeDelta& td) {
	ZoneScopedN("fx_draw::Systems::fx_timer");

	std::vector<entt::entity> to_delete{};
	view.each([delta = td.tickDelta, &scene, &to_delete](auto e, auto& t) {
			t.remaining -= delta;

			if (t.remaining <= 0) {
				to_delete.push_back(e);
			}
		}
	);

	for (const auto& e : to_delete) {
		scene.destroy(e);
	}
}

} // fx_draw::Systems


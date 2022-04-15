#include "./fx_renderer.hpp"

#include <mm/fs_const_archiver.hpp>
#include <mm/opengl/shader.hpp>
#include <mm/services/opengl_renderer.hpp>

#include <mm/services/scene_service_interface.hpp>

#include <entt/entity/registry.hpp>

#include <mm/components/position2d.hpp>

#include "../../components/simple_line.hpp"
#include "../../components/simple_circle.hpp"
#include "../../components/simple_filled_circle.hpp"
#include "../../components/simple_rectangle.hpp"
#include "../../components/simple_filled_rectangle.hpp"

#include "../../components/fading_filled_circle.hpp"

#include "../../components/fx_timer.hpp"

#include <tracy/Tracy.hpp>

namespace fx_draw::OpenGL::RenderTasks {

FXDrawRenderTask::FXDrawRenderTask(MM::Engine& engine) : _fx_draw{engine} {
}

void FXDrawRenderTask::render(MM::Services::OpenGLRenderer& rs, MM::Engine& engine) {
	ZoneScopedN("fx_draw::FXDrawRenderTask::render");
	auto* ssi = engine.tryService<MM::Services::SceneServiceInterface>();
	if (ssi == nullptr) {
		return; // nothing to draw
	}

	auto& scene = ssi->getScene();

	if (!scene.ctx().contains<MM::OpenGL::Camera3D>()) {
		return; // nothing to draw
	}

	_fx_draw.setCamera(scene.ctx().at<MM::OpenGL::Camera3D>());

	rs.targets[target_fbo]->bind(MM::OpenGL::FrameBufferObject::W);

	glDisable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);

	// tri stuff
	{
		scene.view<MM::Components::Position2D, Components::SimpleFilledCircle>().each([this](const MM::Components::Position2D& p, const Components::SimpleFilledCircle& sfc) {
			_fx_draw.drawSolidCircle(p.pos, sfc.radius, sfc.color);
		});

		scene.view<MM::Components::Position2D, Components::FadingFilledCircle, Components::FXTimer>()
		.each([this](const MM::Components::Position2D& p, const Components::FadingFilledCircle& ffc, const auto& timer) {
			auto tmp_col = ffc.color;
			tmp_col.a = glm::mix(0.f, tmp_col.a, timer.remaining / timer.duration);

			_fx_draw.drawSolidCircle(p.pos, ffc.radius, tmp_col);
		});

		scene.view<MM::Components::Position2D, Components::SimpleFilledRectangle>().each([this](const MM::Components::Position2D& p, const Components::SimpleFilledRectangle& sfr) {
			glm::vec2 points[4];
			points[0] = p.pos + sfr.upper_left; // upper left
			points[1] = p.pos + glm::vec2{sfr.lower_right.x, sfr.upper_left.y}; // upper right
			points[2] = p.pos + sfr.lower_right; // lower right
			points[3] = p.pos + glm::vec2{sfr.upper_left.x, sfr.lower_right.y}; // lower left
			_fx_draw.drawSolidPolygon(points, 4, sfr.color);
		});
	}
	_fx_draw.flushTris();

	// ======================

	// line stuff
	{
		scene.view<Components::SimpleLine>().each([this](const Components::SimpleLine& sl) {
			_fx_draw.drawLine(sl.p1, sl.p2, sl.color);
		});

		scene.view<MM::Components::Position2D, Components::SimpleCircle>().each([this](const MM::Components::Position2D& p, const Components::SimpleCircle& sc) {
			_fx_draw.drawCircle(p.pos, sc.radius, sc.color);
		});

		scene.view<MM::Components::Position2D, Components::SimpleRectangle>().each([this](const MM::Components::Position2D& p, const Components::SimpleRectangle& sfr) {
			glm::vec2 points[4];
			points[0] = p.pos + sfr.upper_left; // upper left
			points[1] = p.pos + glm::vec2{sfr.lower_right.x, sfr.upper_left.y}; // upper right
			points[2] = p.pos + sfr.lower_right; // lower right
			points[3] = p.pos + glm::vec2{sfr.upper_left.x, sfr.lower_right.y}; // lower left
			_fx_draw.drawPolygon(points, 4, sfr.color);
		});
	}
	_fx_draw.flushLines();
}

} // fx_draw::OpenGL::RenderTasks


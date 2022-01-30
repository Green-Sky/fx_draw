#pragma once

#include <mm/opengl/render_task.hpp>

#include "../../fx_draw.hpp"

#include <string>
//#include <memory>

// fwd
//namespace MM::OpenGL {
	//class Shader;
	//class Buffer;
	//class VertexArrayObject;
//}

namespace fx_draw::OpenGL::RenderTasks {

class FXDrawRenderTask : public MM::OpenGL::RenderTask {
	private:
		FXDraw _fx_draw;

	public:
		explicit FXDrawRenderTask(MM::Engine& engine);

		const char* name(void) override { return "FXDraw"; }

		void render(MM::Services::OpenGLRenderer& rs, MM::Engine& engine) override;

	public:
		std::string target_fbo = "display";
};

} // fx_draw::OpenGL::RenderTasks


#pragma once

#include <glm/vec2.hpp>
#include <glm/vec4.hpp>

#include <mm/engine_fwd.hpp>

#include <mm/opengl/camera_3d.hpp>
#include <mm/opengl/shader.hpp>
#include <mm/opengl/instance_buffer.hpp>
#include <mm/opengl/vertex_array_object.hpp>

namespace fx_draw {

// this rendering is loosly based on the box2d debug draw code design

class FXDraw {
	private:
		// buffers
		static const uint32_t _line_vert_buffer_size = 512 * 2; static_assert(_line_vert_buffer_size % 2 == 0);
		uint32_t _line_v_count = 0; // will be %2==0 between calls
		glm::vec2 _line_verts[_line_vert_buffer_size];
		glm::vec4 _line_vert_colors[_line_vert_buffer_size];

		static const uint32_t _tri_vert_buffer_size = 512 * 3; static_assert(_tri_vert_buffer_size % 3 == 0);
		uint32_t _tri_v_count = 0; // will be %3==0 between calls
		glm::vec2 _tri_verts[_tri_vert_buffer_size];
		glm::vec4 _tri_vert_colors[_tri_vert_buffer_size];

		// opengl buffers
		std::unique_ptr<MM::OpenGL::VertexArrayObject> _line_vertex_array;
		std::unique_ptr<MM::OpenGL::InstanceBuffer<glm::vec2>> _line_vertex_buffer;
		std::unique_ptr<MM::OpenGL::InstanceBuffer<glm::vec4>> _line_color_buffer;

		std::unique_ptr<MM::OpenGL::VertexArrayObject> _tri_vertex_array;
		std::unique_ptr<MM::OpenGL::InstanceBuffer<glm::vec2>> _tri_vertex_buffer;
		std::unique_ptr<MM::OpenGL::InstanceBuffer<glm::vec4>> _tri_color_buffer;

		// shaders
		std::shared_ptr<MM::OpenGL::Shader> _line_shader;
		std::shared_ptr<MM::OpenGL::Shader> _tri_shader;

	private:
		glm::mat4 _vp;


	public:
		FXDraw(MM::Engine& engine);
		~FXDraw(void);

		// call this once per frame, before submitting draw calls
		void setCamera(const MM::OpenGL::Camera3D& cam);
		void setCamera(const glm::mat4& cam); // alt

	public:
		void drawLine(const glm::vec2& p1, const glm::vec2& p2, const glm::vec4& color);
		void drawPolyLine(const glm::vec2* points, int32_t point_count, const glm::vec4& color);
		// same as polyline, but connects first and last point
		void drawPolygon(const glm::vec2* points, int32_t point_count, const glm::vec4& color);
		void drawCircle(const glm::vec2& center, float radius, const glm::vec4& color);

		// expects the first point to be in the middle, from which all tris start
		void drawSolidPolygon(const glm::vec2* points, int32_t point_count, const glm::vec4& color);
		void drawSolidCircle(const glm::vec2& center, float radius, const glm::vec4& color);

		void flushLines(void);
		void flushTris(void);

	public:
		const char* line_vert_shader_path = "shader/fx_draw/line_vert.glsl";
		const char* line_frag_shader_path = "shader/fx_draw/line_frag.glsl";

		const char* tri_vert_shader_path = "shader/fx_draw/tri_vert.glsl";
		const char* tri_frag_shader_path = "shader/fx_draw/tri_frag.glsl";

	private:
		inline void addLineVert(const glm::vec2& v, const glm::vec4& color) {
			if (_line_v_count == _line_vert_buffer_size)
				flushLines();

			_line_verts[_line_v_count] = v;
			_line_vert_colors[_line_v_count] = color;
			_line_v_count++;

		}

		inline void addTriVert(const glm::vec2& v, const glm::vec4& color) {
			if (_tri_v_count == _tri_vert_buffer_size)
				flushTris();

			_tri_verts[_tri_v_count] = v;
			_tri_vert_colors[_tri_v_count] = color;
			_tri_v_count++;
		}

		void setupShaderFiles(void);
};

} // fx_draw


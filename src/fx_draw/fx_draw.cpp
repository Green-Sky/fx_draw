#include "./fx_draw.hpp"

#include <glm/gtc/constants.hpp>
#include <glm/trigonometric.hpp>

#include <mm/fs_const_archiver.hpp>

#include <mm/opengl/render_task.hpp> // glsl version macro

#include <cstring>
#include <cassert>

#include <iostream>

namespace fx_draw {

FXDraw::FXDraw(MM::Engine& engine) {
	setupShaderFiles();

	_line_shader = MM::OpenGL::Shader::createF(engine, line_vert_shader_path, line_frag_shader_path);
	assert(_line_shader != nullptr);

	_tri_shader = MM::OpenGL::Shader::createF(engine, tri_vert_shader_path, tri_frag_shader_path);
	assert(_tri_shader != nullptr);

	// line
	{
		_line_vertex_array = std::make_unique<MM::OpenGL::VertexArrayObject>();
		_line_vertex_array->bind();
		glEnableVertexAttribArray(0);
		glEnableVertexAttribArray(1);

		_line_vertex_buffer = std::make_unique<MM::OpenGL::InstanceBuffer<glm::vec2>>();
		_line_vertex_buffer->resize(_line_vert_buffer_size, GL_DYNAMIC_DRAW);
		_line_vertex_buffer->bind();
		glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, (void*)0);
		_line_vertex_buffer->unbind();

		_line_color_buffer = std::make_unique<MM::OpenGL::InstanceBuffer<glm::vec4>>();
		_line_color_buffer->resize(_line_vert_buffer_size, GL_DYNAMIC_DRAW);
		_line_color_buffer->bind();
		glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 0, (void*)0);
		_line_color_buffer->unbind();

		_line_vertex_array->unbind();
	}

	// tri
	{
		_tri_vertex_array = std::make_unique<MM::OpenGL::VertexArrayObject>();
		_tri_vertex_array->bind();
		glEnableVertexAttribArray(0);
		glEnableVertexAttribArray(1);

		_tri_vertex_buffer = std::make_unique<MM::OpenGL::InstanceBuffer<glm::vec2>>();
		_tri_vertex_buffer->resize(_tri_vert_buffer_size, GL_DYNAMIC_DRAW);
		_tri_vertex_buffer->bind();
		glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, (void*)0);
		_tri_vertex_buffer->unbind();

		_tri_color_buffer = std::make_unique<MM::OpenGL::InstanceBuffer<glm::vec4>>();
		_tri_color_buffer->resize(_tri_vert_buffer_size, GL_DYNAMIC_DRAW);
		_tri_color_buffer->bind();
		glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 0, (void*)0);
		_tri_color_buffer->unbind();

		_tri_vertex_array->unbind();
	}
}

FXDraw::~FXDraw(void) {
}

void FXDraw::setCamera(const MM::OpenGL::Camera3D& cam) {
	_vp = cam.getViewProjection();
}

void FXDraw::setCamera(const glm::mat4& cam) {
	_vp = cam;
}

void FXDraw::drawLine(const glm::vec2& p1, const glm::vec2& p2, const glm::vec4& color) {
	//if (_line_v_count == _line_vert_buffer_size - 2)
		//flushLines();

	addLineVert(p1, color);
	addLineVert(p2, color);
}

void FXDraw::drawPolyLine(const glm::vec2* points, int32_t point_count, const glm::vec4& color) {
	assert(point_count >= 2);

	glm::vec2 p1 = points[0];
	for (int32_t i = 1; i < point_count; i++) {
		glm::vec2 p2 = points[i];
		drawLine(p1, p2, color);
		p1 = p2;
	}
}

void FXDraw::drawPolygon(const glm::vec2* points, int32_t point_count, const glm::vec4& color) {
	assert(point_count >= 2);

	glm::vec2 p1 = points[point_count-1]; // first line is last + first
	for (int32_t i = 0; i < point_count; i++) {
		glm::vec2 p2 = points[i];
		drawLine(p1, p2, color);
		p1 = p2;
	}
}

void FXDraw::drawCircle(const glm::vec2& center, float radius, const glm::vec4& color) {
	const float segments = 24.f;
	const float increment = 2 * glm::pi<float>() / segments;

	float sin_inc = glm::sin(increment);
	float cos_inc = glm::cos(increment);
	glm::vec2 r1{1, 0};

	glm::vec2 p1 = center + radius * r1;

	for (int32_t i = 0; i < segments; i++) {
		glm::vec2 r2 {cos_inc * r1.x - sin_inc * r1.y, sin_inc * r1.x + cos_inc * r1.y};
		glm::vec2 p2 = center + radius * r2;

		addLineVert(p1, color);
		addLineVert(p2, color);

		r1 = r2;
		p1 = p2;
	}
}

void FXDraw::drawSolidPolygon(const glm::vec2* points, int32_t point_count, const glm::vec4& color) {
	for (int32_t i = 1; i < point_count - 1; i++) {
		addTriVert(points[0], color);
		addTriVert(points[i], color);
		addTriVert(points[i+1], color);
	}
}

void FXDraw::drawSolidCircle(const glm::vec2& center, float radius, const glm::vec4& color) {
	const float segments = 24.f;
	const float increment = 2 * glm::pi<float>() / segments;

	float sin_inc = glm::sin(increment);
	float cos_inc = glm::cos(increment);
	glm::vec2 r1{cos_inc, sin_inc};

	glm::vec2 p1 = center + radius * r1;

	for (int32_t i = 0; i < segments; i++) {
		glm::vec2 r2 {cos_inc * r1.x - sin_inc * r1.y, sin_inc * r1.x + cos_inc * r1.y};
		glm::vec2 p2 = center + radius * r2;

		addTriVert(center, color);
		addTriVert(p1, color);
		addTriVert(p2, color);

		r1 = r2;
		p1 = p2;
	}
}

void FXDraw::flushLines(void) {
	if (_line_v_count == 0)
		return;

	// draw
	_line_shader->bind();
	_line_shader->setUniformMat4f("projectionMatrix", _vp);
	_line_vertex_array->bind();

	{
		auto* mapped_vert = _line_vertex_buffer->map(_line_v_count, GL_DYNAMIC_DRAW);
		std::memcpy(mapped_vert, _line_verts, _line_v_count * sizeof(glm::vec2));
		_line_vertex_buffer->unmap();
		_line_vertex_buffer->bind();
	}
	{
		auto* mapped_col = _line_color_buffer->map(_line_v_count, GL_DYNAMIC_DRAW);
		std::memcpy(mapped_col, _line_vert_colors, _line_v_count * sizeof(glm::vec4));
		_line_color_buffer->unmap();
		_line_color_buffer->bind();
	}

	glLineWidth(1.5f);
	glDrawArrays(GL_LINES, 0, _line_v_count);

	_line_vertex_array->unbind();
	_line_shader->unbind();

	// reset buffers
	_line_v_count = 0;
}

void FXDraw::flushTris(void) {
	if (_tri_v_count == 0)
		return;

	// draw
	_tri_shader->bind();
	_tri_shader->setUniformMat4f("projectionMatrix", _vp);
	_tri_vertex_array->bind();

	{
		auto* mapped_vert = _tri_vertex_buffer->map(_tri_v_count, GL_DYNAMIC_DRAW);
		std::memcpy(mapped_vert, _tri_verts, _tri_v_count * sizeof(glm::vec2));
		_tri_vertex_buffer->unmap();
		_tri_vertex_buffer->bind();
	}
	{
		auto* mapped_col = _tri_color_buffer->map(_tri_v_count, GL_DYNAMIC_DRAW);
		std::memcpy(mapped_col, _tri_vert_colors, _tri_v_count * sizeof(glm::vec4));
		_tri_color_buffer->unmap();
		_tri_color_buffer->bind();
	}

	//glEnable(GL_BLEND);
	//glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glDrawArrays(GL_TRIANGLES, 0, _tri_v_count);
	//glDisable(GL_BLEND);

	_tri_vertex_array->unbind();
	_tri_shader->unbind();

	// reset buffers
	_tri_v_count = 0;
}

void FXDraw::setupShaderFiles(void) {
	FS_CONST_MOUNT_FILE(line_vert_shader_path,
GLSL_VERSION_STRING
R"(
uniform mat4 projectionMatrix;

layout(location = 0) in vec2 v_position;
layout(location = 1) in vec4 v_color;

out vec4 f_color;

void main() {
	f_color = v_color;
	gl_Position = projectionMatrix * vec4(v_position, 1.0f, 1.0f);
})")

	FS_CONST_MOUNT_FILE(line_frag_shader_path,
GLSL_VERSION_STRING
R"(
#ifdef GL_ES
	precision mediump float;
#endif

in vec4 f_color;
out vec4 _out_color;

void main() {
	_out_color = f_color;
})")

	// ================

	FS_CONST_MOUNT_FILE(tri_vert_shader_path,
GLSL_VERSION_STRING
R"(
uniform mat4 projectionMatrix;

layout(location = 0) in vec2 v_position;
layout(location = 1) in vec4 v_color;

out vec4 f_color;

void main() {
	f_color = v_color;
	gl_Position = projectionMatrix * vec4(v_position, 1.0f, 1.0f);
})")

	FS_CONST_MOUNT_FILE(tri_frag_shader_path,
GLSL_VERSION_STRING
R"(
#ifdef GL_ES
	precision mediump float;
#endif

in vec4 f_color;
out vec4 _out_color;

void main() {
	_out_color = f_color;
})")
}

} // fx_draw


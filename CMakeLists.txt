cmake_minimum_required(VERSION 3.9 FATAL_ERROR)

add_library(fx_draw
	./src/fx_draw/fx_draw.hpp
	./src/fx_draw/fx_draw.cpp

	./src/fx_draw/opengl/render_tasks/fx_renderer.hpp
	./src/fx_draw/opengl/render_tasks/fx_renderer.cpp

	./src/fx_draw/components/fx_timer.hpp

	./src/fx_draw/components/simple_line.hpp
	./src/fx_draw/components/simple_circle.hpp
	./src/fx_draw/components/simple_filled_circle.hpp
	./src/fx_draw/components/fading_filled_circle.hpp

	./src/fx_draw/components/serialize/fx_draw.hpp

	./src/fx_draw/systems/fx_timer.hpp
	./src/fx_draw/systems/fx_timer.cpp
)

target_compile_features(fx_draw PUBLIC cxx_std_17)

target_link_libraries(fx_draw
	opengl_renderer_s
	common_components
	common_components_serialize_json # glm_json
)

target_include_directories(fx_draw PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/src")

########################################


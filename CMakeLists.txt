cmake_minimum_required( VERSION 3.15 )
project( OLED_render VERSION 1.0.0 LANGUAGES CXX )
add_executable( OLED_render 
	src/main.cpp 
	src/uv_surface.cpp 
	src/level_surface.cpp
	src/scene_frame.cpp
	src/render.cpp
	src/oled.cpp 
	src/ascii.cpp 
	src/vec.h 
	src/common.h 
	)
target_compile_features( OLED_render PUBLIC cxx_std_20 )
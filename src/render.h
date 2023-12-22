#pragma once

#include <iostream>
#include <functional>

#include "common.h"
#include "uv_surface.h"

void render_frames_to_console( const std::vector<vector2<char>> & frames, int ms_per_frame = 10 );

void render_oled_frames_to_file( const std::string & file, const std::vector<OledFrame> & frames, int ms_per_frame = 10 );

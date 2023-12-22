#pragma once

#include "common.h"
#include "vec.h"

/*
This object is a simple wrapper for a z-buffered frame.
*/

struct scene_frame {
	struct format {
		int screen_width;
		int screen_height;
		float screen_distance;
		vec3 light_position;
	};

	scene_frame( format );

	bool has_data_at( int x, int y ) const;
	bool contains( int x, int y ) const;
	void write_pixel( int x, int y, float value, float distance );
	void write_scene_frame( const scene_frame & );

	void add_outline();

	format f;
	Frame data;
	Frame z_buffer;
};
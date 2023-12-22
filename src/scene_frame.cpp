#include "scene_frame.h"

scene_frame::scene_frame( scene_frame::format _f )
	: f( _f )
	, data( f.screen_width, f.screen_height, 0 )
	, z_buffer( f.screen_width, f.screen_height, std::numeric_limits<float>::max() )
	{}

bool scene_frame::has_data_at( int x, int y ) const
	{
	return z_buffer.at( x, y ) != std::numeric_limits<float>::max();
	}

bool scene_frame::contains( int x, int y ) const
	{
	return 0 <= x && x < f.screen_width && 0 <= y && y < f.screen_height;
	}

void scene_frame::write_pixel( int x, int y, float value, float distance )
	{
	if( distance < z_buffer.at( x, y ) )
		{
		data.at( x, y ) = value;
		z_buffer.at( x, y ) = distance;
		}
	}
	
void scene_frame::write_scene_frame( const scene_frame & other_scene )
	{
	for( int y = 0; y < data.height; ++y )
		for( int x = 0; x < data.width; ++x )
			write_pixel( x, y, other_scene.data.at( x, y ), other_scene.z_buffer.at( x, y ) );
	}

void scene_frame::add_outline()
	{
	for( int y = 0; y < data.height; ++y )
		{
		for( int x = 0; x < data.width; ++x )
			{
			const bool hit = has_data_at( x, y );
			if( !hit ) continue;
			
			auto safe_z_check = [&]( int x, int y )
				{
				if( x < 0 || z_buffer.width <= x || y < 0 || z_buffer.height <= y )
					return true;
				else return has_data_at( x, y );
				};
			if( !safe_z_check( x - 1, y ) ||
				!safe_z_check( x + 1, y ) ||
				!safe_z_check( x, y - 1 ) ||
				!safe_z_check( x, y + 1 ) )
				data.at( x, y ) = 1;
			}
		}
	}
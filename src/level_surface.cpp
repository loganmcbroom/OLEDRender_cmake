#include "level_surface.h"

#include <algorithm>
#include <cmath>

level_surface::level_surface( 
	std::function<float (vec3, Second)> _equation, 
	std::function<vec3 (vec3, Second)> _normal
	// std::function< vec3 ( Second, int ) > _orientation,
    // std::function< vec3 ( Second, int ) > _translation
	)
	: equation( _equation )
	, normal( _normal )
	// , orientation( _orientation )
	// , translation( _translation )
	{}

void level_surface::add_to_scene_frame( 
	scene_frame & scene_frame,
    Second t, 
    vec3 translation,
    vec3 orientation,
    float scale
	) const
	{
	const float ray_step_size_1 = 0.2f;
	const float ray_step_size_2 = ray_step_size_1 / 20.0f;
	const float ray_step_size_3 = ray_step_size_2 / 20.0f;

	const float aspect = scene_frame.f.screen_height / scene_frame.f.screen_width;

	// For each pixel, ray march
	for( int y = 0; y < scene_frame.f.screen_height; ++y )
		{
		for( int x = 0; x < scene_frame.f.screen_width; ++x )
			{
			const vec2 screen_coords = {
				( float( x ) / scene_frame.f.screen_width  - 0.5f ) * 2.0f,
				( float( y ) / scene_frame.f.screen_height - 0.5f ) * 2.0f
				};

			// This ray will be marched to hunt collisions
			const vec3 ray_base = vec3{
				scene_frame.f.screen_distance,
				screen_coords.x, 
				screen_coords.y
				}.normalized();

			bool hit_1 = false;
			bool hit_2 = false;
			bool hit_3 = false;
			vec3 obj_coords;
			vec3 global_coords = ray_base * ( scene_frame.f.screen_distance / ray_base.x );
			const bool base_eval = equation( global_coords.untranslate( translation ).unrotate( orientation ).unscale( scale ), t ) > 0;
			while( global_coords.mag() < 50 )
				{
					 if( !hit_1 ) global_coords = global_coords + ray_base * ray_step_size_1;
				else if( !hit_2 ) global_coords = global_coords + ray_base * ray_step_size_2;		
				else 			  global_coords = global_coords + ray_base * ray_step_size_3;		
				obj_coords = global_coords.untranslate( translation ).unrotate( orientation ).unscale( scale );

				const bool current_eval = equation( obj_coords, t ) > 0;
				if( current_eval != base_eval )
					{
					// If the coarse steps hit the surface, step back and begin fine steps
					if( !hit_1 )
						{
						hit_1 = true;
						global_coords = global_coords - ray_base * ray_step_size_1;
						}
					else if( !hit_2 )
						{
						hit_2 = true;
						global_coords = global_coords - ray_base * ray_step_size_2;
						}
					else
						{
						hit_3 = true;
						break;
						}
					} 
				}
			if( !hit_3 ) continue;

			const vec3 normal_direction = normal( obj_coords, t ).rotate( orientation ).normalized();

			// If not outline, light based on surface normal pointing towards light source
			const vec3 light_direction = ( scene_frame.f.light_position - global_coords ).normalized(); 
			const float light_cos_theta = light_direction * normal_direction;

			auto light_curve = []( float x )
				{
				if( x < 0.0f ) return 0.1f + 0.1f*x;
				if( x < 0.7f ) return 0.1f + 0.3f/0.7f*x;
				else return 0.4f + 0.6f/0.3f*(x-0.7f);
				};
			const float brightness = std::clamp( light_curve( light_cos_theta ), 0.0f, 1.0f );

			scene_frame.write_pixel( x, y, brightness, global_coords.mag() );
			}
		}
	}


// ==================================================================================================================================================
// Sphere
// ==================================================================================================================================================

level_surface level_surface::sphere( 
	[]( vec3 v, float )
		{
		return v.x*v.x + v.y*v.y + v.z*v.z - 1;
		}, 
	[]( vec3 v, float )
		{
		return v;
		} );

	
// ==================================================================================================================================================
// Spheroid
// ==================================================================================================================================================

level_surface level_surface::spheroid( 
	[]( vec3 v, float )
		{
		return v.x*v.x + v.y*v.y/4 + v.z*v.z - 1;
		}, 
	[]( vec3 v, float )
		{
		v.y /= 2;
		return v;
		} );


// ==================================================================================================================================================
// Heart
// ==================================================================================================================================================

level_surface level_surface::heart( 
	[]( vec3 v, float ) -> float
		{
		const float x = v.x, y = v.y, z = v.z;
		const float c = x*x + 9.0f/4.0f*y*y + z*z - 1.0f;
		return c*c*c - ( x*x + 9.0f/200.0f*y*y ) * z*z*z;
		}, 
	[]( vec3 v, float ) -> vec3
		{
		const double x = v.x;
		const double y = v.y;
		const double z = v.z;

		const double c = x*x + 9.0/4.0*y*y + z*z - 1;

		const float nx = 6.0*x*c*c - 2.0*z*z*z*x;
		const float ny = 27.0/2.0*y*c*c - 9.0/100.0*z*z*z*y;
		const float nz = 6.0*z*c*c - 3.0*z*z*(x*x + 9.0/200.0*y*y);

		return {nx,ny,nz};
		} );
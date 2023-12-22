#include "uv_surface.h"

#include <algorithm>

#include "common.h"

uv_surface::uv_surface(
	std::function<vec3 (vec2, Second)> _surface, 
	std::function<vec3 (vec3, Second)> _normal
	) 
    : surface( _surface )
    , normal( _normal )
	{}

void uv_surface::add_to_scene_frame( 
    scene_frame & scene_frame,
    Second t, 
    vec3 translation,
    vec3 orientation,
    float scale,
    float u_delta,
    float v_delta
    ) const
    {
    const float aspect = scene_frame.f.screen_height / scene_frame.f.screen_width;

    for( float v = -1; v < 1; v += v_delta )
        {
        for( float u = -1; u < 1; u += u_delta )
            {
            const vec3 obj_coords = surface( vec2{ u, v }, t );
            const vec3 global_coords = obj_coords.scale( scale ).rotate( orientation ).translate( translation );

            // Find the screen coordinates on [-a,a]x[-1,1]
            const float perspective_scale = scene_frame.f.screen_distance / global_coords.x;
            const vec2 screen_coords = 
                {
                global_coords.y * perspective_scale * std::sqrt( aspect ),
                global_coords.z * perspective_scale / std::sqrt( aspect ),
                };

            // Convert to pixel coordinates
            const vec2 pixel_coords =
                {
                ( screen_coords.x / 2.0f + 0.5f ) * scene_frame.f.screen_width,
                ( screen_coords.y / 2.0f + 0.5f ) * scene_frame.f.screen_height,
                };
            if( !scene_frame.contains( pixel_coords.x, pixel_coords.y ) )
                continue;
            
            // Normalized direction towards light from point
            const vec3 light_direction = ( scene_frame.f.light_position - global_coords ).normalized();
            const vec3 normal_direction = normal( obj_coords, t ).rotate( orientation ).normalized();
            const float cos_theta = light_direction * normal_direction;
            
            auto light_curve = []( float x )
                {
                if( x > 0 ) return 0.3f + 0.7f * x;
                else return 0.1f - 0.4f * x;
                };
            const float brightness = std::clamp( light_curve( cos_theta ), 0.0f, 1.0f );

            scene_frame.write_pixel( pixel_coords.x, pixel_coords.y, brightness, global_coords.mag() );
            }
        }
    }

// ==================================================================================================================================================
// Torus
// ==================================================================================================================================================

static const float r0 = 1;
static const float r1 = .3;

static vec3 torus_uv_surface( vec2 uv_surface, Second t )
    {
    const float u = uv_surface.x, v = uv_surface.y;
    const float r = r0 + r1 * cos( pi * v );
    const float theta = pi * u;
    const float y = r * cos( theta );
    const float z = r * sin( theta );
    const float x = r1 * sin( pi * v );

    return { x, y, z };
    }

vec3 torus_normal( vec3 coords, Second t )
    {
    const float y = coords.y;
    const float z = coords.z;
    const float theta = atan2( z, y );

    //return normalize( { coords.x - r0 * cos( theta ), coords[1] - r0 * sin( theta ), coords[2] } );
    return vec3{ coords.x, y - r0 * cos( theta ), z - sin( theta ) };
    }

const uv_surface uv_surface::torus( torus_uv_surface, torus_normal );

// ==================================================================================================================================================
// Wormhole
// ==================================================================================================================================================

static const float width_scale = 0.8;

vec3 wormhole_uv_surface( vec2 uv, Second t )
    {
    const float u = uv.x, v = uv.y;
    const float bloop_height = 4*(0.5f-t);
    const float bloop_amount = std::pow( std::cos( pi/4*( v - bloop_height ) ), 1000.0f );
    const float r = width_scale * ( std::cos( pi * v ) + 1 ) / 2.0f + 0.1f*bloop_amount;
    const float theta = 2.0 * pi * v + pi / 2 * u / 2.0f;

    const float x = r * std::cos( theta );
    const float y = r * std::sin( theta );
    const float z = v;

    const float scale = 1.8;
    return { scale*x, scale*y, scale*z };
    }

// 1.0 * ( cos( pi * v ) + 1 ) / 2.0 * cos( 2.0 * pi * v + pi / 2.0 * u / 2.0 )

vec3 wormhole_normal( vec3 coords, Second t )
    {
    const float x = coords.x;
    const float y = coords.y;
    const float z = coords.z;

    if( x == 0 && y == 0 ) 
        return vec3( { 0, 0, 0 } );

    const float b_a_a = pi/4*z - pi*(0.5f - t);
    const float b_a = std::cos( b_a_a );
    const float b = 0.1f*std::pow( b_a, 1000.0f );
    const float db_dz = 0.1f*1000.0f*std::pow( b_a, 999 ) * -std::sin( b_a_a ) * pi/4;
    const float c = std::cos(pi/2*z);
    const float s = width_scale;
    const float dg_dz = -2.0f*(s*c*c + b)*(-pi*s*c*std::sin(pi/2*z) + db_dz );

    return vec3{ 
        2*coords.x, 
        2*coords.y, 
        coords.mag() * ( width_scale * pi / 2.0f * sin( pi * coords.z ) ) };
    }

const uv_surface uv_surface::wormhole( wormhole_uv_surface, wormhole_normal );
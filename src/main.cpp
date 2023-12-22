#include <cstdlib>

#include "common.h"
#include "vec.h"
#include "uv_surface.h"
#include "level_surface.h"
#include "oled.h"
#include "ascii.h"
#include "render.h"
#include "scene_frame.h"

scene_frame waving_hearts( scene_frame::format format, Second t )
    {
    scene_frame scene( format );

    // static const level_surface background = level_surface(
    //     [&]( vec3 v, Second t ) { return v.z - 0.3f*std::cos( t )*std::cos(2*pi*v.x) - 0.3f*std::cos( t )*std::cos(2*pi*v.y); },
    //     [&]( vec3 v, Second t )
    //         {
    //         return vec3
    //             {
    //             2*pi*0.3f*std::cos( t )*std::sin(2*pi*v.x),
    //             2*pi*0.3f*std::cos( t )*std::sin(2*pi*v.y),
    //             1
    //             };
    //         }
    //     );

    const vec3 heart1_orientation = vec3{ 0, .6f * std::sin( t*6 + pi/3*0 - pi/6 ), pi/2 };
    const vec3 heart2_orientation = vec3{ 0, .6f * std::sin( t*6 + pi/3*1 - pi/6 ), pi/2 };
    const vec3 heart3_orientation = vec3{ 0, .6f * std::sin( t*6 + pi/3*2 - pi/6 ), pi/2 };
    const vec3 heart1_translation = vec3{ 4, -3.0f + 7.0f * std::fmod( t + 0.000f, 1.0f ), -2.5f + 0.4f*std::sin( pi*2*(t*2 + 0.5f*0 ) ) }; 
    const vec3 heart2_translation = vec3{ 4, -3.0f + 7.0f * std::fmod( t + 0.333f, 1.0f ),  0.0f + 0.4f*std::sin( pi*2*(t*2 + 0.5f*1 ) ) }; 
    const vec3 heart3_translation = vec3{ 4, -3.0f + 7.0f * std::fmod( t + 0.666f, 1.0f ),  2.5f + 0.4f*std::sin( pi*2*(t*2 + 0.5f*2 ) ) }; 
    level_surface::heart.add_to_scene_frame( scene, t, heart1_translation, heart1_orientation );
    level_surface::heart.add_to_scene_frame( scene, t, heart2_translation, heart2_orientation );
    level_surface::heart.add_to_scene_frame( scene, t, heart3_translation, heart3_orientation );

    scene.add_outline();

    //background.add_to_scene_frame( scene, { 6, 0, 0 }, { 0, pi/2, 0 }, t ); 

    return scene;
    }

scene_frame heart_and_mountains( scene_frame::format format, Second t )
    {
    scene_frame scene( format );

    // Create mountains object
    const float mountain_osc = 0.5f;
    const float mountain_rot = std::atan(1.0f/2.0f);
    const float mountain_speed = std::sqrt( 8.0f*8.0f + 4.0f*4.0f ) / 2.0f;
    static const level_surface mountains = level_surface(
        [&]( vec3 v, Second t ) { return v.z - 0.4f*std::cos(mountain_osc*pi*v.x) - 0.4f*std::cos(mountain_osc*pi*v.y); },
        [&]( vec3 v, Second t )
            {
            return vec3
                {
                mountain_osc*pi*0.4f*std::sin(mountain_osc*pi*v.x),
                mountain_osc*pi*0.4f*std::sin(mountain_osc*pi*v.y),
                1
                };
            }
        );

    // Render heart
    const vec3 heart_translation = vec3{ 4.3, 0, 2.0f + 0.2f*std::cos(4.0f*pi*t) }; 
    const vec3 heart_orientation = vec3{ 0, 0, t*pi }; 
    level_surface::heart.add_to_scene_frame( scene, t, heart_translation, heart_orientation );
    scene.add_outline();

    // Render mountains
    const vec3 mountain_translation = vec3{ 5, t*mountain_speed, -4.0f };
    const vec3 mountain_orientation = vec3{ 0, 0, mountain_rot };
    mountains.add_to_scene_frame( scene, t, mountain_translation, mountain_orientation );

    // Add stars
    srand( 3 );
    for( int y = format.screen_height/3; y < format.screen_height; ++y )
        for( int x = 0; x < format.screen_width; ++x )
            if( std::rand() % 70 == 0 )
                {
                scene.write_pixel( x, y, 1, 100000 );

                // One in 5 stars is big
                if( std::rand() % 5 == 0 )
                    {
                    if( 0 <= x - 1                   ) scene.write_pixel( x - 1, y, 1, 100000 );
                    if( x + 1 < format.screen_width  ) scene.write_pixel( x + 1, y, 1, 100000 );
                    if( 0 <= y - 1                   ) scene.write_pixel( x, y - 1, 1, 100000 );
                    if( y + 1 < format.screen_height ) scene.write_pixel( x, y + 1, 1, 100000 );
                    }
                }
    
    return scene;
    }

scene_frame vortex( scene_frame::format format, Second t )
    {
    scene_frame scene( format );

    // Render arms
    uv_surface::wormhole.add_to_scene_frame( scene, t, { 4, 0, 0 }, { 0, 0, t*pi } );
    uv_surface::wormhole.add_to_scene_frame( scene, t, { 4, 0, 0 }, { 0, 0, t*pi + pi } );

    scene.add_outline();

    return scene;
    }

scene_frame pulser( scene_frame::format format, Second t )
    {
    scene_frame scene( format );

    // Render background
    const float height_scale = 1.0f;
    const float freq = 6;
    uv_surface wave( 
        [&]( vec2 uv, Second t ) -> vec3 
            {
            uv.x = uv.x * 4;
            uv.y = uv.y * 4;
            const float r = uv.mag();
            const float h = height_scale * std::cos( -4*pi*t + freq*r );
            return { uv.x, uv.y, h };
            }, 
        [&]( vec3 w, Second t ) -> vec3 
            {
            const float r = std::sqrt( w.x*w.x + w.y*w.y );
            if( w.x == 0 && w.y == 0 ) return { 0, 0, 1 };
            return
                {
                height_scale * std::sin( -4*pi*t + freq*r ) / r * freq * w.x,
                height_scale * std::sin( -4*pi*t + freq*r ) / r * freq * w.y,
                1
                };
            } );
    wave.add_to_scene_frame( scene, t, { 5, 0, 0 }, { 0, -pi/2, 0 }, 1.0f, 0.002, 0.002 );

    return scene;
    }


scene_frame space_trainer( scene_frame::format format, Second t )
    {
    scene_frame scene( format );

    const float scale = 2.3f;

    auto add_tori = [&]( vec3 rot, float pow )
        {
        scene_frame tori( format );
        uv_surface::torus.add_to_scene_frame( tori, t, {7, 0, 0}, rot, std::pow( scale, pow ), 0.002f );
        tori.add_outline();
        scene.write_scene_frame( tori );
        };

    add_tori( { 0, -2*pi*t, 0      }, -2.0f );
    add_tori( { 0, 0, -1*pi*t      }, -1.0f );
    add_tori( { 0, 0.5f*pi*t, 0    },  0.0f );
    add_tori( { 0, 0, 0.25f*pi*t   },  1.0f );
    add_tori( { 0, 0, 0            },  2.0f );

    return scene;
    }

void render_oled_generator(
    bool to_file,
    const std::string & name, 
    int num_frames, 
    std::function< scene_frame ( scene_frame::format, Second )> generator,
    float screen_distance = 3,
    vec3 light = { 0, -2, 4 } 
    )
    {
    float ms_per_frame = 20;

    scene_frame::format format;
    format.screen_width = 32;
    format.screen_height = 128;
    format.light_position = light;
    format.screen_distance = screen_distance;

    std::vector<vector2<char>> frames;
    for( int i = 0; i < num_frames; ++i )
        {
        scene_frame scene = generator( format, i*ms_per_frame/1000.0f );
        if( to_file )
            frames.push_back( convert_frame_to_oled_frame( scene.data ) );
        else
            frames.push_back( convert_frame_to_ascii_frame( scene.data ) );
        }

    if( to_file )
        render_oled_frames_to_file( 
            name, 
            frames, 
            ms_per_frame 
            );
    else
        render_frames_to_console(
            frames,
            ms_per_frame
            );
    }

int main()
    {
    render_oled_generator( true, "space_trainer", 200, space_trainer, 3 );
    }


#pragma once

#include <functional>

#include "common.h"
#include "vec.h"
#include "scene_frame.h"

struct uv_surface {
    uv_surface( 
        // Takes uv_surface coords and time, return object coordinates
        std::function<vec3 (vec2, Second)> surface, 
        // Takes object coordinates and time, returns the normal to the surface at the position
        std::function<vec3 (vec3, Second)> normal
        );

    void add_to_scene_frame( 
        scene_frame & scene_frame,
        Second t, 
        vec3 translation,
        vec3 orientation,
        float scale = 1,
        float u_delta = 0.01f,
        float v_delta = 0.01f
        ) const;

    const std::function<vec3 (vec2, Second)> surface;
    const std::function<vec3 (vec3, Second)> normal;

    

    static const uv_surface torus;
    static const uv_surface wormhole;
};


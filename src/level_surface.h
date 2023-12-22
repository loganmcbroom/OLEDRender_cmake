#pragma once

#include <functional>

#include "common.h"
#include "vec.h"
#include "scene_frame.h"

struct level_surface {

    level_surface(
        // Takes a level equation set equal to 0 and time, and surface index, and returns the output of the equation
        std::function<float ( vec3, Second )> equation, 
        // Takes a surface position and time, returns the normal to the surface at that point
        std::function<vec3 ( vec3, Second)> normal
        // // Takes time, returns rotation amount in radians for each axis applied in xyz order
        // std::function< vec3 ( Second, int ) > orientation = []( Second t, int ){ return vec3{ 0, 0, t }; },
        // // Tames time and surface index, returns translation vector
        // std::function< vec3 ( Second, int ) > translation = []( Second, int ){ return vec3{ 8, 0, 0 }; }
        );

    void add_to_scene_frame(
        scene_frame & scene_frame,
        Second t, 
        vec3 translation,
        vec3 orientation,
        float scale = 1
        ) const;

    const std::function<float (vec3, Second )> equation;
    const std::function<vec3 (vec3, Second )> normal;
    // const std::function< vec3 ( Second ) > orientation;
    // const std::function< vec3 ( Second ) > translation;

	static level_surface sphere;
	static level_surface spheroid;
	static level_surface heart;
	//static level_surface heart_scene;
};
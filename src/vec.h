#pragma once

#include <cmath>

struct vec2 {
	float operator*( vec2 other ) const { return x * other.x + y * other.y; }
	float mag() const { return std::sqrt( x*x + y*y ); }
	vec2 normalized() const 
		{ 
		const float m = mag();
		return vec2{ x / m, y / m }; 
		}

	float x, y;
};

struct vec3 {
	float operator*( vec3 v ) const { return x * v.x + y * v.y + z * v.z; }
	vec3 operator+( vec3 v ) const { return { x+v.x, y+v.y, z+v.z}; }
	vec3 operator-( vec3 v ) const { return { x-v.x, y-v.y, z-v.z}; }
	vec3 operator*( float s ) const { return { x*s, y*s, z*s}; }
	
	float mag() const { return std::sqrt( x*x + y*y + z*z ); }
	vec3 normalized() const 
		{ 
		const float m = mag();
		if( std::abs( m ) < 0.0001 ) return {0,0,0};
		return vec3{ x / m, y / m, z / m }; 
		}

	vec3 rotate_x( float theta ) const
		{
		const float c = std::cos( theta );
		const float s = std::sin( theta );
		return vec3{
			x,
			c*y-s*z,
			s*y+c*z
			};
		}

	vec3 rotate_y( float theta ) const
		{
		const float c = std::cos( theta );
		const float s = std::sin( theta );
		return vec3{
			c*x+s*z,
			y,
			-s*x+c*z
			};
		}

	vec3 rotate_z( float theta ) const
		{
		const float c = std::cos( theta );
		const float s = std::sin( theta );
		return vec3{
			c*x-s*y,
			s*x+c*y,
			z
			};
		}

	vec3 rotate( vec3 r ) const
		{
		return rotate_x( r.x ).rotate_y( r.y ).rotate_z( r.z );
		}

	vec3 unrotate( vec3 r ) const
		{
		return rotate_z( -r.z ).rotate_y( -r.y ).rotate_x( -r.x );
		}

	vec3 translate( vec3 v ) const 
		{ 
		return *this + v; 
		}

	vec3 untranslate( vec3 v ) const 
		{
		return *this - v;
		}

	vec3 scale( float f ) const
		{
		return *this * f;
		}

	vec3 unscale( float f ) const
		{
		return *this * (1.0f / f );
		}

	float x, y, z;
};
#pragma once

#include <vector>

template <typename T>
struct vector2 {
	size_t width;
	size_t height;
	std::vector<T> data;

	vector2( size_t w, size_t h, T v ) : width( w ), height( h ), data( w*h, v ) {}
	// T operator[]( size_t n ) const { return data[n]; }
	// T& operator[]( size_t n ){ return data[n]; }
	T at( int x, int y ) const { return data[y * width + x]; }
	T& at( int x, int y ) { return data[y * width + x]; }
	size_t size() const { return data.size(); }
};
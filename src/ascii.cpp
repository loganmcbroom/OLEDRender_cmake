#include "ascii.h"

#include <iostream>

#include "render.h"

static std::string shades( " ,-~:;!=*#$@" );

AsciiFrame convert_frame_to_ascii_frame( const Frame & frame )
	{
	AsciiFrame ascii_frame( frame.width, frame.height, 0 );

    for( int y = 0; y < frame.height; ++y ) 
        {
        for( int x = 0; x < frame.width; ++x ) 
            {
            const float brightness = frame.at(x, y);
            const int shade = floor( brightness * ( float( shades.length() ) - 0.01f ) );
            ascii_frame.at(x,y) = shades[shade];
            }
        }

	return ascii_frame;
	}
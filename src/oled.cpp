#include "oled.h"

#include <vector>
#include <ios>
#include <iomanip>
#include <sstream>
#include <map>
#include <functional>
#include <iostream>

#include "common.h"
#include "ascii.h"
#include "render.h"

static bool dither( float l, int x, int y )
    {
    static const std::map< float, std::function<bool(int, int)>> ditherMap = 
        {
        { 0.0f,      []( int x, int y ){ return false; } },
        { 0.11f,     []( int x, int y ){ return x % 3 == 0 && y % 3 == 0; } },
        { 0.25f,     []( int x, int y ){ return x % 2 == 0 && y % 2 == 0; } },
        { 0.33f,     []( int x, int y ){ return ( x + y ) % 3 == 0; } },
        { 0.5f,      []( int x, int y ){ return ( x + y ) % 2 == 0; } },
        { 0.66f,     []( int x, int y ){ return ( x + y ) % 3 != 0; } },
        { 0.75f,     []( int x, int y ){ return x % 2 == 0 || y % 2 == 0; } },
        { 0.88f,     []( int x, int y ){ return x % 3 != 0 || y % 3 != 0; } },
        { 1.0f,      []( int x, int y ){ return true; } }
        };

    auto iter = ditherMap.upper_bound( l );
    auto const hi = iter;
    --iter;
    auto const lo = iter;
    if( hi == ditherMap.end() ) return true;

    return ( hi->first - l > l - lo->first ? lo : hi )->second( x, y );
    };

OledFrame convert_frame_to_oled_frame( const Frame & frame, bool invert )
    {
    OledFrame oled_frame( frame.width, frame.height, ' ' );

    const char on_char = invert? ' ' : '#';
    const char off_char = invert? '#' : ' ';

    for( int y = 0; y < frame.height; ++y ) 
        {
        for( int x = 0; x < frame.width; ++x ) 
            {
            const bool on = dither( frame.at(x,y), x, y );
            oled_frame.at(x,y) = on? on_char : off_char;
            }
        }

    return oled_frame;
    }

HexString convert_oled_frame_to_hex_data( OledFrame d )
    {
    std::string b;
    for( int x = 0; x < d.width/8; ++x )
        {
        for( int y = 0; y < d.height; ++y )
            {
            int byte = 0;
            for( int bit = 0; bit < 8; ++bit )
                if( d.at( x * 8 + bit, y ) == '#' ) byte += 1 << bit;

            std::stringstream ss;
            ss << "0x" << std::hex << std::setfill('0') << std::setw(2) << byte << " ";
            b += ss.str();

            b += ", ";
            }
        b += "\n";
        }
    return b;
    }

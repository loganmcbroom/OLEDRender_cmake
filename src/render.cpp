#include "render.h"

#include <iostream>
#include <windows.h>
#include <thread>
#include <fstream>

#include "ascii.h"
#include "oled.h"

void clear_screen()
    {
    HANDLE                     hStdOut;
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    DWORD                      count;
    DWORD                      cellCount;
    COORD                      homeCoords = { 0, 0 };

    hStdOut = GetStdHandle( STD_OUTPUT_HANDLE );
    if (hStdOut == INVALID_HANDLE_VALUE) return;

    /* Get the number of cells in the current buffer */
    if (!GetConsoleScreenBufferInfo( hStdOut, &csbi )) return;
    cellCount = csbi.dwSize.X *csbi.dwSize.Y;

    /* Fill the entire buffer with spaces */
    if (!FillConsoleOutputCharacter(
        hStdOut,
        (TCHAR) ' ',
        cellCount,
        homeCoords,
        &count
        )) return;

    /* Fill the entire buffer with the current colors and attributes */
    if (!FillConsoleOutputAttribute(
        hStdOut,
        csbi.wAttributes,
        cellCount,
        homeCoords,
        &count
        )) return;

    /* Move the cursor home */
    SetConsoleCursorPosition( hStdOut, homeCoords );
    }

void print_frame_to_console( const vector2<char> frame )
    {
    clear_screen();

    // Rendering an entire buffer to console is smoother than printing lines one at a time
    std::string buffer( ( frame.width + 1 ) * frame.height, 'X' );
	for( int y = 0; y < frame.height; ++y ) 
        {
        for( int x = 0; x < frame.width; ++x ) 
            buffer[ x + y*(frame.width + 1)] = frame.at(frame.width - 1 - x, frame.height - 1 - y);
		buffer[(y+1)*(frame.width + 1) - 1] = '\n';
        }
    std::cout << buffer;
    }

void print_frame_to_console_sideways( const vector2<char> frame )
    {
    clear_screen();

    // Rendering an entire buffer to console is smoother than printing lines one at a time
    std::string buffer( ( frame.height + 1 ) * frame.width, 'X' );
    for( int x = 0; x < frame.width; ++x ) 
        {
	    for( int y = 0; y < frame.height; ++y ) 
            buffer[ y + x*(frame.height + 1)] = frame.at(frame.width - 1 - x, frame.height - 1 - y);
		buffer[(x+1)*(frame.height + 1) - 1] = '\n';
        }
    std::cout << buffer;
    }

void render_frames_to_console( const std::vector<vector2<char>> & frames, int ms_per_frame )
    {
    int i = 0;
    while( true )
        {
        vector2<char> frame = frames[ i % frames.size() ];
        print_frame_to_console_sideways( frame );
        std::this_thread::sleep_for( std::chrono::milliseconds( ms_per_frame ) );
        ++i;
        }
    }

void render_oled_frames_to_file( const std::string & file, const std::vector<OledFrame> & frames, int ms_per_frame )
    {
    std::ofstream f( file + ".h" );

    if( !f ) 
        throw std::runtime_error( "Failed to open file" );

    f << "static const char PROGMEM " << file << "[" << frames.size() << "][" << frames[0].width*frames[0].height/8 << "] = {\n";

    for( int i = 0; i < frames.size(); ++i )
        {
        f << "{ // " << i << "\n";
        OledFrame oled_frame = frames[i];
        HexString hex = convert_oled_frame_to_hex_data( oled_frame );
        f << hex;
        f << "},\n";
        }

    f << "};";
    }

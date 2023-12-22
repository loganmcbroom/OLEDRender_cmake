#pragma once

#include "common.h"

OledFrame convert_frame_to_oled_frame( const Frame & data, bool invert = false );
HexString convert_oled_frame_to_hex_data( OledFrame d );
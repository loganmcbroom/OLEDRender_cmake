#pragma once  

#include <cmath>
#include <string>

#include "vector2.h"

using Frame = vector2<float>;
using OledFrame = vector2<char>;
using AsciiFrame = vector2<char>;
using HexString = std::string;
using Second = float;

const float pi = std::acos( -1.0 );
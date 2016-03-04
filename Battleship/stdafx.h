// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//

#pragma once

#include "targetver.h"

#define WIN32_LEAN_AND_MEAN             // Exclude rarely-used stuff from Windows headers
// Windows Header Files:
#include <windows.h>
#include <windowsx.h>

// C RunTime Header Files
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>
#include <string>

#include "Math.h"
using namespace BVMath;

#define SCREEN_WIDTH 500
#define SCREEN_HEIGHT 900
#define BOARD_LEFT 50
#define BOARD_RIGHT 350
#define LOWER_BOARD_TOP 500
#define LOWER_BOARD_BOTTOM 800
#define UPPER_BOARD_TOP 100
#define UPPER_BOARD_BOTTOM 400
#define SHIP_SIZE 30
#define EMPTY_SEA -1
#define ATTACK_HIT 5
#define ATTACK_MISS 6

#include <d2d1.h> // Include the Direct2D API.
#pragma comment(lib, "d2d1.lib") // Connect to the Direct2D Import Library.

#include <dwrite.h> // Include the DirectWrite API.
#pragma comment(lib, "dwrite.lib") // Connect to the DirectWrite Import Library.
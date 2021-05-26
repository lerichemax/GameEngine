#pragma once

#include <stdio.h>
#include <sstream> // stringstream
#include <vector>
#include <cmath>
#include <SDL.h>
#undef main

#include "Debugger.h"

inline void SafeDelete(void* pToDelete)
{
	if (pToDelete != nullptr)
	{
		delete(pToDelete);
		pToDelete = nullptr;
	}
}

#define WIN32_LEAN_AND_MEAN
#include <SDL_opengl.h>
#include <windows.h>

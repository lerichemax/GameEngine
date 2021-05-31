#pragma once

#include <stdio.h>
#include <sstream> // stringstream
#include <vector>
#include <cmath>
#include <SDL.h>
#undef main

#include "Debugger.h"

template <typename T>
inline void SafeDelete(T& pToDelete)
{
	if (pToDelete != nullptr)
	{
		delete(pToDelete);
		pToDelete = nullptr;
	}
}

#include <SDL_opengl.h>
#include <windows.h>

using namespace empire;

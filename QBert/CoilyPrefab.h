#pragma once
#include "GameObject.h"

class Pyramid;
class Qube;
class CoilyPrefab final : public empire::GameObject
{
public:
	CoilyPrefab(Qube* pQube, Pyramid* pPyramid);
};

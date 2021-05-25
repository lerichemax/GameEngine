#pragma once
#include "GameObject.h"

class Qube;
class DiskPrefab final : public empire::GameObject
{
public:
	DiskPrefab(Qube* top);
};
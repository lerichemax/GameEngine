#pragma once
#include "Serializer.h"

class Command : public IContextSerializable
{
public:
	Command() = default;
	virtual ~Command() = default;

	virtual void Execute(GameObject const& gObject) = 0;
};
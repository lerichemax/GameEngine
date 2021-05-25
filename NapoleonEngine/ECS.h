#pragma once

using ID = unsigned int;

class Component;
class ECS
{
	template <class T>
	ID GetId();
};

template <class T>
ID ECS::GetId()
{
	static int compId = compId++;
	return compId;
}
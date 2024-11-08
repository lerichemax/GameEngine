#pragma once
#include "Entity.h"
#include "Serializer.h"

#include <set>

class Registry;
class System
{
	friend class Registry;
	friend class Scene;

public:
	virtual ~System() = default;

protected:
	virtual void Initialize() {};
	virtual void Start() {};
	virtual void Update() {};

	System() = default;
	System(System const& other);

	Registry* m_pRegistry;
};
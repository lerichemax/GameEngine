#pragma once
#include "Entity.h"
#include "Serializer.h"

#include <bitset>

class GameObject;
class Component
{
	friend class GameObject;
public:
	Component();
		
	virtual ~Component() = default;
		
	GameObject* const GetGameObject() const { return m_pGameObject; }

	void SetEnable(bool isEnabled){ m_bIsEnabled = isEnabled; }
	bool IsEnable()const { return m_bIsEnabled; }
	
protected:
	GameObject* m_pGameObject;

	virtual void Initialize() = 0;
	virtual void Update() = 0;
	virtual Component* Clone() const = 0;
	
private:
	void RootInitialize(GameObject* pParent);// Initial initialize

	bool m_bIsEnabled;
		
};

using ComponentType = uint8_t;

const ComponentType MAX_COMPONENTS = 32;

using Signature = std::bitset<MAX_COMPONENTS>;

struct ECS_Component : public ISerializable
{
	template<typename T> friend class ComponentArray;

	ECS_Component() = default;
	ECS_Component(bool unique);

	bool IsActive() const;
	bool IsUnique() const;

	void SetActive(bool isActive);

	virtual void Serialize(StreamWriter& writer) const override;
	virtual void Deserialize(JsonReader const* reader) override;

private:
	bool m_IsActive{true};
	bool m_IsUnique{}; //Not yet implemented
	bool m_NeedsUpdate{};
};
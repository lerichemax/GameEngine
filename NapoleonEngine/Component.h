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

struct ECS_Component : public IContextSerializable
{
	template<typename T> friend class ComponentArray;
	friend class GameObject;

	ECS_Component() = default;
	ECS_Component(bool unique);

	virtual ~ECS_Component() = default;

	bool IsActive() const;
	bool IsUnique() const;

	void SetActive(bool isActive);

	void Serialize(StreamWriter& writer) const override;
	void Deserialize(JsonReader const* reader, SerializationMap& context) override;

	void RestoreContext(JsonReader const* reader, SerializationMap const& context) override;

	GameObject* const GetGameObject() const { return m_pGameObject; }

private:
	GameObject* m_pGameObject{};

	bool m_IsActive{true};
	bool m_IsUnique{};
};
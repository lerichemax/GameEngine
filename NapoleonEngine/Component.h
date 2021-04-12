#pragma once

namespace empire
{
	class GameObject;
	class Component
	{

	public:
		Component();
		virtual ~Component() = default;
		
		virtual void Init(GameObject* pParent);
		virtual void Update() = 0;

		GameObject* const GetGameObject() const { return m_pGameObject; }
	protected:
		GameObject* m_pGameObject;
	};
}
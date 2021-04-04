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

		GameObject* const GetGameObject() const { return m_pParentObject; }
	protected:
		GameObject* m_pParentObject;
	};

}
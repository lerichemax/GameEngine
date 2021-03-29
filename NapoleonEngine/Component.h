#pragma once

namespace empire
{
	class GameObject;
	class Component // Eventually make pure virtual
	{
	public:
		Component();
		virtual ~Component() = default;
		
		virtual void Init(GameObject* pParent);
		virtual void Update() = 0;
	
	private:
		GameObject* m_pParentObject;
	};

}
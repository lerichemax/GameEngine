#pragma once

namespace empire
{
	class GameObject;
	class Component
	{
		friend class GameObject;
	public:
		Component();
		virtual ~Component() = default;
		
		virtual void Update() = 0;

		GameObject* const GetGameObject() const { return m_pGameObject; }
	protected:
		GameObject* m_pGameObject;

		virtual void Initialize() = 0;
	private:
		void RootInitialize(GameObject* pParent);// Initial initialize
		
	};
}
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
}
#pragma once

namespace dae
{
	class SceneObject;
	class Component // Eventually make pure virtual
	{
	public:
		Component();
		virtual ~Component() = default;
		
		virtual void Init(SceneObject* pParent);
		virtual void Update() = 0;
	
	private:
		SceneObject* m_pParentObject;
	};

}
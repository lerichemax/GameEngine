#pragma once
#include "SceneObject.h"
#include "Observer.h"
#include "Component.h"

constexpr unsigned int MAX_OBSERVERS = 32;
namespace dae
{
	class SubjectComponent final : public Component
	{
	public:
		SubjectComponent() = default;
		SubjectComponent(SubjectComponent const& other) = delete;
		SubjectComponent(SubjectComponent&& other) = delete;
		SubjectComponent& operator=(SubjectComponent const& rhs) = delete;
		SubjectComponent& operator=(SubjectComponent&& rhs) = delete;
		~SubjectComponent();

		void Update() override {}
		
		void AddObserver(Observer* pObserver);
		void RemoveObserver(Observer* pObserver);
		void Notify(SceneObject const& object, Event event);
	private:
		Observer* m_pObservers[MAX_OBSERVERS];
		unsigned int m_NbrObservers;

		void ShiftArray(int startIndex);
	};
	
}

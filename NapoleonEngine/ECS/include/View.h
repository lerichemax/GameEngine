#pragma once
#include "ComponentManager.h"

class ecs::Registry;

namespace ecs {
	template<ComponentDerived... Components>
	class View
	{
	public:
		View(Registry* const pRegistry);

		class ViewIterator
		{
		public:
			ViewIterator(Registry* const pRegistry, Signature signature, std::vector<Entity>::const_iterator it);

			bool operator==(ViewIterator const& other);
			bool operator!=(ViewIterator const& other);

			ViewIterator& operator=(ViewIterator const& other);
			ViewIterator& operator++();
			Entity operator*();

		private:
			Registry* const m_pRegistry;
			Signature m_Signature;
			std::vector<Entity>::const_iterator m_Iterator;

			void Advance();
		};

		ViewIterator begin();
		ViewIterator end();

	private:
		Registry* const m_pRegistry;
		Signature m_Signature;

		Signature BuildSignature() const;
	};
}

#include "View.inl"

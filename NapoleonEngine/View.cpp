#include "PCH.h"
#include "View.h"

#include "Registry.h"

template<ComponentDerived... Components>
View<Components...>::View(Registry* const pRegistry)
	:m_pRegistry{pRegistry},
	m_Signature{BuildSignature()}
{

}

template<ComponentDerived... Components>
Signature View<Components...>::BuildSignature() const
{
	Signature signature;
	(signature.set(m_pRegistry->GetComponentType<Components>()), ...);

	return signature;
}
template<ComponentDerived... Components>
View<Components...>::ViewIterator View<Components...>::begin()
{
	return ViewIterator{ m_pRegistry, m_Signature, m_pRegistry->m_pEntityManager->m_CreatedEntities.cbegin()};
}

template<ComponentDerived... Components>
View<Components...>::ViewIterator View<Components...>::end()
{
	return ViewIterator{ m_pRegistry, m_Signature, m_pRegistry->m_pEntityManager->m_CreatedEntities.cend() };
}

template<ComponentDerived... Components>
View<Components...>::ViewIterator::ViewIterator(Registry* const pRegistry, Signature signature, std::vector<Entity>::const_iterator it)
	:m_pRegistry{pRegistry},
	m_Signature{signature},
	m_Iterator{it}
{
	Advance();
}

template<ComponentDerived... Components>
bool View<Components...>::ViewIterator::operator==(ViewIterator const& other)
{
	return m_Iterator == other.m_Iterator;
}

template<ComponentDerived... Components>
bool View<Components...>::ViewIterator::operator!=(ViewIterator const& other)
{
	return m_Iterator != other.m_Iterator;
}

template<ComponentDerived... Components>
void View<Components...>::ViewIterator::Advance()
{
	while (m_Iterator != m_pRegistry->m_pEntityManager->m_CreatedEntities.cend() && m_pRegistry->m_pEntityManager->EntityHasATag(*m_Iterator))
	{
		++m_Iterator;
	}
}
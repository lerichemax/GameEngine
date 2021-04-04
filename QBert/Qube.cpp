#include "PCH.h"
#include "Qube.h"
#include "RendererComponent.h"
#include "GameObject.h"
#include "Subject.h"
#include "GameObserver.h"

Qube::Qube(Texture2D* pText, Texture2D* pFlippedText)
	:m_pTexture(pText),
	m_pFlippedTexture(pFlippedText),
	m_pSubject(new Subject{})
{
}

void Qube::SetTexture(Texture2D* pText)
{
	m_pTexture = pText;
	m_pParentObject->GetComponent<RendererComponent>()->SetTexture(m_pTexture);
}

void Qube::AddConnection(ConnectionDirection dir, Qube* const pConnection)
{
	m_pConnections[(int)dir] = pConnection;
}

bool Qube::HasConnection(ConnectionDirection dir) const
{
	return m_pConnections[(int)dir] != nullptr;
}

void Qube::Flip()
{
	if (!m_bIsFlipped)
	{
		m_pParentObject->GetComponent<RendererComponent>()->SetTexture(m_pFlippedTexture);
		m_bIsFlipped = true;
		m_pSubject->Notify(this, (int)GameEvents::QubeFlipped);
	}
}
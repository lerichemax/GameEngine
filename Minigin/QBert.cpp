#include "MiniginPCH.h"
#include "QBert.h"
#include "TransformComponent.h"
#include "PlayerObserver.h"

#include "TextRendererComponent.h"
using namespace dae;

QBert::QBert(Tag tag)
	:GameObject{ tag },
	m_NbrLives{ 3 },
	m_NbrPoints{}
{
	GetComponent<TransformComponent>().SetPosition(216.f, 120.f, 0.f);
}

void QBert::Die()
{
	m_NbrLives--;
	Notify(this, Event::PlayerDied);

}

void QBert::EarnPoints()
{
	m_NbrPoints++;
	Notify(this, Event::IncreasePoints);
}
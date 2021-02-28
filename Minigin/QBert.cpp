#include "MiniginPCH.h"
#include "QBert.h"
#include "TransformComponent.h"
#include "SubjectComponent.h"
#include "PlayerObserver.h"

#include "TextRendererComponent.h"
using namespace dae;

QBert::QBert(UIObject* plivesCounter)
	:GameObject{Tag::player},
	m_NbrLives{3},
	m_pScreenCounter{plivesCounter}
{
	GetComponent<TransformComponent>().SetPosition(216.f, 120.f,0.f);
	AddComponent<SubjectComponent>();
	GetComponent<SubjectComponent>().AddObserver(new PlayerObserver{});
	m_pScreenCounter->GetComponent<TransformComponent>().SetPosition(20.f, 50.f, 0.f);
	m_pScreenCounter->GetComponent<TextRendererComponent>().SetText("Lives " + std::to_string(m_NbrLives));
}

void QBert::Die()
{
	m_NbrLives--;
	GetComponent<SubjectComponent>().Notify(*this, Event::PlayerDied);
	m_pScreenCounter->GetComponent<TextRendererComponent>().SetText("Lives " + std::to_string(m_NbrLives));
}
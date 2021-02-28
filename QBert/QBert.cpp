#include "MiniginPCH.h"
#include "QBert.h"
#include "TransformComponent.h"
#include "SubjectComponent.h"
#include "PlayerObserver.h"

using namespace dae;

QBert::QBert()
	:GameObject{Tag::player},
	m_NbrLives{3}
{
	AddComponent<TransformComponent>(216.f, 120.f);
	AddComponent<SubjectComponent>();
	GetComponent<SubjectComponent>().AddObserver(new PlayerObserver{});
}

void QBert::Die()
{
	m_NbrLives--;
	GetComponent<SubjectComponent>().Notify(*this, Event::PlayerDied);
}
#include "PCH.h"
#include "QBertScene.h"

#include "GameObject.h"
#include "TextRendererComponent.h"


using namespace empire;

QBertScene::QBertScene(std::string const& name, Level startLevel)
	: Scene(name),
	m_Level(startLevel),
	m_pQbert(nullptr),
	m_pPyramid(nullptr),
	m_pManager(nullptr)
{
}

void QBertScene::CustomOnActivate()
{
	ResetGame();
}

void QBertScene::EndGame(bool isWin)
{
	if (isWin)
	{
		m_pGameOverMenu->GetComponentInChildren<TextRendererComponent>()->SetText("You Win!");
	}
	else
	{
		m_pGameOverMenu->GetComponentInChildren<TextRendererComponent>()->SetText("Game Over");
	}
	m_pGameOverMenu->SetActive(true);
}
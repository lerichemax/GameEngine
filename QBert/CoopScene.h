#pragma once
#include "QBertScene.h"
#include "Scene.h"

class CoopScene  final : public QBertScene
{
public:
	CoopScene();
	

	void ResetScene(Level newLevel) override;
	void ResetGame() override;

private:
	QBert* m_pQbertP2;

	void Initialize() override;
	void DeclareInput() override;
};

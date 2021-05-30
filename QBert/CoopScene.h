#pragma once
#include "QBertScene.h"
#include "Scene.h"

class CoopScene  final : public QBertScene
{
public:
	CoopScene();
	void Initialize() override;

	void ResetScene(Level newLevel) override;
	void ResetGame() override;

private:
	QBert* m_pQbertP2;
};

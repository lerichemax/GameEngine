#include "PCH.h"
#include "TestScene.h"


TestScene::TestScene()
	:Scene("TestScene")
{
}

void TestScene::Initialize()
{
	auto const pObject = Instantiate("QBert");

	pObject->GetTransform()->SetLocation(250, 250);
	pObject->GetTransform()->Scale(1.5f);
}
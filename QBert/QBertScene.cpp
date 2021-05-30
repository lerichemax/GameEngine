#include "PCH.h"
#include "QBertScene.h"



using namespace empire;

QBertScene::QBertScene(std::string const& name, Level startLevel)
	: Scene(name),
	m_Level(startLevel),
	m_pQbert(nullptr),
	m_pPyramid(nullptr),
	m_pManager(nullptr)
{
}
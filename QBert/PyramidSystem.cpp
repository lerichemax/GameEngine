#include "PCH.h"
#include "PyramidSystem.h"
#include "QubeSystem.h"

#include "PyramidComponent.h"
#include "QubeComponent.h"

#include "GameObject.h"
#include "TransformComponent.h"
#include "RendererComponent.h"
#include "ObserverManager.h"
#include "Timer.h"
#include "PrefabsManager.h"
#include "GameManager.h"

#include "QBert.h"
#include "Texture2D.h"

#include <list>

#include "ColoredDisk.h"

void PyramidSystem::Initialize()
{
	assert(!m_Entities.empty() && "Pyramid System : No entities matching Pyramid system signature.");

	Entity pyramidEntity = *m_Entities.begin();
	
	glm::vec2 startPos{ 0,0 };
	glm::vec2 lastPos{ startPos };

	auto* const pPyramidComp = m_pRegistry->GetComponent<PyramidComponent>(pyramidEntity);
	auto* const pTransform = m_pRegistry->GetComponent<ECS_TransformComponent>(pyramidEntity);

	//spawn qubes
	for (unsigned int i = pPyramidComp->MAX_WIDTH; i != 0; i--)
	{
		lastPos = startPos;
		for (unsigned int j = 0; j < i; j++)
		{
			GameObject* pQubeObj = Instantiate("Qube", pTransform->GetPosition() + lastPos);
			pPyramidComp->GetGameObject()->AddChild(pQubeObj);
			pPyramidComp->Qubes.push_back(pQubeObj->GetEntity());

			auto pQube = pQubeObj->GetComponent<QubeComponent>();

			if (i == pPyramidComp->MAX_WIDTH)
			{
				pQube->bIsLastRow = true;
			}
			if (j == 0 || j == i - 1)
			{
				pQube->bIsSideColumn = true;
			}

			lastPos.x += pQube->pDefaultText->GetWidth() * pQubeObj->GetTransform()->GetScale().x;
		}
		auto* const pQube = m_pRegistry->GetComponent<QubeComponent>(pPyramidComp->Qubes.back());
		startPos.x += pQube->pDefaultText->GetWidth() * 0.85f; // magic numbers
		startPos.y -= pQube->pDefaultText->GetHeight() * m_pRegistry->GetComponent<ECS_TransformComponent>(pPyramidComp->Qubes.back())->GetScale().y * 0.75f; // magic number
	}
	std::reverse(pPyramidComp->Qubes.begin(), pPyramidComp->Qubes.end());

	CreateConnections(pPyramidComp->Qubes);
	CreateEscheresqueLeftConnections();
	CreateEscheresqueRightConnections();
}

void PyramidSystem::Start()
{
	QubeSystem::OnAnyQubeFlipped.Subscribe([this]() {
		CheckAllQubesFlipped();
		});
}

PyramidSystem::~PyramidSystem()
{
	QubeSystem::OnAnyQubeFlipped.UnsuscribeAll();
}

void PyramidSystem::Update()
{
	//DiskSpawnerTimer();
}

Entity PyramidSystem::GetTop() const
{ 
	auto pPyramidComp = m_pRegistry->GetComponent<PyramidComponent>(*m_Entities.begin());
	return pPyramidComp->Qubes.front(); 
}

void PyramidSystem::DiskSpawnerTimer()
{
	//Spawn Disks
	//if (NbrDisksSpawned < MAX_NBR_DISKS)
	//{
	//	if (m_DiskSpawnTimer < DISK_SPAWNING_INTERVAL)
	//	{
	//		m_DiskSpawnTimer += Timer::GetInstance().GetDeltaTime();

	//	}
	//	else
	//	{
	//		m_pQubes[FindOutsideQubeIndex()]->AddConnectionToDisk();
	//		m_DiskSpawnTimer = 0;
	//		NbrDisksSpawned++;
	//	}
	//}
}

void PyramidSystem::CreateConnections(std::vector<Entity> const& qubes)
{
	unsigned int leftChild{};
	unsigned int rightChild{};
	int offset{ 1 };
	int endOfLine{ 1 };
	int counter{ 0 };
	
	for (int i = 0; i < static_cast<int>(qubes.size()); i++)
	{
		rightChild = i * 2 + offset;
		leftChild = i * 2 + offset+1;
		counter++;
		
		if (counter != endOfLine)
		{	
			offset--;
		}
		else
		{
			counter = 0;
			endOfLine++;
		}
		
		if (leftChild < qubes.size())
		{
			m_pRegistry->GetComponent<QubeComponent>(qubes[i])->Connections[static_cast<int>(ConnectionDirection::downLeft)] = qubes[leftChild];
			m_pRegistry->GetComponent<QubeComponent>(qubes[leftChild])->Connections[static_cast<int>(ConnectionDirection::upRight)] = qubes[i];
		}
		if (rightChild < qubes.size())
		{
			m_pRegistry->GetComponent<QubeComponent>(qubes[i])->Connections[static_cast<int>(ConnectionDirection::downRight)] = qubes[rightChild];
			m_pRegistry->GetComponent<QubeComponent>(qubes[rightChild])->Connections[static_cast<int>(ConnectionDirection::upLeft)] = qubes[i];
		}
	}
}

void PyramidSystem::CreateEscheresqueRightConnections()
{
	//unsigned int idx = static_cast<unsigned int>(m_pQubes.size()) - MAX_WIDTH;
	//int nextlineStartIdx{};

	//for (unsigned int i = MAX_WIDTH; i != 0; i--)
	//{
	//	for (unsigned int j = 0; j < i; j++)
	//	{
	//		if (j == 0)
	//		{
	//			nextlineStartIdx = idx - (i - 1);
	//		}
	//		if (j != i - 1)
	//		{
	//			//m_pQubes[idx]->AddEscheresqueRightConnection(ConnectionDirection::downLeft, m_pQubes[idx - (i - 1)]);
	//			//m_pQubes[idx]->AddEscheresqueRightConnection(ConnectionDirection::downRight, m_pQubes[idx + 1]);
	//			if (j != 0)
	//			{
	//				//m_pQubes[idx - (i - 1)]->AddEscheresqueRightConnection(ConnectionDirection::upRight, m_pQubes[idx]);
	//				//m_pQubes[idx + 1]->AddEscheresqueRightConnection(ConnectionDirection::upLeft, m_pQubes[idx]);
	//			}
	//		}
	//		idx++;
	//	}
	//	idx = nextlineStartIdx;
	//}
}

void PyramidSystem::CreateEscheresqueLeftConnections()
{
	//unsigned int idx = static_cast<unsigned int>(m_pQubes.size()) - 1;
	//
	//for (unsigned int i = MAX_WIDTH; i != 0; i--)
	//{
	//	for (unsigned int j = 0; j < i; j++)
	//	{
	//		if (j != i - 1)
	//		{
	//			//m_pQubes[idx]->AddEscheresqueLeftConnection(ConnectionDirection::downLeft, m_pQubes[idx - 1]);
	//			//m_pQubes[idx]->AddEscheresqueLeftConnection(ConnectionDirection::downRight, m_pQubes[idx - i]);
	//			if (j!=0)
	//			{
	//				//m_pQubes[idx - 1]->AddEscheresqueLeftConnection(ConnectionDirection::upRight, m_pQubes[idx]);
	//				//m_pQubes[idx - i]->AddEscheresqueLeftConnection(ConnectionDirection::upLeft, m_pQubes[idx]);
	//			}
	//		}
	//		idx--;
	//	}
	//}
}

void PyramidSystem::CheckAllQubesFlipped() const
{
	Entity pyramidEntity = *m_Entities.begin();

	auto* const pPyramidComp = m_pRegistry->GetComponent<PyramidComponent>(pyramidEntity);
	for (auto qube : pPyramidComp->Qubes)
	{	
		if (!m_pRegistry->GetComponent<QubeComponent>(qube)->bIsFlipped)
		{
			return;
		}
	}
	OnAllQubesFlipped.Notify(/*NbrDisksSpawned **/ ColoredDisk::GetPoints());
}

void PyramidSystem::Reset(Level level)
{
	//m_DiskSpawnTimer = 0;
	//NbrDisksSpawned = 0;
	//
	auto* const pPyramidComp = m_pRegistry->GetComponent<PyramidComponent>(*m_Entities.begin());
	auto* const pQubeSystem = m_pRegistry->GetSystem<QubeSystem>();

	for (Entity qubeEntity : pPyramidComp->Qubes)
	{
		pQubeSystem->Reset(level, qubeEntity);
	}
}

void PyramidSystem::PartialReset()
{
	//for (auto pQube : m_pQubes)
	//{
	//	pQube->CharacterJumpOut();
	//}
}

unsigned int PyramidSystem::FindOutsideQubeIndex() const
{
	unsigned int randomIndex{};

	//do
	//{
	//	randomIndex = rand() % m_pQubes.size();
	//}
	//while (!IsOutsideOfPyramid(m_pQubes[randomIndex]) && !m_pQubes[randomIndex]->HasConnectionToDisk());
	
	return randomIndex;
}

bool PyramidSystem::IsOutsideOfPyramid(QubeSystem* const pQube) const
{
	/*return (pQube->GetConnection(ConnectionDirection::upLeft) == nullptr || pQube->GetConnection(ConnectionDirection::upRight) == nullptr)
		&& !IsTop(pQube);*/
	return true;
}

bool PyramidSystem::IsTop(QubeSystem* const pQube) const
{
	return true;// pQube == m_pQubes.front();
}

bool PyramidSystem::FindNextQubeToQbert(QubeSystem* const pStartingQube, ConnectionDirection* directions, unsigned int size) const
{
	return true;
	
	//int const currentIdx = GetQubeIndex(pStartingQube);
	//int const targetIdx = GetQBertIndex();

	//if (targetIdx == -1)
	//{
	//	return false;
	//}
	//
	//auto* visited = new bool[m_pQubes.size()];
	//
	////at each index, stores the previous qube and the connection to it
	//std::pair<int,int>* predecessors = new std::pair<int, int>[m_pQubes.size()]; //first qube idx, second connection idx

	////fill both array
	//std::fill_n(visited, m_pQubes.size(), false);
	//std::fill_n(predecessors, m_pQubes.size(), std::make_pair(-1, -1));

	////Breadth first search to fill the predecessors array
	//std::list<int> queue{};
	//queue.push_back(currentIdx);
	//visited[currentIdx] = true;
	//while (!queue.empty())
	//{
	//	//int const q = queue.front();
	//	queue.pop_front();
	//	//auto const pQube = GetQube(q);
	//	bool doBreak{ false };
	//	
	//	for (int i{}; i < 4; i++)
	//	{
	//		/*if (pQube->HasConnection(static_cast<ConnectionDirection>(i)))
	//		{
	//			int nextIdx = GetQubeIndex(pQube->GetConnection(static_cast<ConnectionDirection>(i)));
	//			if (!visited[nextIdx])
	//			{
	//				visited[nextIdx] = true;
	//				predecessors[nextIdx].first = q;
	//				predecessors[nextIdx].second = i;
	//				queue.push_back(nextIdx);

	//				if (nextIdx == targetIdx)
	//				{
	//					doBreak = true;
	//				}
	//			}
	//		}*/
	//	}
	//	if (doBreak)
	//	{
	//		break;
	//	}
	//}

	////Use the predecessors array to find the shortest 
	//std::vector<std::pair<int,int>> path;
	//int crawl = targetIdx;
	//path.push_back(std::make_pair(crawl, -1));
	//while (predecessors[crawl].first != -1)
	//{
	//	path.back().second = predecessors[crawl].second;
	//	path.push_back(predecessors[crawl]);
	//	crawl = predecessors[crawl].first;
	//}

	//delete[] visited;
	//delete[] predecessors;

	//size_t const pathSize = path.size();
	//
	//for (size_t i{1}; i <= size;i++)
	//{
	//	if (i < pathSize)
	//	{
	//		directions[i - 1] = static_cast<ConnectionDirection>(path[pathSize - i].second);
	//	}
	//	else
	//	{
	//		directions[i - 1] = ConnectionDirection::null;
	//	}
	//}
	//return true;
}

void PyramidSystem::Serialize(StreamWriter& writer) const
{
	writer.WriteInt64("type", static_cast<int64>(std::type_index(typeid(PyramidSystem)).hash_code()));
}

void PyramidSystem::SetSignature()
{
	Signature signature;
	signature.set(m_pRegistry->GetComponentType<PyramidComponent>());
	signature.set(m_pRegistry->GetComponentType<ECS_TransformComponent>());

	m_pRegistry->SetSystemSignature<PyramidSystem>(signature);
}

int PyramidSystem::GetQBertIndex() const
{
	return 0;
	//return GetQubeIndex(m_pQBert->GetCurrentQube());
}

int PyramidSystem::GetQubeIndex(QubeSystem* const pQube) const
{
	//for (int i{}; i < static_cast<int>(m_pQubes.size()); i++)
	//{
	//	if (m_pQubes[i] == pQube)
	//	{
	//		return i;
	//	}
	//}

	return -1;
}
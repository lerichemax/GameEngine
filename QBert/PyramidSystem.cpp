#include "PCH.h"
#include "PyramidSystem.h"
#include "QubeSystem.h"
#include "DiskSystem.h"
#include "LivesSystem.h"
#include "GameManagerSystem.h"
#include "CharacterMovementSystem.h"

#include "PyramidComponent.h"
#include "QubeComponent.h"
#include "DiskComponent.h"
#include "MovementComponent.h"
#include "CharacterLives.h"
#include "TransformComponent.h"
#include "RendererComponent.h"
#include "QbertComponent.h"
#include "TextRendererComponent.h"

#include "GameObject.h"

#include "PrefabsManager.h"
#include "Timer.h"
#include "Texture2D.h"

#include <list>

void PyramidSystem::Initialize()
{
	assert(!m_Entities.empty() && "Pyramid System : No entities matching Pyramid system signature.");

	Entity pyramidEntity = *m_Entities.begin();
	
	glm::vec2 startPos{ 0,0 };
	glm::vec2 lastPos{ startPos };

	auto* const pPyramidComp = m_pRegistry->GetComponent<PyramidComponent>(pyramidEntity);
	auto* const pTransform = m_pRegistry->GetComponent<TransformComponent>(pyramidEntity);

	//spawn qubes
	for (unsigned int i = pPyramidComp->MAX_WIDTH; i != 0; i--)
	{
		lastPos = startPos;
		for (unsigned int j = 0; j < i; j++)
		{
			GameObject* pQubeObj = Instantiate("Qube", pTransform->GetLocation() + lastPos);
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
		startPos.y -= pQube->pDefaultText->GetHeight() * m_pRegistry->GetComponent<TransformComponent>(pPyramidComp->Qubes.back())->GetScale().y * 0.75f; // magic number
	}
	std::reverse(pPyramidComp->Qubes.begin(), pPyramidComp->Qubes.end());

	CreateConnections(pPyramidComp->Qubes);
	CreateEscheresqueLeftConnections();
	CreateEscheresqueRightConnections();

	for (Entity entity : pPyramidComp->Qubes)
	{
		m_pRegistry->GetComponentInChildren<TextRendererComponent>(entity)->SetText(std::string(std::to_string(entity) + ", " + std::to_string(GetQubeIndex(entity))));
	}

	for (size_t i = 0; i < pPyramidComp->MAX_NBR_DISKS; i++)
	{
		auto* const pDiskObj = Instantiate("Disk");

		auto* const pDisk = pDiskObj->GetComponent<DiskComponent>();
		pDisk->TargetPosition = m_pRegistry->GetComponent<TransformComponent>(GetTop())->GetLocation();
		pDisk->TargetPosition.y += pDisk->OFFSET;

		pDiskObj->SetActive(false, true);
	}
}

void PyramidSystem::Start()
{
	m_pDiskSystem = m_pRegistry->GetSystem<DiskSystem>();

	QubeSystem::OnAnyQubeFlipped.Subscribe([this]() {
		CheckAllQubesFlipped();
		});

	m_pDiskSystem->OnDiskReachedTop.Subscribe([this](Entity entity) {
		m_pRegistry->GetComponent<PyramidComponent>(*m_Entities.begin())->NbrDisksSpawned--;
		});

	m_pRegistry->GetSystem<LivesSystem>()->OnDied.Subscribe([this](Entity entity, int nbrLives) {
		if (m_pRegistry->HasTag(entity, QBERT_TAG))
		{
			auto* const pPyramid = m_pRegistry->GetComponent<PyramidComponent>(*m_Entities.begin());
			pPyramid->DiskSpawnTimer = 0;
			pPyramid->NbrDisksSpawned = 0;
		}
	});

	m_pRegistry->GetSystem<CharacterMovementSystem>()->OnJumpedOnDisk.Subscribe([this](Entity diskEntity) {
		auto* const pPyramid = m_pRegistry->GetComponent<PyramidComponent>(*m_Entities.begin());

		Entity qubeEntity = *std::find_if(pPyramid->Qubes.begin(), pPyramid->Qubes.end(), [this, diskEntity](Entity qubeEntity) {
			return m_pRegistry->GetComponent<QubeComponent>(qubeEntity)->ConnectionToDisk->Disk == diskEntity;
			});

		auto* const pQube = m_pRegistry->GetComponent<QubeComponent>(qubeEntity);
		pQube->ConnectionToDisk->Direction = ConnectionDirection::null;
		pQube->ConnectionToDisk->Disk = NULL_ENTITY;
		});
}

PyramidSystem::~PyramidSystem()
{
	QubeSystem::OnAnyQubeFlipped.UnsuscribeAll();
}

void PyramidSystem::Update()
{
	DiskSpawnerTimer();
}

Entity PyramidSystem::GetTop() const
{ 
	auto pPyramidComp = m_pRegistry->GetComponent<PyramidComponent>(*m_Entities.begin());
	return pPyramidComp->Qubes.front(); 
}

Entity PyramidSystem::GetRandomTopQube() const
{
	auto pPyramidComp = m_pRegistry->GetComponent<PyramidComponent>(*m_Entities.begin());
	int random{ rand() % 2 + 1 };

	auto* pQube = m_pRegistry->GetComponent<QubeComponent>(pPyramidComp->Qubes[random]);

	if (!pQube->Characters.empty())
	{
		if (random % 2 == 0)
		{
			random--;
		}
		else
		{
			random++;
		}
		pQube = m_pRegistry->GetComponent<QubeComponent>(pPyramidComp->Qubes[random]);

		if (!pQube->Characters.empty())
		{
			return NULL_ENTITY;
		}
	}

	return pPyramidComp->Qubes[random];
}

void PyramidSystem::DiskSpawnerTimer()
{
	auto pPyramidComp = m_pRegistry->GetComponent<PyramidComponent>(*m_Entities.begin());

	//Spawn Disks
	if (pPyramidComp->NbrDisksSpawned < pPyramidComp->MAX_NBR_DISKS)
	{
		if (pPyramidComp->DiskSpawnTimer < pPyramidComp->DISK_SPAWNING_INTERVAL)
		{
			pPyramidComp->DiskSpawnTimer += Timer::GetInstance().GetDeltaTime();

		}
		else
		{
			m_pDiskSystem->SpawnDisk(pPyramidComp->Qubes[FindOutsideQubeIndex(pPyramidComp)]);
			pPyramidComp->DiskSpawnTimer = 0;
			pPyramidComp->NbrDisksSpawned++;
		}
	}
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
	OnAllQubesFlipped.Notify(pPyramidComp->NbrDisksSpawned * DiskComponent::POINTS);
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

unsigned int PyramidSystem::FindOutsideQubeIndex(PyramidComponent* const pPyramid) const
{
	unsigned int randomIndex{};

	do
	{
		randomIndex = rand() % pPyramid->Qubes.size();
	} while (!IsOutsideOfPyramid(pPyramid->Qubes[randomIndex], pPyramid) 
		|| m_pRegistry->GetComponent<QubeComponent>(pPyramid->Qubes[randomIndex])->HasConnectionToDisk());
	
	return randomIndex;
}

bool PyramidSystem::IsOutsideOfPyramid(Entity qubeEntity, PyramidComponent* const pPyramid) const
{
	auto* const pQube = m_pRegistry->GetComponent<QubeComponent>(qubeEntity);

	return ((pQube->GetConnection(ConnectionDirection::upLeft) == NULL_ENTITY || pQube->GetConnection(ConnectionDirection::upRight) == NULL_ENTITY)
		&& qubeEntity != pPyramid->Qubes.front());
}

bool PyramidSystem::TryFindPathTo(Entity startingQube, Entity targetQube, ConnectionDirection* directionsArray, size_t arraySize) const
{
	auto* const pPyramidComp = m_pRegistry->GetComponent<PyramidComponent>(*m_Entities.begin());
	int const currentIdx = GetQubeIndex(startingQube);
	int targetIdx = GetQubeIndex(targetQube);
	bool bTargetOnQube = false;

	if (targetIdx == -1)
	{
		auto* const pDisk = m_pRegistry->GetComponent<DiskComponent>(targetIdx);
		if (IS_VALID(pDisk))
		{
			targetIdx = GetQubeIndex(pDisk->LinkedQube);
			if (targetIdx == -1)
			{
				return false;
			}
			bTargetOnQube = true;
		}
	}
	
	bool* pVisited = new bool[pPyramidComp->Qubes.size()];
	
	//at each index, stores the previous qube and the connection to it
	std::pair<int,int>* pPredecessors = new std::pair<int, int>[pPyramidComp->Qubes.size()]; //first qube idx, second connection idx

	//fill both array
	std::fill_n(pVisited, pPyramidComp->Qubes.size(), false);
	std::fill_n(pPredecessors, pPyramidComp->Qubes.size(), std::make_pair(-1, -1));

	std::vector<std::pair<int, int>> path;

	//Breadth first search to fill the pPredecessors array
	std::list<int> queue{};
	queue.push_back(currentIdx);
	pVisited[currentIdx] = true;
	while (!queue.empty())
	{
		int const q = queue.front();
		queue.pop_front();

		if (q == -1)
		{
			return false;
		}

		auto const pQube = m_pRegistry->GetComponent<QubeComponent>(pPyramidComp->Qubes[q]);
		bool doBreak{ false };
		
		for (int i{}; i < 4; i++)
		{
			if (pQube->HasConnection(static_cast<ConnectionDirection>(i)))
			{
				int nextIdx = GetQubeIndex(pQube->GetConnection(static_cast<ConnectionDirection>(i)));
				if (!pVisited[nextIdx])
				{
					pVisited[nextIdx] = true;
					pPredecessors[nextIdx].first = q;
					pPredecessors[nextIdx].second = i;
					queue.push_back(nextIdx);

					if (nextIdx == targetIdx)
					{
						if (bTargetOnQube)
						{
							auto* const pTargetqube = m_pRegistry->GetComponent<QubeComponent>(pPyramidComp->Qubes[targetIdx]);
							path.push_back(std::make_pair(-1, static_cast<int>(pTargetqube->ConnectionToDisk->Direction)));
						}
						
						doBreak = true;
						break;
					}
				}
			}
		}
		if (doBreak)
		{
			break;
		}
	}

	//Use the pPredecessors array to find the shortest 
	int crawl = targetIdx;
	path.push_back(std::make_pair(crawl, -1));
	while (pPredecessors[crawl].first != -1)
	{
		path.back().second = pPredecessors[crawl].second;
		if (pPredecessors[crawl].first == currentIdx)
		{
			break;
		}
		path.push_back(pPredecessors[crawl]);
		crawl = pPredecessors[crawl].first;
	}

	delete[] pVisited;
	delete[] pPredecessors;

	size_t const pathSize = path.size();
	
	for (size_t i{1}; i <= arraySize;i++)
	{
		if (i <= pathSize)
		{
			directionsArray[i - 1] = static_cast<ConnectionDirection>(path[pathSize - i].second);
		}
		else
		{
			directionsArray[i - 1] = ConnectionDirection::null;
		}
	}
	return true;
}

void PyramidSystem::Serialize(StreamWriter& writer) const
{
	writer.WriteInt64("type", static_cast<int64>(std::type_index(typeid(PyramidSystem)).hash_code()));
}

void PyramidSystem::SetSignature() const
{
	Signature signature;
	signature.set(m_pRegistry->GetComponentType<PyramidComponent>());
	signature.set(m_pRegistry->GetComponentType<TransformComponent>());

	m_pRegistry->SetSystemSignature<PyramidSystem>(signature);
}

int PyramidSystem::GetQubeIndex(Entity qubeEntity) const
{
	auto* const pPyramidComp = m_pRegistry->GetComponent<PyramidComponent>(*m_Entities.begin());

	for (int i{}; i < static_cast<int>(pPyramidComp->Qubes.size()); i++)
	{
		if (pPyramidComp->Qubes[i] == qubeEntity)
		{
			return i;
		}
	}

	return -1;
}
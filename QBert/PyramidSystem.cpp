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
#include "AiControllerComponent.h"

#include "GameObject.h"

#include "PrefabsManager.h"
#include "Timer.h"
#include "Texture2D.h"

#include <list>

void PyramidSystem::Initialize()
{
	auto view = m_pRegistry->GetView<PyramidComponent, TransformComponent>();

	assert(view.begin() != view.end() && "Pyramid System : No entities matching Pyramid system signature.");

	m_PyramidEntity = *view.begin();
	
	glm::vec2 startPos{ 0,0 };
	glm::vec2 lastPos{ startPos };

	auto* const pPyramidComp = m_pRegistry->GetComponent<PyramidComponent>(m_PyramidEntity);
	auto* const pTransform = m_pRegistry->GetComponent<TransformComponent>(m_PyramidEntity);

	//spawn qubes
	for (unsigned int i = pPyramidComp->MAX_WIDTH; i != 0; i--)
	{
		lastPos = startPos;
		for (unsigned int j = 0; j < i; j++)
		{
			auto pQubeObj = Instantiate("Qube", pTransform->GetLocation() + lastPos);
			m_pRegistry->AddChild(pPyramidComp->GetEntity(), pQubeObj->GetEntity());
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
	CreateEscheresqueLeftConnections(pPyramidComp);
	CreateEscheresqueRightConnections(pPyramidComp);

	for (size_t i = 0; i < pPyramidComp->MAX_NBR_DISKS; i++)
	{
		auto pDiskObj = Instantiate("Disk");

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

	m_pDiskSystem->OnDiskReachedTop.Subscribe([this](Entity diskEntity) {
		m_pRegistry->GetComponent<PyramidComponent>(m_PyramidEntity)->NbrDisksSpawned--;

		auto* const pPyramid = m_pRegistry->GetComponent<PyramidComponent>(m_PyramidEntity);

		Entity qubeEntity = *std::find_if(pPyramid->Qubes.begin(), pPyramid->Qubes.end(), [this, diskEntity](Entity qubeEntity) {
			return m_pRegistry->GetComponent<QubeComponent>(qubeEntity)->ConnectionToDisk->Disk == diskEntity;
			});

		auto* const pQube = m_pRegistry->GetComponent<QubeComponent>(qubeEntity);
		pQube->ConnectionToDisk->Direction = ConnectionDirection::null;
		pQube->ConnectionToDisk->Disk = NULL_ENTITY;
	});

	m_pRegistry->GetSystem<LivesSystem>()->OnDied.Subscribe([this](Entity entity, int nbrLives) {
		if (m_pRegistry->EntityHasTag(entity, QBERT_TAG))
		{
			auto* const pPyramid = m_pRegistry->GetComponent<PyramidComponent>(m_PyramidEntity);
			pPyramid->DiskSpawnTimer = 0;
			pPyramid->NbrDisksSpawned = 0;
		}
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
	auto pPyramidComp = m_pRegistry->GetComponent<PyramidComponent>(m_PyramidEntity);
	return pPyramidComp->Qubes.front(); 
}

Entity PyramidSystem::GetRandomTopQube() const
{
	auto* const pPyramidComp = m_pRegistry->GetComponent<PyramidComponent>(m_PyramidEntity);
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

Entity PyramidSystem::GetSpawnQube(MovementMode mode) const
{
	switch (mode)
	{
	case MovementMode::Normal:
		return GetRandomTopQube();
		break;
	case MovementMode::EscheresqueLeft:
		return GetEscheresqueLeftTop();
		break;
	case MovementMode::EscheresqueRight:
		return GetEscheresqueRightTop();
		break;
	}

	return false;
}

Entity PyramidSystem::GetQubeAtIndex(int index) const
{
	auto* const pPyramid = m_pRegistry->GetComponent<PyramidComponent>(m_PyramidEntity);

	if (pPyramid->Qubes.size() <= index)
	{
		return NULL_ENTITY;
	}

	return pPyramid->Qubes[index];
}

Entity PyramidSystem::GetEscheresqueLeftTop() const
{ 
	auto* const pPyramidComp = m_pRegistry->GetComponent<PyramidComponent>(m_PyramidEntity);

	return pPyramidComp->Qubes.back();
}

Entity PyramidSystem::GetEscheresqueRightTop() const
{ 
	auto* const pPyramidComp = m_pRegistry->GetComponent<PyramidComponent>(m_PyramidEntity);

	return pPyramidComp->Qubes[pPyramidComp->Qubes.size() - pPyramidComp->MAX_WIDTH];
}

void PyramidSystem::DiskSpawnerTimer()
{
	auto pPyramidComp = m_pRegistry->GetComponent<PyramidComponent>(m_PyramidEntity);

	//Spawn Disks
	if (pPyramidComp->NbrDisksSpawned < pPyramidComp->MAX_NBR_DISKS)
	{
		if (pPyramidComp->DiskSpawnTimer < pPyramidComp->DISK_SPAWNING_INTERVAL)
		{
			pPyramidComp->DiskSpawnTimer += Timer::Get().GetDeltaTime();

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

void PyramidSystem::CreateEscheresqueRightConnections(PyramidComponent* const pPyramid)
{
	unsigned int idx = static_cast<unsigned int>(pPyramid->Qubes.size()) - pPyramid->MAX_WIDTH;
	int nextlineStartIdx{};

	for (unsigned int i = pPyramid->MAX_WIDTH; i != 0; i--)
	{
		for (unsigned int j = 0; j < i; j++)
		{
			if (j == 0)
			{
				nextlineStartIdx = idx - (i - 1);
			}
			if (j != i - 1)
			{
				m_pRegistry->GetComponent<QubeComponent>(pPyramid->Qubes[idx])->EscheresqueRightConnections[static_cast<int>(ConnectionDirection::downLeft)] 
					= pPyramid->Qubes[idx - (i - 1)];
				m_pRegistry->GetComponent<QubeComponent>(pPyramid->Qubes[idx])->EscheresqueRightConnections[static_cast<int>(ConnectionDirection::downRight)] 
					= pPyramid->Qubes[idx + 1];

				if (j != 0)
				{
					m_pRegistry->GetComponent<QubeComponent>(pPyramid->Qubes[idx - (i - 1)])->EscheresqueRightConnections[static_cast<int>(ConnectionDirection::upRight)] 
						= pPyramid->Qubes[idx];
					m_pRegistry->GetComponent<QubeComponent>(pPyramid->Qubes[idx + 1])->EscheresqueRightConnections[static_cast<int>(ConnectionDirection::upLeft)] 
						= pPyramid->Qubes[idx];
				}
			}
			idx++;
		}
		idx = nextlineStartIdx;
	}
}

void PyramidSystem::CreateEscheresqueLeftConnections(PyramidComponent* const pPyramid)
{
	unsigned int idx = static_cast<unsigned int>(pPyramid->Qubes.size()) - 1;
	
	for (unsigned int i = pPyramid->MAX_WIDTH; i != 0; i--)
	{
		for (unsigned int j = 0; j < i; j++)
		{
			if (j != i - 1)
			{
				m_pRegistry->GetComponent<QubeComponent>(pPyramid->Qubes[idx])->EscheresqueLeftConnections[static_cast<int>(ConnectionDirection::downLeft)] 
					= pPyramid->Qubes[idx - 1];
				m_pRegistry->GetComponent<QubeComponent>(pPyramid->Qubes[idx])->EscheresqueLeftConnections[static_cast<int>(ConnectionDirection::downRight)] 
					= pPyramid->Qubes[idx - i];

				if (j != 0)
				{
					m_pRegistry->GetComponent<QubeComponent>(pPyramid->Qubes[idx])->EscheresqueLeftConnections[static_cast<int>(ConnectionDirection::upRight)] 
						= pPyramid->Qubes[idx];
					m_pRegistry->GetComponent<QubeComponent>(pPyramid->Qubes[idx])->EscheresqueLeftConnections[static_cast<int>(ConnectionDirection::upLeft)] 
						= pPyramid->Qubes[idx];
				}
			}
			idx--;
		}
	}
}

void PyramidSystem::CheckAllQubesFlipped() const
{
	auto* const pPyramidComp = m_pRegistry->GetComponent<PyramidComponent>(m_PyramidEntity);
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
	auto* const pPyramidComp = m_pRegistry->GetComponent<PyramidComponent>(m_PyramidEntity);
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
	auto* const pPyramidComp = m_pRegistry->GetComponent<PyramidComponent>(m_PyramidEntity);
	int const currentIdx = GetQubeIndex(startingQube);
	int targetIdx = GetQubeIndex(targetQube);
	bool bTargetOnDisk = false;

	if (targetIdx == -1)
	{
		auto* const pDisk = m_pRegistry->GetComponent<DiskComponent>(targetQube);
		if (IS_VALID(pDisk))
		{
			targetIdx = GetQubeIndex(pDisk->LinkedQube);
			if (targetIdx == -1)
			{
				return false;
			}
			bTargetOnDisk = true;
		}
	}
	
	bool* pVisited = new bool[pPyramidComp->Qubes.size()];
	
	//at each index, stores the previous qube and the connection to it
	std::pair<int,int>* pPredecessors = new std::pair<int, int>[pPyramidComp->Qubes.size()]; //first qube idx, second connection idx

	//fill both array
	std::fill_n(pVisited, pPyramidComp->Qubes.size(), false);
	std::fill_n(pPredecessors, pPyramidComp->Qubes.size(), std::make_pair(-1, -1));

	std::vector<std::pair<int, int>> path;

	if (currentIdx == targetIdx && bTargetOnDisk)
	{
		auto* const pTargetqube = m_pRegistry->GetComponent<QubeComponent>(m_pRegistry->GetComponent<DiskComponent>(targetQube)->LinkedQube);
		path.push_back(std::make_pair(-1, static_cast<int>(pTargetqube->ConnectionToDisk->Direction)));
	}
	else
	{
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
							if (bTargetOnDisk)
							{
								auto* const pTargetqube = m_pRegistry->GetComponent<QubeComponent>(m_pRegistry->GetComponent<DiskComponent>(targetQube)->LinkedQube);
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
			if (path[pathSize - i].second == -1)
			{
				continue;
			}

			directionsArray[i - 1] = static_cast<ConnectionDirection>(path[pathSize - i].second);
		}
		else
		{
			directionsArray[i - 1] = ConnectionDirection::null;
		}
	}
	return true;
}

int PyramidSystem::GetQubeIndex(Entity qubeEntity) const
{
	auto* const pPyramidComp = m_pRegistry->GetComponent<PyramidComponent>(m_PyramidEntity);

	for (int i{}; i < static_cast<int>(pPyramidComp->Qubes.size()); i++)
	{
		if (pPyramidComp->Qubes[i] == qubeEntity)
		{
			return i;
		}
	}

	return -1;
}
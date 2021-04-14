#include "PCH.h"
#include "Pyramid.h"

#include "GameObject.h"
#include "TransformComponent.h"
#include "RendererComponent.h"
#include "Subject.h"
#include "ObserverManager.h"
#include "Timer.h"
#include "ObserverManager.h"

#include "QubeObserver.h"
#include "QubePrefab.h"
#include "QBert.h"
#include "Qube.h"
#include "CoilyPrefab.h"
#include "Coily.h"
#include "SlickSamPrefab.h"
#include "EnemyObserver.h"
#include "SlickSam.h"

#include <list>

Pyramid::Pyramid(unsigned int maxWidth, QBert* pQbert)
	:MAX_WIDTH(maxWidth)
{
	ObserverManager::GetInstance().AddObserver(10, new QubeObserver{ this, pQbert });
	ObserverManager::GetInstance().AddObserver(30, new EnemyObserver{ this });//hardcoded id, change later	
}

Pyramid::~Pyramid()
{
	m_pQubes.clear();
}

void Pyramid::Update()
{
	//DiskSpawnerTimer();
	//CoilySpawnerTimer();
	//SlickSamSpawnerTimer();
}

void Pyramid::DiskSpawnerTimer()
{
	//Spawn Disks
	if (m_NbrDisksSpawned < MAX_NBR_DISKS)
	{
		if (m_DiskSpawnTimer < DISK_SPAWNING_INTERVAL)
		{
			m_DiskSpawnTimer += Timer::GetInstance().GetDeltaTime();

		}
		else
		{
			m_pQubes[FindOutsideQubeIndex()]->AddConnectionToDisk(m_pQubes.front());
			m_DiskSpawnTimer = 0;
			m_NbrDisksSpawned++;
		}

	}
}

void Pyramid::CoilySpawnerTimer()
{
	//Spawn Coilies
	if (m_NbrCoily < MAX_COILY)
	{
		if (m_CoilySpawnTimer < COILY_SPAWN_INTERVAL)
		{
			m_CoilySpawnTimer += Timer::GetInstance().GetDeltaTime();
			return;
		}

		int random{ rand() % 2 + 1 };
		auto coily = new CoilyPrefab(m_pQubes[random], this);
		AddCoilyToArray(coily->GetComponent<Coily>());
		coily->GetComponent<Coily>()->GetSubject()->AddObserver(ObserverManager::GetInstance().GetObserver(30));
		m_CoilySpawnTimer = 0;
		m_NbrCoily++;
		m_pGameObject->AddChild(coily);
	}
}

void Pyramid::AddCoilyToArray(Coily* pCoily)
{
	for (int i = 0; i < MAX_COILY; i++)
	{
		if (m_pCoilies[i] == nullptr)
		{
			m_pCoilies[i] = pCoily;
		}
	}
}

void Pyramid::SlickSamSpawnerTimer()
{
	//Spawn Slick and Sam
	if (m_NbrSlickSam < MAX_SLICKSAM)
	{
		if (m_SlickSamSpawnTimer < SLICKSAM_SPAWN_INTERVAL)
		{
			m_SlickSamSpawnTimer += Timer::GetInstance().GetDeltaTime();
		}
		else
		{
			int random{ rand() % 2 + 1 };
			auto slickSamP = new SlickSamPrefab(m_pQubes[random]);
			m_pGameObject->AddChild(slickSamP);
			slickSamP->GetComponent<SlickSam>()->GetSubject()->AddObserver(ObserverManager::GetInstance().GetObserver(30));
			m_SlickSamSpawnTimer = 0;
			m_NbrSlickSam++;
		}
	}
}

void Pyramid::Initialize()
{
	glm::vec2 startPos{m_pGameObject->GetTransform()->GetPosition()};
	glm::vec2 lastPos{startPos};

	auto observer = ObserverManager::GetInstance().GetObserver(10);
	
	//spawn qubes
	for (unsigned int i = MAX_WIDTH; i != 0; i--)
	{
		lastPos = startPos;
		for (unsigned int j = 0; j < i; j++)
		{
			auto pQube = new QubePrefab{ lastPos };
			m_pQubes.push_back(pQube->GetComponent<Qube>());
			m_pQubes.back()->GetSubject()->AddObserver(observer);
			
			if (i == MAX_WIDTH)
			{
				m_pQubes.back()->SetIsLastRow(true);
			}
			if (j == 0 || j == i-1)
			{
				m_pQubes.back()->SetIsSideColumn(true);
			}
			
			lastPos.x += pQube->GetComponent<RendererComponent>()->GetTextureWidth();
			m_pGameObject->AddChild(pQube);
		}
		startPos.x += m_pQubes.back()->GetGameObject()->GetComponent<RendererComponent>()->GetTextureWidth() / 2;
		startPos.y -= m_pQubes.back()->GetGameObject()->GetComponent<RendererComponent>()->GetTextureHeight()*0.75f;
	}
	std::reverse(m_pQubes.begin(), m_pQubes.end());

	CreateConnections();
}

void Pyramid::CreateConnections()
{
	int leftChild{};
	int rightChild{};
	int offset{ 1 };
	int endOfLine{ 1 };
	int counter{ 0 };
	
	for (int i = 0; i < (int)m_pQubes.size(); i++)
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
		
		if (leftChild < m_pQubes.size())
		{
			m_pQubes[i]->AddConnection(ConnectionDirection::downLeft, m_pQubes[leftChild]);
			m_pQubes[leftChild]->AddConnection(ConnectionDirection::upRight, m_pQubes[i]);
		}
		if (rightChild < m_pQubes.size())
		{
			m_pQubes[i]->AddConnection(ConnectionDirection::downRight, m_pQubes[rightChild]);
			m_pQubes[rightChild]->AddConnection(ConnectionDirection::upLeft, m_pQubes[i]);
		}
	}
}

bool Pyramid::AreAllQubesFlipped() const
{
	for (auto pQube : m_pQubes)
	{
		if (!pQube->IsFlipped())
		{
			return false;
		}
	}
	return true;
}

void Pyramid::Reset()
{
	m_DiskSpawnTimer = 0;
	m_NbrDisksSpawned = 0;
	for (auto pQube : m_pQubes)
	{
		pQube->Reset();
	}
}

unsigned int Pyramid::FindOutsideQubeIndex() const
{
	unsigned int randomIndex{};

	do
	{
		randomIndex = rand() % m_pQubes.size();
	}
	while (!IsOutsideOfPyramid(m_pQubes[randomIndex]) && !m_pQubes[randomIndex]->HasConnectionToDisk());
	
	return randomIndex;
}

bool Pyramid::IsOutsideOfPyramid(Qube* pQube) const
{
	return (pQube->GetConnection(ConnectionDirection::upLeft) == nullptr || pQube->GetConnection(ConnectionDirection::upRight) == nullptr)
		&& !IsTop(pQube);
}

bool Pyramid::IsTop(Qube* pQube) const
{
	return pQube == m_pQubes.front();
}

void Pyramid::FindNextQubeToQbert(Qube* const pStartingQube, ConnectionDirection* directions, int const size) const
{
	for (int i = 0; i < size; i++)
	{
		directions[i] = ConnectionDirection::null;
	}
	
	int currentIdx = GetIndex(pStartingQube);
	int targetIdx = GetQBertIndex();

	if (targetIdx == -1)
	{
		std::fill_n(directions, size, ConnectionDirection::null);
		return;
	}
	
	bool* visited = new bool[m_pQubes.size()];
	
	//at each index, stores the previous qube and the connection to it
	std::pair<int,int>* predecessors = new std::pair<int, int>[m_pQubes.size()]; //first qube idx, second connection idx

	//fill both array
	std::fill_n(visited, m_pQubes.size(), false);
	std::fill_n(predecessors, m_pQubes.size(), std::make_pair(-1, -1));

	//Breadth first search to fill the predecessors array
	std::list<int> queue{};
	queue.push_back(currentIdx);
	visited[currentIdx] = true;
	while (!queue.empty())
	{
		int q = queue.front();
		queue.pop_front();
		auto pQube = GetQube(q);
		bool doBreak{ false };
		
		for (int i{}; i < 4; i++)
		{
			if (pQube->HasConnection((ConnectionDirection)i))
			{
				int nextIdx = GetIndex(pQube->GetConnection((ConnectionDirection)i));
				if (!visited[nextIdx])
				{
					visited[nextIdx] = true;
					predecessors[nextIdx].first = q;
					predecessors[nextIdx].second = i;
					queue.push_back(nextIdx);

					if (nextIdx == targetIdx)
					{
						doBreak = true;
					}
				}
			}
		}
		if (doBreak)
		{
			break;
		}
	}

	//Use the predecessors array to find the shortest 
	std::vector<std::pair<int,int>> path;
	int crawl = targetIdx;
	path.push_back(std::make_pair(crawl, -1));
	while (predecessors[crawl].first != -1)
	{
		path.back().second = predecessors[crawl].second;
		path.push_back(predecessors[crawl]);
		crawl = predecessors[crawl].first;
	}

	delete[] visited;
	delete[] predecessors;

	size_t pathSize = path.size();
	
	for (size_t i{1}; i <= size;i++)
	{
		if (i < pathSize)
		{
			directions[i - 1] = (ConnectionDirection)path[pathSize - i].second;
		}
		else
		{
			directions[i - 1] = ConnectionDirection::null;
		}
	}
}

int Pyramid::GetQBertIndex() const
{
	for (int i{}; i < (int)m_pQubes.size(); i++)
	{
		if (m_pQubes[i]->HasCharacter()) //&& operator doesn't work here...
		{
			if (typeid(*m_pQubes[i]->GetCharacter()) == typeid(QBert))
			{
				return i;
			}
		}
	}
	std::cout << "Pyramid::GetQBertIndex -> QBert Qube index not found\n";
	return -1;
}

int Pyramid::GetIndex(Qube* pQube) const
{
	for (int i{}; i < (int)m_pQubes.size(); i++)
	{
		if (m_pQubes[i] == pQube)
		{
			return i;
		}
	}
	std::cout << "Pyramid::GetQBertIndex -> Qube index not found\n";
	return -1;
}

void Pyramid::CoilyDied(Coily* pCoily)
{
	for (int i{}; i< MAX_COILY;i++)
	{
		if (m_pCoilies[i] == pCoily)
		{
			m_pCoilies[i] = nullptr;
			break;
		}
	}
	m_NbrCoily--;
}
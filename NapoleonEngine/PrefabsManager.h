#pragma once
#include "Singleton.h"
#include "GameObject.h"

#include <memory>

class Scene;
using Prefab = Scene;

class PrefabsManager final : public Singleton<PrefabsManager>
{
public:
	~PrefabsManager();
	PrefabsManager(PrefabsManager const& other) = delete;
	PrefabsManager(PrefabsManager&& other) = delete;
	PrefabsManager& operator=(PrefabsManager const&) = delete;
	PrefabsManager& operator=(PrefabsManager&&) = delete;
		
	void AddPrefab(std::string const& key, GameObject* pPrefab);
	std::shared_ptr<Prefab> CreatePrefab(std::string const& name);
		
	GameObject* Instantiate(std::string const& key) const;
	GameObject* Instantiate(std::string const& key, TransformComponent const& transform) const;
	GameObject* Instantiate(std::string const& key, glm::vec2 const& pos) const;

	std::shared_ptr<Prefab> GetPrefab(std::string const& name) const;

private:
	friend class Singleton<PrefabsManager>;

	class PrefabsManagerImpl;
	std::unique_ptr<PrefabsManagerImpl> m_pImpl;
		
	PrefabsManager();
};

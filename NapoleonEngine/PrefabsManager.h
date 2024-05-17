#pragma once
#include "Singleton.h"
#include "GameObject.h"

#include <memory>

class Scene;
using Prefab = BaseScene;

class PrefabsManager final : public Singleton<PrefabsManager>
{
public:
	~PrefabsManager();
	PrefabsManager(PrefabsManager const& other) = delete;
	PrefabsManager(PrefabsManager&& other) = delete;
	PrefabsManager& operator=(PrefabsManager const&) = delete;
	PrefabsManager& operator=(PrefabsManager&&) = delete;
		
	std::shared_ptr<Prefab> CreatePrefab(std::string const& name);
	std::shared_ptr<Prefab> GetPrefab(std::string const& name) const;

private:
	friend class Singleton<PrefabsManager>;

	class PrefabsManagerImpl;
	std::unique_ptr<PrefabsManagerImpl> m_pImpl;
		
	PrefabsManager();
};

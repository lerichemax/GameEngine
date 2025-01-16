#pragma once

#include <memory>

class Scene;
class Prefab;
class JSonReader;
class PrefabsManager final
{
public:
	PrefabsManager();
	~PrefabsManager();
	PrefabsManager(PrefabsManager const& other) = delete;
	PrefabsManager(PrefabsManager&& other) = delete;
	PrefabsManager& operator=(PrefabsManager const&) = delete;
	PrefabsManager& operator=(PrefabsManager&&) = delete;
		
	std::shared_ptr<Prefab> CreatePrefab(); //Create and save prefab immediately ?
	void SavePrefab(std::shared_ptr<Prefab> pPrefab, std::string const& name);
	void InstantiatePrefab(std::string const& name, Scene* const targetScene) const;

private:
	friend class Singleton<PrefabsManager>;

	class PrefabsManagerImpl;
	std::unique_ptr<PrefabsManagerImpl> m_pImpl;
};

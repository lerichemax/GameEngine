#pragma once
#include "Singleton.h"
#include "GameObject.h"

#include <memory>

class Prefab;
class JSonReader;
class PrefabsManager final : public Singleton<PrefabsManager>
{
public:
	~PrefabsManager();
	PrefabsManager(PrefabsManager const& other) = delete;
	PrefabsManager(PrefabsManager&& other) = delete;
	PrefabsManager& operator=(PrefabsManager const&) = delete;
	PrefabsManager& operator=(PrefabsManager&&) = delete;
		
	std::shared_ptr<Prefab> CreatePrefab();
	void SavePrefab(std::shared_ptr<Prefab> pPrefab, std::string const& name);
	std::unique_ptr<JsonReader> GetPrefabForSerialization(std::string const& name);
	void InstantiatePrefab(std::string const& name, Scene* const pScene) const;

private:
	friend class Singleton<PrefabsManager>;

	class PrefabsManagerImpl;
	std::unique_ptr<PrefabsManagerImpl> m_pImpl;
		
	PrefabsManager();
};

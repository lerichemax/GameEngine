#pragma once
#include "Singleton.h"
#include "GameObject.h"

#include <map>
namespace empire
{
	class PrefabsManager final : public Singleton<PrefabsManager>
	{
	public:
		~PrefabsManager();
		PrefabsManager(PrefabsManager const& other) = delete;
		PrefabsManager(PrefabsManager&& other) = delete;
		PrefabsManager& operator=(PrefabsManager const&) = delete;
		PrefabsManager& operator=(PrefabsManager&&) = delete;
		
		void AddPrefab(std::string const& key, GameObject* pPrefab);
		
		GameObject* Instantiate(std::string const& key) const;
		GameObject* Instantiate(std::string const& key, TransformComponent const& transform) const;
		GameObject* Instantiate(std::string const& key, glm::vec2 const& pos) const;
		//GameObject* Instantiate(std::string const& key, glm::vec3 const& pos, glm::vec3 const& scale) const;
		//GameObject* Instantiate(std::string const& key, glm::vec3 const& pos, glm::vec3 const& scale, float rot) const;
	private:
		friend class Singleton<PrefabsManager>;
		
		PrefabsManager() : m_pPrefabs(){}

		std::map<std::string, GameObject*> m_pPrefabs;
	};
}

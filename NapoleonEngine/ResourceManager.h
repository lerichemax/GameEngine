#pragma once
#include "Singleton.h"
#include <memory>
#include <map>

class Texture2D;
class Font;
class ResourceManager final : public Singleton<ResourceManager>
{
public:
	ResourceManager(ResourceManager const& other) = delete;
	ResourceManager(ResourceManager&& other) = delete;
	ResourceManager& operator=(ResourceManager const& rhs) = delete;
	ResourceManager& operator=(ResourceManager&& rhs) = delete;
	~ResourceManager();
		
	void Init(const std::string& data);
		
	std::shared_ptr<Texture2D> GetTexture(const std::string& file);
	std::shared_ptr<Font> GetFont(const std::string& file, unsigned int size);
	
private:
	friend class Singleton<ResourceManager>;

	class ResourceManagerImpl;
	std::unique_ptr<ResourceManagerImpl> m_pImpl;
	ResourceManager();
};

#pragma once
#include "Singleton.h"
#include <memory>

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
		
	Texture2D* const GetTexture(const std::string& file);
	Font* const GetFont(const std::string& file, unsigned int size);
	
private:
	friend class Singleton<ResourceManager>;

	class ResourceManagerImpl;
	std::unique_ptr<ResourceManagerImpl> m_pImpl;
	ResourceManager();

};

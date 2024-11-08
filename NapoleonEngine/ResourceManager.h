#pragma once
#include "Singleton.h"
#include "SoundEffect.h"

#include <memory>
#include <map>

class Texture2D;
class Font;
struct _TTF_Font;
class ResourceManager final : public Singleton<ResourceManager>
{
	friend class NapoleonEngine;

public:
	ResourceManager(ResourceManager const& other) = delete;
	ResourceManager(ResourceManager&& other) = delete;
	ResourceManager& operator=(ResourceManager const& rhs) = delete;
	ResourceManager& operator=(ResourceManager&& rhs) = delete;
	~ResourceManager();
		
	Texture2D* const GetTexture(const std::string& file);
	bool TryGetTexture(std::string const& fileName, Texture2D*& pTexture);
	Texture2D* const GetTextTexture(_TTF_Font*, const char* txt, SDL_Color Color, int id);

	Font* const GetFont(const std::string& file, unsigned int size);
	ID GetEffect(const std::string& file);
	SoundEffect* const GetEffectById(ID id) const;
	
private:
	friend class Singleton<ResourceManager>;

	class ResourceManagerImpl;
	std::unique_ptr<ResourceManagerImpl> m_pImpl;
	ResourceManager();

	void Init(const std::string& data, Renderer* const pRenderer);
};

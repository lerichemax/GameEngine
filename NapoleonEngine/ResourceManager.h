#pragma once
#include "Singleton.h"
#include "SoundEffect.h"

#include <memory>
#include <map>
#include <SDL_ttf.h>

class Texture2D;
class Font;
class Renderer;
class Animation;
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
	SDL_Texture* const GetSDLTexture(const std::string& file);
	bool TryGetTexture(std::string const& fileName, Texture2D*& pTexture);
	Texture2D* const GetTextTexture(TTF_Font* pFont, const char* txt, SDL_Color Color, int id); // pass font wrapper ?
	Animation* const GetAnimation(std::string const& animationName);
	Animation* const RegisterAnimation(std::string const& animationName, std::string const& textureFiles...);

	Font* const GetFont(const std::string& file, int size);
	ID GetEffect(const std::string& file);
	SoundEffect* const GetEffectById(ID id) const;
	
private:
	friend class Singleton<ResourceManager>;

	class ResourceManagerImpl;
	std::unique_ptr<ResourceManagerImpl> m_pImpl;
	ResourceManager();

	void Init(Renderer* const pRenderer);
};

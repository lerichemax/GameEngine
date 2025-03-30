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

	template<typename... TextureFiles> inline Animation* const RegisterAnimation(std::string const& animationName, TextureFiles&&... textureFiles);

	Font* const GetFont(const std::string& file, int size);
	ID GetEffect(const std::string& file);
	SoundEffect* const GetEffectById(ID id) const;
	
private:
	friend class Singleton<ResourceManager>;

	ResourceManager();

	Renderer* m_pRenderer;

	std::map<std::string, std::unique_ptr<Texture2D>> m_pTextures;
	std::map<int, std::unique_ptr<Texture2D>> m_pTxtTextures;
	std::map<std::string, std::unique_ptr<Animation>> m_pAnimations;

	std::vector<std::unique_ptr<Font>> m_pFonts;
	std::map<std::string, std::unique_ptr<SoundEffect>> m_pEffectsStr;
	std::map<ID, std::unique_ptr<SoundEffect>> m_pEffects;

	void Init(Renderer* const pRenderer);

	Texture2D* const LoadTexture(const std::string& file);
	Texture2D* const SafeLoadTexture(const std::string& file);
	Font* const LoadFont(const std::string& file, unsigned int size);
	SoundEffect* const LoadEffect(const std::string& file);

	Animation* const AddAnimation(std::string const& animationName, std::vector<std::string> const& textureFiles); 
};

template<typename... TextureFiles>
Animation* const ResourceManager::RegisterAnimation(std::string const& animationName, TextureFiles&&... textureFiles)
{
	static_assert((std::is_convertible_v<TextureFiles, std::string> && ...),
		"All texture files must be of type std::string or convertible to std::string");

	std::vector<std::string> files;
	std::vector<std::string> textures{ std::forward<TextureFiles>(textureFiles)... };

	for (const auto& texture : textures) {
		files.push_back(static_cast<std::string>(texture));
	}

	return AddAnimation(animationName, files);
}
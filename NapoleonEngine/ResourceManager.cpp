#include "PCH.h"
#include "ResourceManager.h"
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_mixer.h>

#include "TextRendererComponent.h"

#include "Renderer.h"
#include "Texture2D.h"
#include "Font.h"
#include "Animation.h"

#include <vector>
#include <map>
#include <memory>

#include "NapoleonEngine.h"
#include "SceneManager.h"

#define DATA_PATH "./Data/"

ResourceManager::ResourceManager()
	:m_pTextures(),
	m_pFonts()
{
	TextRendererComponent::OnAnyDestroyed.Subscribe([this](int id) {
		if (m_pTxtTextures.find(id) != m_pTxtTextures.end())
		{
			m_pTxtTextures[id].reset();
			m_pTxtTextures.erase(id);
		}
	});
}

ResourceManager::~ResourceManager()
{
	TextRendererComponent::OnAnyDestroyed.UnsuscribeAll();

	m_pTextures.clear();
	m_pFonts.clear();

	IMG_Quit();
}

void ResourceManager::Init(Renderer* const pRenderer)
{
	m_pRenderer = pRenderer;

	// load support for png and jpg, this takes a while!
	if ((IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG) != IMG_INIT_PNG)
	{
		throw std::runtime_error(std::string("Failed to load support for png's: ") + SDL_GetError());
	}

	if ((IMG_Init(IMG_INIT_JPG) & IMG_INIT_JPG) != IMG_INIT_JPG)
	{
		throw std::runtime_error(std::string("Failed to load support for jpg's: ") + SDL_GetError());
	}

	if (TTF_Init() != 0)
	{
		throw std::runtime_error(std::string("Failed to load support for fonts: ") + SDL_GetError());
	}
}

Texture2D* const ResourceManager::LoadTexture(const std::string& file)
{
	const auto fullPath = DATA_PATH + file;
	auto const texture = IMG_LoadTexture(m_pRenderer->GetSDLRenderer(), fullPath.c_str());
	if (texture == nullptr)
	{
		return nullptr;
	}

	return m_pTextures.insert(std::make_pair(file, std::unique_ptr<Texture2D>(new Texture2D{ texture, file }))).first->second.get();
}

Texture2D* const ResourceManager::SafeLoadTexture(const std::string& file)
{
	const auto fullPath = DATA_PATH + file;
	auto const texture = IMG_LoadTexture(m_pRenderer->GetSDLRenderer(), fullPath.c_str());
	if (texture == nullptr)
	{
		throw std::runtime_error(std::string("Failed to load texture: ") + SDL_GetError());
	}

	return m_pTextures.insert(std::make_pair(file, std::unique_ptr<Texture2D>(new Texture2D{ texture, file }))).first->second.get();
}

Texture2D* const ResourceManager::GetTexture(const std::string& file)
{
	if (m_pTextures.find(file) == m_pTextures.end())
	{
		try
		{
			return SafeLoadTexture(file);

		}
		catch (std::runtime_error const& error)
		{
			LOG_ERROR(error.what());
		}
	}

	return m_pTextures.at(file).get();
}

SDL_Texture* const ResourceManager::GetSDLTexture(const std::string& file)
{
	if (m_pTextures.find(file) == m_pTextures.end())
	{
		try
		{
			return SafeLoadTexture(file)->GetSDLTexture();

		}
		catch (std::runtime_error const& error)
		{
			LOG_ERROR(error.what());
		}
	}

	return m_pTextures.at(file)->GetSDLTexture();
}

bool ResourceManager::TryGetTexture(std::string const& fileName, Texture2D*& pTexture)
{
	if (m_pTextures.find(fileName) == m_pTextures.end())
	{
		Texture2D* pTempText = LoadTexture(fileName);
		if (pTempText != nullptr)
		{
			pTexture = pTempText; 
			return true;
		}
		return false;
	}

	pTexture = m_pTextures.at(fileName).get();
	return true;
}

Texture2D* const ResourceManager::GetTextTexture(TTF_Font* pFont, const char* txt, SDL_Color Color, int id)
{
	const auto surf = TTF_RenderText_Blended(pFont, txt, Color);
	if (surf == nullptr)
	{
		throw std::runtime_error(std::string("Render text failed: ") + SDL_GetError());
	}
	auto const texture = SDL_CreateTextureFromSurface(m_pRenderer->GetSDLRenderer(), surf);
	if (texture == nullptr)
	{
		throw std::runtime_error(std::string("Create text texture from surface failed: ") + SDL_GetError());
	}
	SDL_FreeSurface(surf);

	if (m_pTxtTextures.find(id) == m_pTxtTextures.end())
	{
		return m_pTxtTextures.insert(std::make_pair(id, std::unique_ptr<Texture2D>(new Texture2D{ texture }))).first->second.get();
	}

	m_pTxtTextures[id].reset(new Texture2D{ texture });
	return m_pTxtTextures[id].get();
}

Animation* const ResourceManager::GetAnimation(std::string const& animationName)
{
	auto pAnimIt = m_pAnimations.find(animationName);
	if (pAnimIt != m_pAnimations.end())
	{
		return pAnimIt->second.get();
	}
	return nullptr;
}

Font* const ResourceManager::GetFont(const std::string& file, int size)
{
	auto fontIt = std::find_if(m_pFonts.begin(), m_pFonts.end(), [&file, &size](std::unique_ptr<Font>& pFont)
		{
			return pFont->GetFilePath() == file && pFont->GetSize() == size;
		});
	if (fontIt != m_pFonts.end())
	{
		return fontIt->get();
	}
	else
	{
		return LoadFont(file, size);
	}
}

ID ResourceManager::GetEffect(const std::string& file)
{
	if (m_pEffectsStr.find(file) == m_pEffectsStr.end())
	{
		try
		{
			return LoadEffect(file)->GetId();

		}
		catch (std::runtime_error const& error)
		{
			LOG_ERROR(error.what());
		}
	}

	return m_pEffectsStr.at(file)->GetId();
}

SoundEffect* const ResourceManager::GetEffectById(ID id) const
{
	auto it = m_pEffects.find(id);

	if (it != m_pEffects.end())
	{
		return it->second.get();
	}
	return nullptr;
}

Font* const ResourceManager::LoadFont(const std::string& file, unsigned int size)
{
	std::string fullPath{ DATA_PATH + file };
	_TTF_Font* pFont = TTF_OpenFont(fullPath.c_str(), size);
	if (pFont == nullptr)
	{
		LOG_ERROR("Failed to load font: %s", SDL_GetError());
		return nullptr;
	}

	m_pFonts.push_back(std::make_unique<Font>(pFont, file, size ));
	return m_pFonts.back().get();
}

SoundEffect* const ResourceManager::LoadEffect(const std::string& file)
{
	std::string fullPath{ DATA_PATH + file };
	Mix_Chunk* pMixChunk = Mix_LoadWAV(fullPath.c_str());
	if (pMixChunk == nullptr)
	{
		std::string errorMsg = "SoundEffect: Failed to load " + fullPath + ",\n SDL_mixer Error: " + Mix_GetError() + '\n';
		LOG_ERROR("SoundEffect: Failed to load %s\n SDL_mixer Error: %s", fullPath.c_str(), Mix_GetError());
	}

	auto pEffect = new SoundEffect{  pMixChunk, file };
	m_pEffects.insert(std::make_pair(pEffect->GetId(), std::unique_ptr<SoundEffect>{pEffect}));
	return pEffect;
}

Animation* const ResourceManager::AddAnimation(std::string const& animationName, std::vector<std::string> const& textureFiles)
{
	auto animationPair = m_pAnimations.emplace(animationName, std::make_unique<Animation>(animationName, textureFiles));
	return animationPair.first->second.get();
}

#include "PCH.h"
#include "ResourceManager.h"
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>

#include "TextRendererComponent.h"

#include "Renderer.h"
#include "Texture2D.h"
#include "Font.h"

#include <vector>
#include <map>
#include <memory>

#include "NapoleonEngine.h"
#include "SceneManager.h"

#define DATA_PATH "./Data/"

class ResourceManager::ResourceManagerImpl final
{
public:
	ResourceManagerImpl();
	ResourceManagerImpl(ResourceManagerImpl const& other) = delete;
	ResourceManagerImpl(ResourceManagerImpl&& other) = delete;
	ResourceManagerImpl& operator=(ResourceManagerImpl const& rhs) = delete;
	ResourceManagerImpl& operator=(ResourceManagerImpl&& rhs) = delete;
	~ResourceManagerImpl();

	void Init(Renderer* const pRenderer);

	Texture2D* const GetTexture(const std::string& file);
	SDL_Texture* const GetSDLTexture(const std::string& file);
	bool TryGetTexture(std::string const& fileName, Texture2D*& pTexture);
	Texture2D* const GetTextTexture(TTF_Font*, const char* txt, SDL_Color Color, int id);

	Font* const GetFont(const std::string& file, int size);
	ID GetEffect(const std::string& file);
	SoundEffect* const GetEffectById(ID id) const;

private:
	Renderer* m_pRenderer;

	std::map<std::string, std::unique_ptr<Texture2D>> m_pTextures;
	std::map<int, std::unique_ptr<Texture2D>> m_pTxtTextures;

	std::vector<std::unique_ptr<Font>> m_pFonts;
	std::map<std::string, std::unique_ptr<SoundEffect>> m_pEffectsStr;
	std::map<ID, std::unique_ptr<SoundEffect>> m_pEffects;

	Texture2D* const LoadTexture(const std::string& file);
	Texture2D* const SafeLoadTexture(const std::string& file);
	Font* const LoadFont(const std::string& file, unsigned int size);
	SoundEffect* const LoadEffect(const std::string& file);
};

ResourceManager::ResourceManagerImpl::ResourceManagerImpl()
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

ResourceManager::ResourceManagerImpl::~ResourceManagerImpl()
{
	TextRendererComponent::OnAnyDestroyed.UnsuscribeAll();

	m_pTextures.clear();
	m_pFonts.clear();

	IMG_Quit();
}

void ResourceManager::ResourceManagerImpl::Init(Renderer* const pRenderer)
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

Texture2D* const ResourceManager::ResourceManagerImpl::LoadTexture(const std::string& file)
{
	const auto fullPath = DATA_PATH + file;
	auto const texture = IMG_LoadTexture(m_pRenderer->GetSDLRenderer(), fullPath.c_str());
	if (texture == nullptr)
	{
		return nullptr;
	}

	return m_pTextures.insert(std::make_pair(file, std::unique_ptr<Texture2D>(new Texture2D{ texture, file }))).first->second.get();
}

Texture2D* const ResourceManager::ResourceManagerImpl::SafeLoadTexture(const std::string& file)
{
	const auto fullPath = DATA_PATH + file;
	auto const texture = IMG_LoadTexture(m_pRenderer->GetSDLRenderer(), fullPath.c_str());
	if (texture == nullptr)
	{
		throw std::runtime_error(std::string("Failed to load texture: ") + SDL_GetError());
	}

	return m_pTextures.insert(std::make_pair(file, std::unique_ptr<Texture2D>(new Texture2D{ texture, file }))).first->second.get();
}

Texture2D* const ResourceManager::ResourceManagerImpl::GetTexture(const std::string& file)
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

SDL_Texture* const ResourceManager::ResourceManagerImpl::GetSDLTexture(const std::string& file)
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

bool ResourceManager::ResourceManagerImpl::TryGetTexture(std::string const& fileName, Texture2D*& pTexture)
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

Texture2D* const ResourceManager::ResourceManagerImpl::GetTextTexture(TTF_Font* pFont, const char* txt, SDL_Color Color, int id)
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

Font* const ResourceManager::ResourceManagerImpl::GetFont(const std::string& file, int size)
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

ID ResourceManager::ResourceManagerImpl::GetEffect(const std::string& file)
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

SoundEffect* const ResourceManager::ResourceManagerImpl::GetEffectById(ID id) const
{
	auto it = m_pEffects.find(id);

	if (it != m_pEffects.end())
	{
		return it->second.get();
	}
	return nullptr;
}

Font* const ResourceManager::ResourceManagerImpl::LoadFont(const std::string& file, unsigned int size)
{
	m_pFonts.push_back(std::make_unique<Font>(DATA_PATH + file, size ));
	return m_pFonts.back().get();
}

SoundEffect* const ResourceManager::ResourceManagerImpl::LoadEffect(const std::string& file)
{

	auto pEffect = new SoundEffect{ DATA_PATH + file };
	m_pEffects.insert(std::make_pair(pEffect->GetId(), std::unique_ptr<SoundEffect>{pEffect}));
	return pEffect;
}

ResourceManager::ResourceManager()
	:Singleton<ResourceManager>(),
	m_pImpl(std::make_unique<ResourceManagerImpl>())
{
}

ResourceManager::~ResourceManager()
{

}

void ResourceManager::Init(Renderer* const pRenderer)
{
	m_pImpl->Init(pRenderer);
}

Texture2D* const ResourceManager::GetTexture(const std::string& file)
{
	return m_pImpl->GetTexture(file);
}
SDL_Texture* const ResourceManager::GetSDLTexture(const std::string& file)
{
	return m_pImpl->GetSDLTexture(file);
}

bool ResourceManager::TryGetTexture(std::string const& fileName, Texture2D*& pTexture)
{
	return m_pImpl->TryGetTexture(fileName, pTexture);
}

Texture2D* const ResourceManager::GetTextTexture(TTF_Font* pFont, const char* txt, SDL_Color Color, int id)
{
	return m_pImpl->GetTextTexture(pFont, txt, Color, id);
}

Font* const ResourceManager::GetFont(const std::string& file, int size)
{
	return m_pImpl->GetFont(file, size);
}

ID ResourceManager::GetEffect(const std::string& file)
{
	return m_pImpl->GetEffect(file);
}

SoundEffect* const ResourceManager::GetEffectById(ID id) const
{
	return m_pImpl->GetEffectById(id);
}
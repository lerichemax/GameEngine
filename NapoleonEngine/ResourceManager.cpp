#include "PCH.h"
#include "ResourceManager.h"
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>

#include "Renderer.h"
#include "Texture2D.h"
#include "Font.h"

#include <vector>
#include <map>
#include <memory>

#include "NapoleonEngine.h"
#include "SceneManager.h"

class ResourceManager::ResourceManagerImpl final
{
public:
	ResourceManagerImpl();
	ResourceManagerImpl(ResourceManagerImpl const& other) = delete;
	ResourceManagerImpl(ResourceManagerImpl&& other) = delete;
	ResourceManagerImpl& operator=(ResourceManagerImpl const& rhs) = delete;
	ResourceManagerImpl& operator=(ResourceManagerImpl&& rhs) = delete;
	~ResourceManagerImpl();

	void Init(const std::string& data);

	std::shared_ptr<Texture2D> GetTexture(const std::string& file);
	std::shared_ptr<Font> GetFont(const std::string& file, unsigned int size);
	ID GetEffect(const std::string& file);
	std::shared_ptr<SoundEffect> GetEffectById(ID id) const;

private:
	std::string m_DataPath;

	std::map<std::string, std::shared_ptr<Texture2D>> m_pTextures;
	std::vector<std::shared_ptr<Font>> m_pFonts;
	std::map<std::string, std::shared_ptr<SoundEffect>> m_pEffectsStr;
	std::map<ID, std::shared_ptr<SoundEffect>> m_pEffects;

	std::shared_ptr<Texture2D> LoadTexture(const std::string& file);
	std::shared_ptr<Font>  LoadFont(const std::string& file, unsigned int size);
	std::shared_ptr<SoundEffect> LoadEffect(const std::string& file);
};

ResourceManager::ResourceManagerImpl::ResourceManagerImpl()
	:m_DataPath(),
	m_pTextures(),
	m_pFonts()
{
}


ResourceManager::ResourceManagerImpl::~ResourceManagerImpl()
{
	m_pTextures.clear();
	m_pFonts.clear();

	IMG_Quit();
}

void ResourceManager::ResourceManagerImpl::Init(const std::string& dataPath)
{
	m_DataPath = dataPath;

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

std::shared_ptr<Texture2D> ResourceManager::ResourceManagerImpl::LoadTexture(const std::string& file)
{
	const auto fullPath = m_DataPath + file;
	auto const texture = IMG_LoadTexture(Renderer::GetInstance().GetSDLRenderer(), fullPath.c_str());
	if (texture == nullptr)
	{
		throw std::runtime_error(std::string("Failed to load texture: ") + SDL_GetError());
	}
	return m_pTextures.insert(std::make_pair(file, std::make_shared<Texture2D>( texture ))).first->second;
}

std::shared_ptr<Texture2D> ResourceManager::ResourceManagerImpl::GetTexture(const std::string& file)
{
	if (m_pTextures.find(file) == m_pTextures.end())
	{
		try
		{
			return LoadTexture(file);

		}
		catch (std::runtime_error const& error)
		{
			Debugger::GetInstance().LogError(error.what());
		}
	}

	return m_pTextures.at(file);
}

std::shared_ptr<Font> ResourceManager::ResourceManagerImpl::GetFont(const std::string& file, unsigned int size)
{
	auto fontIt = std::find_if(m_pFonts.begin(), m_pFonts.end(), [&file, &size](std::shared_ptr<Font> pFont)
		{
			return pFont->GetFilePath() == file && pFont->GetSize() == size;
		});
	if (fontIt != m_pFonts.end())
	{
		return *fontIt;
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
			Debugger::GetInstance().LogError(error.what());
		}
	}

	return m_pEffectsStr.at(file)->GetId();
}

std::shared_ptr<SoundEffect> ResourceManager::ResourceManagerImpl::GetEffectById(ID id) const
{
	auto it = m_pEffects.find(id);

	if (it != m_pEffects.end())
	{
		return it->second;
	}
	return nullptr;
}

std::shared_ptr<Font> ResourceManager::ResourceManagerImpl::LoadFont(const std::string& file, unsigned int size)
{
	m_pFonts.push_back(std::make_shared<Font>( m_DataPath + file, size ));
	return m_pFonts.back();
}

std::shared_ptr<SoundEffect> ResourceManager::ResourceManagerImpl::LoadEffect(const std::string& file)
{
	auto pEffect = std::make_shared<SoundEffect>(m_DataPath + file);
	m_pEffects.insert(std::make_pair(pEffect->GetId(), pEffect));
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

void ResourceManager::Init(const std::string& dataPath)
{
	m_pImpl->Init(dataPath);
}

std::shared_ptr<Texture2D> ResourceManager::GetTexture(const std::string& file)
{
	return m_pImpl->GetTexture(file);
}

std::shared_ptr<Font> ResourceManager::GetFont(const std::string& file, unsigned int size)
{
	return m_pImpl->GetFont(file, size);
}

ID ResourceManager::GetEffect(const std::string& file)
{
	return m_pImpl->GetEffect(file);
}

std::shared_ptr<SoundEffect> ResourceManager::GetEffectById(ID id) const
{
	return m_pImpl->GetEffectById(id);
}
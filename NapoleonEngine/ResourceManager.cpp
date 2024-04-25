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

	Texture2D* const GetTexture(const std::string& file);
	Font* const GetFont(const std::string& file, unsigned int size);

private:
	std::string m_DataPath;

	std::map<std::string, Texture2D*> m_pTextures;
	std::vector<Font*> m_pFonts;

	Texture2D* const LoadTexture(const std::string& file);
	Font* const LoadFont(const std::string& file, unsigned int size);
};

ResourceManager::ResourceManagerImpl::ResourceManagerImpl()
	:m_DataPath(),
	m_pTextures(),
	m_pFonts()
{
	
}


ResourceManager::ResourceManagerImpl::~ResourceManagerImpl()
{
	for (auto pText : m_pTextures)
	{
		delete pText.second;
	}
	for (auto pFont : m_pFonts)
	{
		delete pFont;
	}

	m_pTextures.clear();
	m_pFonts.clear();
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

Texture2D* const ResourceManager::ResourceManagerImpl::LoadTexture(const std::string& file)
{
	const auto fullPath = m_DataPath + file;
	auto const texture = IMG_LoadTexture(Renderer::GetInstance().GetSDLRenderer(), fullPath.c_str());
	if (texture == nullptr)
	{
		throw std::runtime_error(std::string("Failed to load texture: ") + SDL_GetError());
	}
	m_pTextures.insert(std::make_pair(file, new Texture2D{ texture }));
	return m_pTextures.at(file);
}

Texture2D* const ResourceManager::ResourceManagerImpl::GetTexture(const std::string& file)
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

Font* const ResourceManager::ResourceManagerImpl::GetFont(const std::string& file, unsigned int size)
{
	auto fontIt = std::find_if(m_pFonts.begin(), m_pFonts.end(), [&file, &size](Font* pFont)
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

Font* const ResourceManager::ResourceManagerImpl::LoadFont(const std::string& file, unsigned int size)
{
	m_pFonts.push_back(new Font{ m_DataPath + file, size });
	return m_pFonts.back();
}

ResourceManager::ResourceManager()
	:Singleton<ResourceManager>(),
	m_pImpl(new ResourceManagerImpl{})
{
}

ResourceManager::~ResourceManager()
{

}

void ResourceManager::Init(const std::string& dataPath)
{
	m_pImpl->Init(dataPath);
}

Texture2D* const ResourceManager::GetTexture(const std::string& file)
{
	return m_pImpl->GetTexture(file);
}

Font* const ResourceManager::GetFont(const std::string& file, unsigned int size)
{
	return m_pImpl->GetFont(file, size);
}
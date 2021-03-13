#include "PCH.h"
#include "ResourceManager.h"
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>

#include "Renderer.h"
#include "Texture2D.h"
#include "Font.h"

using namespace empire;

ResourceManager::~ResourceManager()
{
	for (auto pText : m_pTextures)
	{
		delete pText;
	}
	for (auto pFont : m_pFonts)
	{
		delete pFont;
	}

	m_pTextures.clear();
	m_pFonts.clear();
}

void ResourceManager::Init(const std::string& dataPath)
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

Texture2D* const ResourceManager::LoadTexture(const std::string& file)
{
	const auto fullPath = m_DataPath + file;
	auto const texture = IMG_LoadTexture(Renderer::GetInstance().GetSDLRenderer(), fullPath.c_str());
	if (texture == nullptr) 
	{
		throw std::runtime_error(std::string("Failed to load texture: ") + SDL_GetError());
	}
	m_pTextures.push_back(new Texture2D{ texture });
	return m_pTextures.back();
}

Font* const empire::ResourceManager::LoadFont(const std::string& file, unsigned int size)
{
	m_pFonts.push_back(new Font{ m_DataPath + file, size });
	return m_pFonts.back();
}

#include "PCH.h"
#include <SDL_ttf.h>
#include "Font.h"



Font::Font(const std::string& fullPath, int size)
	: m_pFont(nullptr),
	m_Size(size),
	m_FilePath(fullPath)
{
	m_pFont = TTF_OpenFont(fullPath.c_str(), size);
	if (m_pFont == nullptr) 
	{
		LOG_ERROR("Failed to load font: %s", SDL_GetError());
	}
}

Font::~Font()
{
	if (IS_VALID(m_pFont))
	{
		TTF_CloseFont(m_pFont);
	}
}

TTF_Font* Font::GetFont() 
{
	LoadFont();
	return m_pFont;
}

void Font::Serialize(StreamWriter& writer) const
{
	writer.WriteString("fontName", m_FilePath);
	writer.WriteInt("fontSize", m_Size);
}

void Font::Deserialize(JsonReader* const reader)
{
	std::string fontName;
	reader->ReadString("fontName", m_FilePath);

	reader->ReadInt("fontSize", m_Size);
}

void Font::LoadFont()
{
	if (IS_VALID(m_pFont))
	{
		return;
	}

	m_pFont = TTF_OpenFont(m_FilePath.c_str(), m_Size);
	if (m_pFont == nullptr)
	{
		LOG_ERROR("Failed to load font: %s", SDL_GetError());
	}
}
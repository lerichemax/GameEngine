#include "PCH.h"
#include <SDL_ttf.h>
#include "Font.h"



Font::Font(const std::string& fullPath, int size)
	: m_Font(nullptr),
	m_Size(size),
	m_FilePath(fullPath)
{
	m_Font = TTF_OpenFont(fullPath.c_str(), size);
	if (m_Font == nullptr) 
	{
		LOG_ERROR("Failed to load font: %s", SDL_GetError());
	}
}

Font::~Font()
{
	TTF_CloseFont(m_Font);
}

TTF_Font* Font::GetFont() const {
	return m_Font;
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
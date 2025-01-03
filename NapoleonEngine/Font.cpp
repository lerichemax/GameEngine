#include "PCH.h"
#include <SDL_ttf.h>
#include "Font.h"



Font::Font(_TTF_Font* pFont, const std::string& fullPath, int size) //gain ownership of Font : make explicit
	: m_pFont(pFont),
	m_Size(size),
	m_FilePath(fullPath)
{
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

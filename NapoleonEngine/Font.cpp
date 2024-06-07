#include "PCH.h"
#include <SDL_ttf.h>
#include "Font.h"



Font::Font(const std::string& fullPath, unsigned int size)
	: m_Font(nullptr),
	m_Size(size),
	m_FilePath(fullPath)
{
	m_Font = TTF_OpenFont(fullPath.c_str(), size);
	if (m_Font == nullptr) 
	{
		Debugger::GetInstance().LogError(std::string("Failed to load font: ") + SDL_GetError());
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

void Font::Deserialize(JsonReader const* reader)
{
	std::string fontName;
	reader->ReadString("fontName", m_FilePath);

	int size;
	reader->ReadInt("fontSize", size);
	m_Size = size;
}
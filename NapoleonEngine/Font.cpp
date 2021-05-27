#include "PCH.h"
#include <SDL_ttf.h>
#include "Font.h"

TTF_Font* empire::Font::GetFont() const {
	return m_Font;
}

empire::Font::Font(const std::string& fullPath, unsigned int size)
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

empire::Font::~Font()
{
	TTF_CloseFont(m_Font);
}

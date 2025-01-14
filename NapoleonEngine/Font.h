#pragma once
#include "Serializer.h"

struct _TTF_Font;

/**
	* Simple RAII wrapper for an _TTF_Font
	*/
class Font
{
public:
	explicit Font(_TTF_Font* pFont, const std::string& fullPath, int size);
	~Font();

	Font(const Font &) = delete;
	Font(Font &&) = delete;
	Font & operator= (const Font &) = delete;
	Font & operator= (const Font &&) = delete;

	_TTF_Font* GetFont();
	std::string GetFilePath() const { return m_FilePath; }
	int GetSize() const { return m_Size; }

private:
	_TTF_Font* m_pFont{ nullptr };

	PROPERTY(int, m_Size);

	PROPERTY(std::string, m_FilePath);
};

SERIALIZE_CLASS(Font);
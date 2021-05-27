#pragma once

struct _TTF_Font;
namespace empire
{
	/**
	 * Simple RAII wrapper for an _TTF_Font
	 */
	class Font
	{
	public:
		_TTF_Font* GetFont() const;
		explicit Font(const std::string& fullPath, unsigned int size);
		explicit Font();
		~Font();

		Font(const Font &) = delete;
		Font(Font &&) = delete;
		Font & operator= (const Font &) = delete;
		Font & operator= (const Font &&) = delete;

		std::string GetFilePath() const { return m_FilePath; }
		unsigned int GetSize() const { return m_Size; }
	private:
		_TTF_Font* m_Font;
		unsigned int m_Size;

		std::string m_FilePath;
	};
}

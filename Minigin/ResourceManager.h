#pragma once
#include "Singleton.h"

namespace dae
{
	class Texture2D;
	class Font;
	class ResourceManager final : public Singleton<ResourceManager>
	{
	public:
		~ResourceManager();
		void Init(const std::string& data);
		Texture2D* const LoadTexture(const std::string& file);
		Font* const LoadFont(const std::string& file, unsigned int size);
	private:
		friend class Singleton<ResourceManager>;
		ResourceManager() = default;
		std::string m_DataPath;

		std::vector<Texture2D*> m_pTextures;
		std::vector<Font*> m_pFonts;
	};
}

#include "PCH.h"
#include "Renderer.h"

#include "imgui.h"

#include "backends/imgui_impl_sdl.h"
#include "backends/imgui_impl_opengl2.h"

#include <SDL.h>

#include "RendererComponent.h"
#include "SceneManager.h"
#include "Texture2D.h"

SDL_Renderer* Renderer::m_pRenderer{};

Renderer::Renderer()
	: Singleton<Renderer>(),
	m_pWindow(nullptr),
	m_BackgroundColor(0, 0, 0, 0)
{
	
}

Renderer::~Renderer()
{
	Destroy();
}

void Renderer::Init(unsigned int width, unsigned int height, std::string const& name)
{
	CreateSDLWindow(width, height, name);

	m_pRenderer = SDL_CreateRenderer(m_pWindow, GetOpenGLDriverIndex(), SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	if (m_pRenderer == nullptr) 
	{
		throw std::runtime_error(std::string("SDL_CreateRenderer Error: ") + SDL_GetError());
	}
	SDL_SetRenderDrawBlendMode(m_pRenderer, SDL_BLENDMODE_BLEND);
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGui_ImplSDL2_InitForOpenGL(m_pWindow, SDL_GL_GetCurrentContext());
	ImGui_ImplOpenGL2_Init();
}

void Renderer::Render()
{
	SDL_SetRenderDrawColor(m_pRenderer, m_BackgroundColor.R, m_BackgroundColor.G, m_BackgroundColor.B, m_BackgroundColor.A);
	SDL_RenderClear(m_pRenderer);

	SceneManager::GetInstance().Render();

	Debugger::GetInstance().Render();

	SDL_RenderPresent(m_pRenderer);
}

void Renderer::Destroy()
{
	ImGui_ImplOpenGL2_Shutdown();
	ImGui_ImplSDL2_Shutdown();
	ImGui::DestroyContext();
	
	if (m_pRenderer != nullptr)
	{
		SDL_DestroyRenderer(m_pRenderer);
		m_pRenderer = nullptr;
	}

	SDL_DestroyWindow(m_pWindow);
	m_pWindow = nullptr;

}

void Renderer::RenderTexture(const Texture2D& texture, const float x, const float y) const
{
	SDL_Rect dst;
	dst.x = static_cast<int>(x);
	dst.y = static_cast<int>(y);
	SDL_QueryTexture(texture.GetSDLTexture(), nullptr, nullptr, &dst.w, &dst.h);
	SDL_RenderCopy(GetSDLRenderer(), texture.GetSDLTexture(), nullptr, &dst);
}

void Renderer::RenderTexture(const Texture2D& texture, const float x, const float y, const float width, const float height) const
{
	SDL_Rect dst;
	dst.x = static_cast<int>(x);
	dst.y = static_cast<int>(y);
	dst.w = static_cast<int>(width);
	dst.h = static_cast<int>(height);
	SDL_RenderCopy(GetSDLRenderer(), texture.GetSDLTexture(), nullptr, &dst);
}

void Renderer::RenderTexture(const Texture2D& texture, TransformComponent const& pTransform) const
{
	glPushMatrix();
	{
		glTranslatef(pTransform.m_WorldLocation.x, pTransform.m_WorldLocation.y, 0);
		glRotatef(pTransform.m_WorldRotation, 0, 0, 1);
		glScalef(pTransform.m_WorldScale.x, pTransform.m_WorldScale.y, 0);
		glTranslatef(-pTransform.m_WorldLocation.x, -pTransform.m_WorldLocation.y, 0);

		RenderTexture(texture, pTransform.m_WorldLocation.x, pTransform.m_WorldLocation.y);
	}
	glPopMatrix();
}

void Renderer::RenderShape(geo::Shape const& pShape) const 
{
	pShape.Draw(m_pRenderer);
}

int Renderer::GetOpenGLDriverIndex()
{
	int oglIdx = -1;
	int nRD = SDL_GetNumRenderDrivers();
	for (int i = 0; i < nRD; i++)
	{
		SDL_RendererInfo info;
		if (!SDL_GetRenderDriverInfo(i, &info))
		{
			if (!strcmp(info.name, "opengl"))
			{
				oglIdx = i;
			}
		}
	}
	return oglIdx;
}

void Renderer::CreateSDLWindow(unsigned int width, unsigned int height, std::string const& name)
{
	m_WindowWidth = width;
	m_WindowHeight = height;
	m_pWindow = SDL_CreateWindow(
		name.c_str(),
		SDL_WINDOWPOS_UNDEFINED,
		SDL_WINDOWPOS_UNDEFINED,
		m_WindowWidth,
		m_WindowHeight,
		SDL_WINDOW_OPENGL
	);

	if (m_pWindow == nullptr)
	{
		throw std::runtime_error(std::string("SDL_CreateWindow Error: ") + SDL_GetError());
	}
}
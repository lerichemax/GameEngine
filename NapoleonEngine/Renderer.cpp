#include "PCH.h"
#include "Renderer.h"

#include "imgui.h"

#include "backends/imgui_impl_sdl.h"
#include "backends/imgui_impl_opengl2.h"

#include <SDL.h>
#include <algorithm>

#include "RendererComponent.h"
#include "SceneManager.h"
#include "Texture2D.h"

SDL_Renderer* Renderer::m_pRenderer{};

Renderer::Renderer(unsigned int Width, unsigned int Height, std::string const& name)
	: m_pWindow(nullptr)
{
	CreateSDLWindow(Width, Height, name);

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

Renderer::~Renderer()
{
	Destroy();
}

void Renderer::Render(Registry* const pRegistry, Color const& backgroundColor)
{
	SDL_SetRenderDrawColor(m_pRenderer, backgroundColor.R, backgroundColor.G, backgroundColor.B, backgroundColor.A);
	SDL_RenderClear(m_pRenderer);

	auto entities = pRegistry->GetEntities<RendererComponent, TransformComponent>();

	std::sort(entities.begin(), entities.end(), [pRegistry](Entity a, Entity b) {
		return pRegistry->GetComponent<RendererComponent>(a)->Layer < pRegistry->GetComponent<RendererComponent>(b)->Layer;
	});

	for (Entity entity : entities)
	{
		auto* const renderComp = pRegistry->GetComponent<RendererComponent>(entity);

		if (!renderComp->IsActive())
		{
			continue;
		}

		auto* const pTransform = pRegistry->GetComponent<TransformComponent>(entity);

		if (renderComp->pTexture != nullptr)
		{
			RenderTexture(*renderComp->pTexture, *pTransform);
		}

		if (renderComp->pShape != nullptr)
		{
			RenderShape(*renderComp->pShape, pTransform->GetLocation());
		}
	}


	Debugger::Get().Render(this);

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

void Renderer::RenderTexture(Texture2D& texture, const float x, const float y) const
{
	SDL_Rect dst;
	dst.x = static_cast<int>(x);
	dst.y = static_cast<int>(y);
	SDL_QueryTexture(texture.GetSDLTexture(), nullptr, nullptr, &dst.w, &dst.h);
	SDL_RenderCopy(GetSDLRenderer(), texture.GetSDLTexture(), nullptr, &dst);
}

void Renderer::RenderTexture(Texture2D& texture, const float x, const float y, const float Width, const float Height) const
{
	SDL_Rect dst;
	dst.x = static_cast<int>(x);
	dst.y = static_cast<int>(y);
	dst.w = static_cast<int>(Width);
	dst.h = static_cast<int>(Height);
	SDL_RenderCopy(GetSDLRenderer(), texture.GetSDLTexture(), nullptr, &dst);
}

void Renderer::RenderTexture(Texture2D& texture, TransformComponent& pTransform) const
{
	glPushMatrix();
	{
		glm::vec2 translation = pTransform.GetLocation();
		float rotation = pTransform.GetRotation();
		glm::vec2 scale = pTransform.GetScale();

		glTranslatef(translation.x, translation.y, 0);
		glRotatef(rotation, 0, 0, 1);
		glScalef(scale.x, scale.y, 0);
		glTranslatef(-translation.x, -translation.y, 0);

		RenderTexture(texture, translation.x, translation.y);
	}
	glPopMatrix();
}

void Renderer::RenderShape(geo::Shape const& pShape, glm::vec2 const& loc) const
{
	pShape.Draw(m_pRenderer, loc);
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

void Renderer::CreateSDLWindow(unsigned int Width, unsigned int Height, std::string const& name)
{
	m_WindowWidth = Width;
	m_WindowHeight = Height;
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
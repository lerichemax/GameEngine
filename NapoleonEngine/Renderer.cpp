#include "PCH.h"
#include "Renderer.h"

#include "imgui.h"

#include "backends/imgui_impl_sdl.h"
#include "backends/imgui_impl_opengl2.h"

#include <SDL.h>
#include <algorithm>
#include <glm/gtx/matrix_decompose.hpp>

#include "TransformSystem.h"
#include "RendererComponent.h"
#include "SceneManager.h"
#include "Texture2D.h"
#include "Camera.h"

void SDLDestroyer::operator()(SDL_Window* pWindow) const
{
	SDL_DestroyWindow(pWindow);
}

void SDLDestroyer::operator()(SDL_Renderer* pRenderer) const
{
	SDL_DestroyRenderer(pRenderer);
}

Renderer::Renderer(unsigned int Width, unsigned int Height, std::string const& name)
	: m_pWindow(nullptr)
{
	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) < 0)
	{
		throw std::runtime_error(std::string("SDL_Init Error: ") + SDL_GetError());
	}

	CreateSDLWindow(Width, Height, name);

	m_pRenderer = std::unique_ptr<SDL_Renderer, SDLDestroyer>(SDL_CreateRenderer(m_pWindow.get(), GetOpenGLDriverIndex(), SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC));
	if (m_pRenderer == nullptr)
	{
		throw std::runtime_error(std::string("SDL_CreateRenderer Error: ") + SDL_GetError());
	}
	SDL_SetRenderDrawBlendMode(m_pRenderer.get(), SDL_BLENDMODE_BLEND);

	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGui_ImplSDL2_InitForOpenGL(m_pWindow.get(), SDL_GL_GetCurrentContext());
	ImGui_ImplOpenGL2_Init();
}

Renderer::~Renderer()
{
	Destroy();
}

void Renderer::Render(Registry* const pRegistry, Color const& backgroundColor, Camera2D const* pCamera)
{
	SDL_SetRenderDrawColor(m_pRenderer.get(), backgroundColor.R, backgroundColor.G, backgroundColor.B, backgroundColor.A);
	SDL_RenderClear(m_pRenderer.get());

	auto entities = pRegistry->GetEntities<RendererComponent, TransformComponent>();

	std::sort(entities.begin(), entities.end(), [pRegistry](Entity a, Entity b) {
		return pRegistry->GetComponent<RendererComponent>(a)->Layer < pRegistry->GetComponent<RendererComponent>(b)->Layer;
	});

	for (Entity entity : entities)
	{
		auto* const pRenderComp = pRegistry->GetComponent<RendererComponent>(entity);

		if (!pRenderComp->IsActive())
		{
			continue;
		}

		auto* const pTransform = pRegistry->GetComponent<TransformComponent>(entity);
		

		switch (pRenderComp->Space)
		{
		case RenderSpace::World:
		{
			auto cameraSpaceTransform = pCamera->TransformToCameraSpace(pTransform->GetWorldTransformMatrix());
			if (IS_VALID(pRenderComp->pTexture))
			{
				RenderTexture(*pRenderComp->pTexture, cameraSpaceTransform);
			}

			if (IS_VALID(pRenderComp->pShape))
			{
				RenderShape(*pRenderComp->pShape, cameraSpaceTransform[2]);
			}
		}
		break;
		case RenderSpace::Screen:
			if (IS_VALID(pRenderComp->pTexture))
			{
				RenderTexture(*pRenderComp->pTexture, pTransform->GetWorldTransformMatrix());
			}

			if (IS_VALID(pRenderComp->pShape))
			{
				RenderShape(*pRenderComp->pShape, pTransform->GetLocation());
			}
			break;
		}

	}


	Debugger::Get().Render(this); //screen space

	SDL_RenderPresent(m_pRenderer.get());
}

void Renderer::Destroy()
{
	ImGui_ImplOpenGL2_Shutdown();
	ImGui_ImplSDL2_Shutdown();
	ImGui::DestroyContext();

}

void Renderer::RenderTexture(Texture2D& texture, const float x, const float y) const
{
	SDL_Rect dst;
	dst.x = static_cast<int>(x);
	dst.y = static_cast<int>(y);
	SDL_QueryTexture(texture.GetSDLTexture(), nullptr, nullptr, &dst.w, &dst.h);
	SDL_RenderCopy(m_pRenderer.get(), texture.GetSDLTexture(), nullptr, &dst);
}

void Renderer::RenderTexture(Texture2D& texture, const float x, const float y, const float Width, const float Height) const
{
	SDL_Rect dst;
	dst.x = static_cast<int>(x);
	dst.y = static_cast<int>(y);
	dst.w = static_cast<int>(Width);
	dst.h = static_cast<int>(Height);
	SDL_RenderCopy(m_pRenderer.get(), texture.GetSDLTexture(), nullptr, &dst);
}

void Renderer::RenderTexture(Texture2D& texture, glm::mat3 const& transform) const
{
	glm::vec2 translation;
	float rotation;
	glm::vec2 scale;

	TransformSystem::DecomposeMat3(transform, translation, rotation, scale);

	glPushMatrix();
	{
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
	pShape.Draw(m_pRenderer.get(), loc);
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
	m_pWindow = std::unique_ptr<SDL_Window, SDLDestroyer>(SDL_CreateWindow(
		name.c_str(),
		SDL_WINDOWPOS_UNDEFINED,
		SDL_WINDOWPOS_UNDEFINED,
		m_WindowWidth,
		m_WindowHeight,
		SDL_WINDOW_OPENGL
	));

	if (m_pWindow == nullptr)
	{
		throw std::runtime_error(std::string("SDL_CreateWindow Error: ") + SDL_GetError());
	}
}
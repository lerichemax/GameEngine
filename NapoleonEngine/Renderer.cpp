#include "PCH.h"
#include "Renderer.h"

#include "imgui.h"

#include "backends/imgui_impl_sdl.h"
#include "backends/imgui_impl_opengl2.h"

#include <SDL.h>

#include "RendererComponent.h"
#include "SceneManager.h"
#include "Texture2D.h"

using namespace empire;

SDL_Renderer* Renderer::m_pRenderer{};

Renderer::Renderer()
	: Singleton<Renderer>(),
	m_BackgroundColor(0, 0, 0, 0)
{
	
}
void Renderer::Init(SDL_Window * window)
{
	m_pWindow = window;
	m_pRenderer = SDL_CreateRenderer(window, GetOpenGLDriverIndex(), SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	if (m_pRenderer == nullptr) 
	{
		throw std::runtime_error(std::string("SDL_CreateRenderer Error: ") + SDL_GetError());
	}
	SDL_SetRenderDrawBlendMode(m_pRenderer, SDL_BLENDMODE_BLEND);
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGui_ImplSDL2_InitForOpenGL(window, SDL_GL_GetCurrentContext());
	ImGui_ImplOpenGL2_Init();
}

void empire::Renderer::Render()
{
	SDL_SetRenderDrawColor(m_pRenderer, m_BackgroundColor.R, m_BackgroundColor.G, m_BackgroundColor.B, m_BackgroundColor.A);
	SDL_RenderClear(m_pRenderer);
	
	//ImGui_ImplOpenGL2_NewFrame();
	//ImGui_ImplSDL2_NewFrame(m_pWindow);
	//ImGui::NewFrame();

	SceneManager::GetInstance().Render();

	Debugger::GetInstance().Render();
	//ImGui::Begin("Viewport");
	//
	//ImGui::End();
	//
	//ImGui::Begin("Test");
	//ImGui::Button("Single Player");
	//ImGui::Button("Co-op");
	//ImGui::Button("Versus");
	//ImGui::End();
	//ImGui::Render();
	//ImGui_ImplOpenGL2_RenderDrawData(ImGui::GetDrawData());

	SDL_RenderPresent(m_pRenderer);
}

void empire::Renderer::Destroy()
{
	ImGui_ImplOpenGL2_Shutdown();
	ImGui_ImplSDL2_Shutdown();
	ImGui::DestroyContext();
	
	if (m_pRenderer != nullptr)
	{
		SDL_DestroyRenderer(m_pRenderer);
		m_pRenderer = nullptr;
	}
}

void empire::Renderer::RenderTexture(const Texture2D& texture, const float x, const float y) const
{
	SDL_Rect dst;
	dst.x = static_cast<int>(x);
	dst.y = static_cast<int>(y);
	SDL_QueryTexture(texture.GetSDLTexture(), nullptr, nullptr, &dst.w, &dst.h);
	SDL_RenderCopy(GetSDLRenderer(), texture.GetSDLTexture(), nullptr, &dst);
}

void empire::Renderer::RenderTexture(const Texture2D& texture, const float x, const float y, const float width, const float height) const
{
	SDL_Rect dst;
	dst.x = static_cast<int>(x);
	dst.y = static_cast<int>(y);
	dst.w = static_cast<int>(width);
	dst.h = static_cast<int>(height);
	SDL_RenderCopy(GetSDLRenderer(), texture.GetSDLTexture(), nullptr, &dst);
}

void empire::Renderer::RenderTexture(const Texture2D& texture, TransformComponent const& pTransform) const
{
	glPushMatrix();
	{
		glTranslatef(pTransform.GetPosition().x , pTransform.GetPosition().y , 0);
		glRotatef(pTransform.GetRotation(), 0, 0, 1);
		glScalef(pTransform.GetScale().x, pTransform.GetScale().y, 0);
		glTranslatef(-pTransform.GetPosition().x , -pTransform.GetPosition().y , 0);
		
		RenderTexture(texture, pTransform.GetPosition().x, pTransform.GetPosition().y);
	}
	glPopMatrix();
}

void Renderer::RenderShape(Shape const& pShape) const
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
#include "PCH.h"
#include "ShapeRenderer.h"


ShapeRenderer::ShapeRenderer(Shape* pShape)
	:RendererComponent(Layer::uiMenuBg),
	m_pShape(pShape)
{
}

ShapeRenderer::ShapeRenderer(ShapeRenderer const& other)
	:RendererComponent(other),
	m_pShape(other.m_pShape->Clone())
{
	
}

ShapeRenderer::~ShapeRenderer()
{
	delete m_pShape;
}

void ShapeRenderer::Render(TransformComponent const&) const
{
	Renderer::GetInstance().RenderShape(*m_pShape);
}
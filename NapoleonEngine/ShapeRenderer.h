#pragma once
#include "RendererComponent.h"

namespace empire
{
	struct Shape;
	class ShapeRenderer final : public RendererComponent
	{
	public:
		ShapeRenderer(Shape* pShape);
		ShapeRenderer* Clone() { return new ShapeRenderer(*this); }

		ShapeRenderer(ShapeRenderer&& other) = delete;
		ShapeRenderer& operator=(ShapeRenderer const& rhs) = delete;
		ShapeRenderer& operator=(ShapeRenderer&& rhs) = delete;
		~ShapeRenderer();

		void Render(TransformComponent const& transform) const override;
	
	private:
		Shape* m_pShape;

		
		ShapeRenderer(ShapeRenderer const& other);
	};
}
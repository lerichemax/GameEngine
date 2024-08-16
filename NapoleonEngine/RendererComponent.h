#pragma once
#include "Component.h"
#include "GameObject.h"
#include "Texture2D.h"
#include "TransformComponent.h"
#include "SceneRenderer.h"

#include <memory>

struct RendererComponent : public ECS_Component
{
	std::shared_ptr<Texture2D> m_pTexture{};
	uint8_t m_Layer{};

	void Serialize(StreamWriter& writer) const override;
	void Deserialize(JsonReader const* reader, SerializationMap& context) override;

	void RestoreContext(JsonReader const* reader, SerializationMap const& context) override {}
};

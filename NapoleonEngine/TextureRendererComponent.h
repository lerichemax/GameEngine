#pragma once
#include "RendererComponent.h"

class TextureRendererComponent final : public RendererComponent
{
public:
	Texture2D* m_pTexture{ nullptr };

	void Serialize(StreamWriter& writer) const override;
	void Deserialize(JsonReader const* reader, SerializationMap& context) override;

	void Render() override;
};
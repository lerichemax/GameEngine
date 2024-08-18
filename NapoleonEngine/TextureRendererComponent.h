#pragma once
#include "RendererComponent.h"

class TextureRendererComponent final : public RendererComponent
{
public:
	std::shared_ptr<Texture2D> m_pTexture{};

	void Serialize(StreamWriter& writer) const override;
	void Deserialize(JsonReader const* reader, SerializationMap& context) override;

protected:
	void Render() override;
};
#pragma once
#include "Component.h"

#include "Qube.h"

struct MovementComponent : public Component
{
	bool m_CanMove{ true };

	QubeSystem* m_pCurrentQube{ nullptr };
	//TextureRendererComponent* m_pRenderer{ nullptr };
	//Jumper* m_pJumper{ nullptr };

	std::string m_TextureIdleDownRight;
	std::string m_TextureIdleDownLeft;
	std::string m_TextureIdleUpRight;
	std::string m_TextureIdleUpLeft;

	std::string m_TextureJumpDownRight;
	std::string m_TextureJumpDownLeft;
	std::string m_TextureJumpUpRight;
	std::string m_TextureJumpUpLeft;

	ConnectionDirection m_CurentDirection{ ConnectionDirection::null};

	void SetTextureIdleNames(std::string const& downRight, std::string const& downLeft, std::string const& upRight, std::string const& upLeft);
	void SetTextureJumpNames(std::string const& downRight, std::string const& downLeft, std::string const& upRight, std::string const& upLeft);

	void Serialize(StreamWriter& writer) const override;
	void Deserialize(JsonReader const* reader, SerializationMap& context) override;
};
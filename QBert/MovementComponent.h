#pragma once
#include "Component.h"

enum class MovementMode
{
	Normal,
	EscheresqueRight,
	EscheresqueLeft
};

enum class ConnectionDirection;
struct MovementComponent : public Component
{
	MovementComponent();

	bool bCanMove{ true };

	Entity CurrentQube;

	std::string TextureIdleDownRight;
	std::string TextureIdleDownLeft;
	std::string TextureIdleUpRight;
	std::string TextureIdleUpLeft;

	std::string TextureJumpDownRight;
	std::string TextureJumpDownLeft;
	std::string TextureJumpUpRight;
	std::string TextureJumpUpLeft;

	ConnectionDirection CurrentDirection;
	MovementMode Mode;

	void SetTextureIdleNames(std::string const& downRight, std::string const& downLeft, std::string const& upRight, std::string const& upLeft);
	void SetTextureJumpNames(std::string const& downRight, std::string const& downLeft, std::string const& upRight, std::string const& upLeft);

	void Serialize(StreamWriter& writer) const override;
	void Deserialize(JsonReader const* reader, SerializationMap& context) override;
};
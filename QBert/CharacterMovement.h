#pragma once
#include "BehaviourComponent.h"

#include "Event.h"

class TextureRendererComponent;
class Jumper;
enum class ConnectionDirection;
class Qube;

class CharacterMovement : public BehaviourComponent
{
public:
	EventHandler<CharacterMovement> OnMoveStarted;

	void Move(ConnectionDirection direction);
	void SetCurrentQube(std::shared_ptr<Qube> pQube);
	std::shared_ptr<Qube> GetCurrentQube() const;

	void Serialize(StreamWriter& writer) const override;
	void Deserialize(JsonReader const* reader, SerializationMap& context) override;

	void SetTextureIdleNames(std::string const& downRight, std::string const& downLeft, std::string const& upRight, std::string const& upLeft);
	void SetTextureJumpNames(std::string const& downRight, std::string const& downLeft, std::string const& upRight, std::string const& upLeft);

protected:
	void Start();

private:
	bool m_CanMove{ true };

	std::shared_ptr<Qube> m_pCurrentQube{ nullptr };
	std::shared_ptr<TextureRendererComponent> m_pRenderer{ nullptr };
	std::shared_ptr<Jumper> m_pJumper{ nullptr };

	std::string m_TextureIdleDownRight;
	std::string m_TextureIdleDownLeft;
	std::string m_TextureIdleUpRight;
	std::string m_TextureIdleUpLeft;

	std::string m_TextureJumpDownRight;
	std::string m_TextureJumpDownLeft;
	std::string m_TextureJumpUpRight;
	std::string m_TextureJumpUpLeft;

	ConnectionDirection m_CurentDirection;

	void JumpToCurrentQube();
	void MoveToCurrentQube();
	void SetIdleTexture();
	void SetJumpTexture();
};

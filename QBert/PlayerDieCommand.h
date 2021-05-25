#pragma once
#include "Command.h"

namespace empire
{
	class GameObject;
}
class PlayerDieCommand final : public empire::Command
{
public:
	PlayerDieCommand(KeyActionState state, empire::GameObject* pPlayer);

	void Execute() override;

private:
	empire::GameObject* m_pPlayerCharacter;
};
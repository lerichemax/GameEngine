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

	bool Execute() override;

private:
	empire::GameObject* m_pPlayerCharacter;
};
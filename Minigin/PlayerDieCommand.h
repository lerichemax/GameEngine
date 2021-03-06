#pragma once
#include "Command.h"

namespace dae
{
	class GameObject;
}
class PlayerDieCommand final : public dae::Command
{
public:
	PlayerDieCommand(KeyActionState state, dae::GameObject* pPlayer);

	bool Execute() override;

private:
	dae::GameObject* m_pPlayerCharacter;
};
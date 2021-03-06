#pragma once
#include "Command.h"

namespace dae
{
	class GameObject;
}
class EarnPointsCommand final : public dae::Command
{
public:
	EarnPointsCommand(KeyActionState state, dae::GameObject* const pPlayer);

	bool Execute() override;

private:
	dae::GameObject* const m_pPlayerCharacter;
};

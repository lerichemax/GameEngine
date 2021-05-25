#pragma once
#include "Command.h"

namespace empire
{
	class GameObject;
}
class EarnPointsCommand final : public empire::Command
{
public:
	EarnPointsCommand(KeyActionState state, empire::GameObject* const pPlayer);

	void Execute() override;

private:
	empire::GameObject* const m_pPlayerCharacter;
};

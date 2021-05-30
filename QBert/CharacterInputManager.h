#pragma once
#include "Component.h"

class CharacterInputManager final : public empire::Component
{
public:
	CharacterInputManager();
	CharacterInputManager* Clone() { return new CharacterInputManager(*this); }
	CharacterInputManager(CharacterInputManager&& other) = delete;
	CharacterInputManager& operator=(CharacterInputManager const& other) = delete;
	CharacterInputManager& operator=(CharacterInputManager&& other) = delete;

	void SetInputActionsActive(bool isActive);
	void AddInputId(int id);

private:
	bool m_bIsReceivingInput;
	std::vector<int> m_InputActionIds;
	
	CharacterInputManager(CharacterInputManager const& other);

	void Update() override{};
	void Initialize() override{};
};
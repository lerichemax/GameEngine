#pragma once
#include "Component.h"

#include <map>
#include <string>

namespace empire
{
	class ButtonComponent;
}

class ButtonAccessor final : public empire::Component
{
public:
	ButtonAccessor();
	ButtonAccessor* Clone() const override { return new ButtonAccessor(*this); }
	
	ButtonAccessor(ButtonAccessor&& other) = delete;
	ButtonAccessor& operator=(ButtonAccessor const& rhs) = delete;
	ButtonAccessor& operator=(ButtonAccessor&& rhs) = delete;
	~ButtonAccessor() = default;

	void AddButton(std::string const& key, ButtonComponent* pBtn);
	void RemoveButton(std::string const& key);
	ButtonComponent* GetButton(std::string const& key) const;

private:
	std::map<std::string, ButtonComponent*> m_pButtons;

	ButtonAccessor(ButtonAccessor const& other);

	void Update() override {}
	void Initialize() override{}
	
};
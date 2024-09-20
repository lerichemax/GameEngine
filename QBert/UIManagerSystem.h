#pragma once
#include "System.h"

class TextRendererComponent;
class UIManagerSystem : public System
{
public:
	UIManagerSystem() = default;
	~UIManagerSystem() = default;

	void Serialize(StreamWriter& writer) const override;

	void SetSignature() const override;

protected:
	void Start() override;

};
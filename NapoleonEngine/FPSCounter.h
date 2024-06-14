#pragma once
#include "Component.h"

#include "System.h"

class TextRendererComponent;
class FPSCounter final : public Component
{
public:
	FPSCounter() = default;

protected:
	FPSCounter* Clone() const override { return new FPSCounter(*this); }
	void Initialize() override;
	void Update() override;
private:
	TextRendererComponent* m_pTextComp;
};

struct FPSCounterComponent : ECS_Component
{
	void Serialize(StreamWriter& writer) const override { writer.WriteString("type", typeid(FPSCounterComponent).name()); };
	void Deserialize(JsonReader const* reader, SerializationMap& context) override {};

	void RestoreContext(JsonReader const* reader, SerializationMap const& context) override {};

};

class FPSCounterSystem : public System
{
public:
	virtual void Update(ComponentManager* const pComponentManager) override;
	std::string GetNameForSerialization() const override { return  typeid(FPSCounterSystem).name(); };

	void SetSignature(Coordinator* const pRegistry) override;

protected:
	std::shared_ptr<System> Clone() const override { return std::make_shared<FPSCounterSystem>(*this); }
};
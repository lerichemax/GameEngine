#pragma once
#include "Serializer.h"

class Command : public IContextSerializable
{
public:
	Command() = default;

	Command(Command const& other) = delete;
	Command(Command&& other) = delete;
	Command& operator=(Command const& rhs) = delete;
	Command& operator=(Command&& rhs) = delete;
	virtual ~Command() = default;

	virtual void Execute(GameObject const& gObject) = 0;

	virtual void Serialize(StreamWriter& writer) const override;
	virtual void Deserialize(JsonReader const* reader, SerializationMap& context) override;
};
#pragma once
#include "{{BASE_CLASS_HEADER}}"

class {{CLASS_NAME}} final : public {{BASE_CLASS}} {

public:
	{{CLASS_NAME}}() = default;

	void SetSignature() const override;
	void Serialize(StreamWriter& writer) const override;

};
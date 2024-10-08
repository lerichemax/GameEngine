#include "PCH.h"
#include "SwitchTextColor.h"

#include "TextRendererComponent.h"

SwitchTextColor::SwitchTextColor(Color targetColor)
	:m_TargetColor(targetColor)
{
	
}

void SwitchTextColor::Execute(GameObject const& gObject)
{
	auto pText = gObject.GetComponent<TextRendererComponent>();
	if (pText != nullptr)
	{
		pText->SetTextColor(m_TargetColor.R, m_TargetColor.G, m_TargetColor.B);
	}
}

void SwitchTextColor::Serialize(StreamWriter& writer) const
{
	writer.WriteInt64("type", static_cast<int64_t>(std::type_index(typeid(SwitchTextColor)).hash_code()));
	writer.WriteObject("color", &m_TargetColor);

	Command::Serialize(writer);
}
void SwitchTextColor::Deserialize(JsonReader const* reader, SerializationMap& context)
{
	auto colorReader = reader->ReadObject("color");
	m_TargetColor.Deserialize(colorReader.get());

	Command::Deserialize(reader, context);
}
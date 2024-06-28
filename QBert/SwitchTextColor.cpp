#include "PCH.h"
#include "SwitchTextColor.h"

#include "TextRendererComponent.h"

SwitchTextColor::SwitchTextColor(Color targetColor, std::shared_ptr<ECS_TextRendererComponent> pText)
	:m_TargetColor(targetColor),
	m_pText(pText)
{
	
}

SwitchTextColor::SwitchTextColor(SwitchTextColor const& other)
	:m_TargetColor(other.m_TargetColor),
	m_pText(other.m_pText)
{
	
}

void SwitchTextColor::Execute()
{
	m_pText->SetTextColor(m_TargetColor.R, m_TargetColor.G, m_TargetColor.B);
}

void SwitchTextColor::Serialize(StreamWriter& writer) const
{
	writer.WriteString("type", typeid(SwitchTextColor).name());
	writer.StartObject("color");
	m_TargetColor.Serialize(writer);
	writer.EndObject();
	writer.WriteInt("textRenderer", m_pText->GetId());

	Command::Serialize(writer);
}
void SwitchTextColor::Deserialize(JsonReader const* reader, SerializationMap& context)
{
	auto colorReader = reader->ReadObject("color");
	m_TargetColor.Deserialize(colorReader.get());

	Command::Deserialize(reader, context);
}

void SwitchTextColor::RestoreContext(JsonReader const* reader, SerializationMap const& context)
{
	int textId = -1;
	reader->ReadInt("textRenderer", textId);
	m_pText = context.GetRef<ECS_TextRendererComponent>(textId);
}
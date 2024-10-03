#include "PCH.h"
#include "ButtonComponent.h"

#include "Command.h"
#include "InputManager.h"
#include "GameObject.h"
#include "TransformComponent.h"

ButtonComponent::ButtonComponent()
	:Component(true),
	m_Dimensions(100, 50), //by default
	m_bIsSelected(false),
	m_bVisualize(false),
	m_pOnClick(nullptr),
	m_pOnSelect(nullptr),
	m_pOnDeselect(nullptr)
{
}

void ButtonComponent::Serialize(StreamWriter& writer) const
{
	writer.WriteInt64("type", static_cast<int64_t>(std::type_index(typeid(ButtonComponent)).hash_code()));
	writer.WriteBool("visualize", m_bVisualize);
	writer.WriteVector("dimension", m_Dimensions);

	if (m_pOnClick != nullptr)
	{
		writer.WriteObject("onClick", m_pOnClick.get());
	}
	
	if (m_pOnSelect != nullptr)
	{
		writer.WriteObject("onSelect", m_pOnSelect.get());
	}

	if (m_pOnDeselect != nullptr)
	{
		writer.WriteObject("onDeselect", m_pOnDeselect.get());
	}
}

void ButtonComponent::Deserialize(JsonReader const* reader, SerializationMap& context)
{
	reader->ReadBool("visualize", m_bVisualize);
	auto dimensionObject = reader->ReadObject("dimension");
	dimensionObject->ReadDouble("x", m_Dimensions.x);
	dimensionObject->ReadDouble("y", m_Dimensions.y);

	auto onclickReader = reader->ReadObject("onClick");
	if (onclickReader != nullptr && onclickReader->IsValid())
	{
		int64_t type;
		onclickReader->ReadInt64("type", type);

		m_pOnClick = std::unique_ptr<Command>(std::move(Factory<Command>::Get().Create(static_cast<size_t>(type))));

		if (m_pOnClick != nullptr)
		{
			m_pOnClick->Deserialize(onclickReader.get(), context);
			context.Add(m_pOnClick->GetId(), m_pOnClick.get());
		}
	}
	
	auto onSelectReader = reader->ReadObject("onSelect");
	if (onSelectReader != nullptr && onSelectReader->IsValid())
	{
		int64_t type;
		onSelectReader->ReadInt64("type", type);

		m_pOnSelect = std::unique_ptr<Command>(std::move(Factory<Command>::Get().Create(static_cast<size_t>(type))));
		if (m_pOnSelect != nullptr)
		{
			m_pOnSelect->Deserialize(onSelectReader.get(), context);
			context.Add(m_pOnSelect->GetId(), m_pOnSelect.get());
		}
	}
	
	auto onDeselectReader = reader->ReadObject("onDeselect");
	if (onDeselectReader != nullptr && onDeselectReader->IsValid())
	{
		int64_t type;
		onDeselectReader->ReadInt64("type", type);

		m_pOnDeselect = std::unique_ptr<Command>(std::move(Factory<Command>::Get().Create(static_cast<size_t>(type))));
		if (m_pOnDeselect != nullptr)
		{
			m_pOnDeselect->Deserialize(onDeselectReader.get(), context);
			context.Add(m_pOnDeselect->GetId(), m_pOnDeselect.get());
		}
	}
	
	Component::Deserialize(reader, context);
}

void ButtonComponent::RestoreContext(JsonReader const* reader, SerializationMap const& context)
{
	std::string type;
	auto onclickReader = reader->ReadObject("onClick");
	if (onclickReader != nullptr && onclickReader->IsValid() && m_pOnClick != nullptr)
	{
		m_pOnClick->RestoreContext(onclickReader.get(), context);
	}

	auto onSelectReader = reader->ReadObject("onSelect");
	if (onSelectReader != nullptr && onSelectReader->IsValid() && m_pOnSelect != nullptr)
	{
		m_pOnSelect->RestoreContext(onSelectReader.get(), context);
	}

	auto onDeselectReader = reader->ReadObject("onDeselect");
	if (onDeselectReader != nullptr && onDeselectReader->IsValid() && m_pOnDeselect != nullptr)
	{
		m_pOnDeselect->RestoreContext(onDeselectReader.get(), context);
	}
}

void ButtonComponent::Select()
{
	m_bIsSelected = true;
	OnSelect.Notify();
}

void ButtonComponent::Deselect()
{
	m_bIsSelected = false;
	OnDeselect.Notify();
}

void ButtonComponent::Click()
{
	OnClick.Notify();
}

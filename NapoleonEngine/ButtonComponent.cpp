#include "PCH.h"
#include "ButtonComponent.h"

#include "Command.h"
#include "InputManager.h"
#include "GameObject.h"
#include "TransformComponent.h"

ButtonComponent::ButtonComponent(float width, float height)
	:Component(),
	m_Dimensions(width, height),
	m_IsSelected(false),
	m_bVisualize(false),
	m_pOnClick(nullptr),
	m_pOnSelect(nullptr),
	m_pOnDeselect(nullptr)
{
}

ButtonComponent::ButtonComponent(ButtonComponent const& other)
	:m_Dimensions(other.m_Dimensions),
	m_IsSelected(other.m_IsSelected),
	m_bVisualize(other.m_bVisualize)
{}

ButtonComponent::~ButtonComponent()
{
	SafeDelete(m_pOnClick);
	SafeDelete(m_pOnSelect);
	SafeDelete(m_pOnDeselect);
}

void ButtonComponent::Update()
{


}


void ButtonComponent::SetOnClickFunction(Command* func)
{
	m_pOnClick = func;
}

void ButtonComponent::SetOnSelectFunction(Command* func)
{
	m_pOnSelect = func;
}
void ButtonComponent::SetOnDeselectFunction(Command* func)
{
	m_pOnDeselect = func;
}

ECS_ButtonComponent::ECS_ButtonComponent()
	:ECS_Component(true),
	m_Dimensions(100, 50), //by default
	m_IsSelected(false),
	m_bVisualize(false),
	m_pOnClick(nullptr),
	m_pOnSelect(nullptr),
	m_pOnDeselect(nullptr)
{
}

void ECS_ButtonComponent::Serialize(StreamWriter& writer) const
{
	writer.WriteString("type", typeid(ECS_ButtonComponent).name());
	writer.WriteBool("visualize", m_bVisualize);
	writer.StartObject("dimension");
	{
		writer.WriteDouble("x", m_Dimensions.x);
		writer.WriteDouble("y", m_Dimensions.y);
	}
	writer.EndObject();

	if (m_pOnClick != nullptr)
	{
		writer.StartObject("onClick");
		{
			m_pOnClick->Serialize(writer);
		}
		writer.EndObject();
	}
	
	if (m_pOnSelect != nullptr)
	{
		writer.StartObject("onSelect");
		{
			m_pOnSelect->Serialize(writer);
		}
		writer.EndObject();
	}

	if (m_pOnDeselect != nullptr)
	{
		writer.StartObject("onDeselect");
		{
			m_pOnDeselect->Serialize(writer);
		}
		writer.EndObject();
	}
}

void ECS_ButtonComponent::Deserialize(JsonReader const* reader, SerializationMap& context)
{
	reader->ReadBool("visualize", m_bVisualize);
	auto dimensionObject = reader->ReadObject("dimension");
	dimensionObject->ReadDouble("x", m_Dimensions.x);
	dimensionObject->ReadDouble("y", m_Dimensions.y);

	std::string type;

	auto onclickReader = reader->ReadObject("onClick");
	if (onclickReader != nullptr && onclickReader->IsValid())
	{
		onclickReader->ReadString("type", type);
		m_pOnClick = CommandFactory::GetInstance().Create(type);

		if (m_pOnClick != nullptr)
		{
			m_pOnClick->Deserialize(onclickReader.get(), context);
			context.Add(m_pOnClick->GetId(), m_pOnClick);
		}
	}
	
	auto onSelectReader = reader->ReadObject("onSelect");
	if (onSelectReader != nullptr && onSelectReader->IsValid())
	{
		onSelectReader->ReadString("type", type);
		m_pOnSelect = CommandFactory::GetInstance().Create(type);
		if (m_pOnSelect != nullptr)
		{
			m_pOnSelect->Deserialize(onSelectReader.get(), context);
			context.Add(m_pOnSelect->GetId(), m_pOnSelect);
		}
	}
	
	auto onDeselectReader = reader->ReadObject("onDeselect");
	if (onDeselectReader != nullptr && onDeselectReader->IsValid())
	{
		onDeselectReader->ReadString("type", type);
		m_pOnDeselect = CommandFactory::GetInstance().Create(type);
		if (m_pOnDeselect != nullptr)
		{
			m_pOnDeselect->Deserialize(onDeselectReader.get(), context);
			context.Add(m_pOnDeselect->GetId(), m_pOnDeselect);
		}
	}
	
	ECS_Component::Deserialize(reader, context);
}

void ECS_ButtonComponent::RestoreContext(JsonReader const* reader, SerializationMap const& context)
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

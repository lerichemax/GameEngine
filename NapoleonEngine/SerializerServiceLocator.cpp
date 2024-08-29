#include "PCH.h"
#include "SerializerServiceLocator.h"

#include "Serializer.h"
#include "Deserializer.h"

Serializer* SerializerServiceLocator::m_pSerializer = nullptr;
Deserializer* SerializerServiceLocator::m_pDeserializer = nullptr;

void SerializerServiceLocator::RegisterSerializer(Serializer* pSerializer)
{ 
	if (m_pSerializer == nullptr)
	{
		m_pSerializer = pSerializer;
	}
};

void SerializerServiceLocator::RegisterDeserializer(Deserializer* pDeserializer)
{
	if (m_pDeserializer == nullptr)
	{
		m_pDeserializer = pDeserializer;
	}
};

void SerializerServiceLocator::CleanUp()
{
	SAFE_DELETE(m_pSerializer);
	SAFE_DELETE(m_pDeserializer);
}
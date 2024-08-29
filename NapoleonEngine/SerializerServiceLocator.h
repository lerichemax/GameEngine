#pragma once

class Serializer;
class Deserializer;

class SerializerServiceLocator final
{
	friend class NapoleonEngine;

public:
	static Serializer& GetSerializerService() { return *m_pSerializer; }
	static Deserializer& GetDeserializerService() { return *m_pDeserializer; }

	static void RegisterSerializer(Serializer* pSerializer);
	static void RegisterDeserializer(Deserializer* pDeserializer);

private:
	static Serializer* m_pSerializer;
	static Deserializer* m_pDeserializer;

	static void CleanUp();
};

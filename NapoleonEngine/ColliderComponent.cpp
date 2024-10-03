#include "PCH.h"
#include "ColliderComponent.h"

void ColliderComponent::SetShape(geo::Shape* pNewShape)
{
	assert(pShape == nullptr && "Can't reassign shape (yet)");

	pShape = std::unique_ptr<geo::Shape>(std::forward<geo::Shape*>(pNewShape));
}

geo::Shape* const ColliderComponent::GetShape() const
{
	return pShape.get();
}

void ColliderComponent::TriggerEnter(Entity other)
{
	OnTriggerEnter.Notify(other);
	OverlappingColliders.insert(other);
}

void ColliderComponent::TriggerExit(Entity other)
{
	OnTriggerExit.Notify(other);
	OverlappingColliders.erase(other);
}

void ColliderComponent::Collide(Entity other)
{
	OnCollision.Notify(other);
}

void ColliderComponent::Serialize(StreamWriter& writer) const
{
	Component::Serialize(writer);

	writer.WriteInt64("type", static_cast<int64_t>(std::type_index(typeid(ColliderComponent)).hash_code()));

	if (pShape == nullptr)
	{
		return;
	}

	writer.WriteObject("shape", pShape.get());
	writer.WriteBool("draw", bDraw);
	writer.WriteBool("trigger", bIsTrigger);
}

void ColliderComponent::Deserialize(JsonReader const* reader, SerializationMap& context)
{
	Component::Deserialize(reader, context);

	auto shapeObj = reader->ReadObject("shape");

	if (shapeObj == nullptr)
	{
		return;
	}
	int64_t type;
	shapeObj->ReadInt64("type", type);

	pShape = std::unique_ptr<geo::Shape>(std::forward<geo::Shape*>(Factory<geo::Shape>::Get().Create(static_cast<size_t>(type))));
	if (pShape != nullptr)
	{
		pShape->Deserialize(shapeObj.get());
	}

	reader->ReadBool("draw", bDraw);
	reader->ReadBool("trigger", bIsTrigger);
}
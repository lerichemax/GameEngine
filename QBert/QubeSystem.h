#pragma once
#include "System.h"
#include "Event.h"

#include "glm/glm.hpp"


class Characters;
class DiskSystem;
class QBertScene;
class PyramidSystem;
class QBert;
enum class ConnectionDirection;
enum class Level;
class QubeSystem final : public System 
{
public:
	QubeSystem() = default;
	~QubeSystem() = default;

	static EventHandler<QubeSystem> OnAnyQubeFlipped;

	void HandleQBertLanding(Entity qubeEntity);
	void Reset(Level level, Entity entity);

	void Serialize(StreamWriter& writer) const override;
	void SetSignature() const override;

protected:
	void Initialize() override;
	void Start() override;

private:
	void Flip(Entity qubeEntity);
	void UnFlip(Entity qubeEntity);
};
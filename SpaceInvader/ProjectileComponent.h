#pragma once
#include "Component.h"

#define PLAYER_PROJECTILE "PlayerProjectile"
#define ENEMY_PROJECTILE "EnemyProjectile"

struct ProjectileComponent final : public ecs::Component
{
	PROPERTY(float, MoveSpeed);
	PROPERTY(int, MoveDirection);
};

SERIALIZE_CLASS(ProjectileComponent, ecs::Component)
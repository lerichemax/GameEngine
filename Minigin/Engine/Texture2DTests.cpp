#include "MiniginPCH.h"
#include <catch.hpp>
#include "ResourceManager.h"

using namespace dae;

TEST_CASE("Texture creation", "Texture2D")
{
	// tell the resource manager where he can find the game data
	auto& resources = ResourceManager::GetInstance();
	resources.Init("../Data/");

	auto texture = resources.LoadTexture("logo.png");
	REQUIRE(texture != nullptr);
}
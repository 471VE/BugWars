#include "pch.h"
#include "Game.h"
#include "GameBase/Log.h"
#include "Tank.h"
#include "Bug.h"
#include "Bullet.h"

#include "GameBase/Framework.h"

Game* g_Game;

Game::Game()
	: GameBase({
				[] { Tank* tank = new Tank;  return tank; },
				[] {return new Bug; },
				[] {return new Bullet; } })
{
	g_Game = this;
}

Game::~Game() {
	for (size_t i = 0; i < objects.size(); ++i)
		delete objects[i];
	objects.clear();
}

void Game::OnUpdate(float dt)
{
	PIXScopedEvent(PIX_COLOR_INDEX(5), __FUNCTION__);

	for (size_t i = 0; i < objects.size(); ++i)
		if (!objects[i]->disabled) {
			objects[i]->Update(dt);
			objects[i]->visible = IsObjectOnScreen(objects[i]);
		}	else {
			delete objects[i];
			objects.erase(objects.begin() + i);
		}
}

void Game::OnRender() const
{
	for (auto& obj : objects)
		if (obj->visible)
			DoRender(obj);
}

void Game::AddObject(GameObject* object)
{
	object->disabled = false;
	objects.push_back(object);
}

void Game::OnBugsSpawned()
{
}

bool Game::IsObjectOnScreen(GameObject* object) {
	Point radius_vector = tank->position - object->position;
	return std::abs(radius_vector.y) < framework->screenSize.y / 2 + 32 && // half of the bug
				 std::abs(radius_vector.x) < framework->screenSize.x / 2 + 32;
}
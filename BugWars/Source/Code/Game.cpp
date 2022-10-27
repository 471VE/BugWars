#include "pch.h"
#include "Game.h"
#include "GameBase/Log.h"
#include "Tank.h"
#include "Bug.h"
#include "Bullet.h"

Game* g_Game;

Game::Game()
	: GameBase({
				[] {return new Tank; },
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
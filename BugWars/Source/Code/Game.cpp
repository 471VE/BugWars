#include "pch.h"
#include "Game.h"
#include "GameBase/Log.h"
#include "Tank.h"
#include "Bug.h"
#include "Bullet.h"
#include "GameBase/Framework.h"

// to clean up code a bit
#define game_objects (*all_objects[j])
#define game_object (*all_objects[j])[i]
#define game_object_index i

#define APPLY_TO_ALL_GAME_OBJECTS(X) \
	for (size_t j = 0; j < 2; ++j) { \
		for (size_t i = 0; i < (*all_objects[j]).size(); ++i) { \
			X \
		} \
	} \

Game* g_Game;

Game::Game()
	: GameBase({
				[] { Tank* tank = new Tank;  return tank; },
				[] {return new Bug; },
				[] {return new Bullet; } })
{
	g_Game = this;

	all_objects[0] = &bugs;
	all_objects[1] = &other_objects;
}

Game::~Game() {
	for (size_t j = 0; j < 2; ++j) {
		for (size_t i = 0; i < (*all_objects[j]).size(); ++i)
			delete (*all_objects[j])[i];
		(*all_objects[j]).clear();
	}
}

void Game::OnUpdate(float dt)
{
	PIXScopedEvent(PIX_COLOR_INDEX(5), __FUNCTION__);
	APPLY_TO_ALL_GAME_OBJECTS(
		if (!game_object->disabled) {
			game_object->Update(dt);
			game_object->visible = IsObjectOnScreen(game_object);
		}
		else {
			if (game_object->GetRTTI() == Bug::s_RTTI) {
				Bug* bug = static_cast<Bug*>(game_object);
				chunks[bug->i][bug->j].erase(game_object);
			}
			delete game_object;
			game_objects.erase(game_objects.begin() + game_object_index);
		}
	)
}

void Game::OnRender() const
{
	APPLY_TO_ALL_GAME_OBJECTS(
		if (game_object->visible)
			DoRender(game_object);
	)
}

void Game::AddObject(GameObject* object)
{
	object->disabled = false;
	if (object->GetRTTI() == Bug::s_RTTI) {
		int x_chunk = GetChunk(object->position.x);
		int y_chunk = GetChunk(object->position.y);

		static_cast<Bug*>(object)->i = x_chunk;
		static_cast<Bug*>(object)->j = y_chunk;

		chunks[x_chunk][y_chunk].insert(object);

		bugs.push_back(object);
	}
	else 
		other_objects.push_back(object);
}

void Game::OnBugsSpawned()
{
}

bool Game::IsObjectOnScreen(GameObject* object) {
	if (tank == nullptr) return false;
	Point radius_vector = tank->position - object->position;
	return std::abs(radius_vector.y) < framework->screenSize.y / 2 + 32 && // half of the bug
				 std::abs(radius_vector.x) < framework->screenSize.x / 2 + 32;
}

int GetChunk(float position_coordinate) {
	return static_cast<int>(position_coordinate) / chunkSize;
}
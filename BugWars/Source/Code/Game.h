#pragma once
#include "GameBase/GameBase.h"
#include "Globals.h"

const int fieldSize = 10000;
const int chunkSize = 500;
const int chunkNum = fieldSize / chunkSize;

const float radius = 500.f;
const float radius2 = radius * radius;

struct Tank;

struct Game : public GameBase
{
	Game();
	~Game();

	virtual void OnUpdate(float dt) override;
	virtual void OnRender() const override;
	virtual void AddObject(GameObject* object) override;

	virtual void OnBugsSpawned() override;
	bool IsObjectOnScreen(GameObject* object);

	std::vector<GameObject*> bugs;
	// Tests demand 'objects' variable, ¯\_(ツ)_/¯
	std::vector<GameObject*>& objects = bugs;

	std::vector<GameObject*> other_objects;
	std::vector<GameObject*>* all_objects[2];

	std::unordered_set<GameObject*> chunks[chunkNum][chunkNum];
};

int GetChunk(float position_coordinate);
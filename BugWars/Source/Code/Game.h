#pragma once
#include "GameBase/GameBase.h"
#include "Globals.h"

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
};
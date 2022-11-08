#include "pch.h"
#include "Bullet.h"
#include "Game.h"
#include "Bug.h"
#include "Tank.h"

IMPLEMENT_RTTI(Bullet);

void Bullet::OnStart(Point)
{
}

void Bullet::OnUpdate(float dt)
{
	int x_chunk = GetChunk(position.x);
	int y_chunk = GetChunk(position.y);
	for (auto object : g_Game->chunks[x_chunk * chunkNum + y_chunk])
		if (!object->disabled)
			if (object->position.Distance2(position) < object->GetRadius() * object->GetRadius())
			{
				g_Game->tank->score++;
				object->disabled = true;
				object->visible = false;
				disabled = true;
				visible = false;
				return;
			}
}

void Bullet::OnLifeEnd()
{
	disabled = true;
	visible = false;
}

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
	for (auto object : g_Game->bugs)
		if (!object->disabled)
			if (object->position.Distance(position) < object->GetRadius())
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

#include "pch.h"
#include "Bug.h"
#include "Game.h"

IMPLEMENT_RTTI(Bug);

void Bug::OnUpdate(float dt)
{
}

BugBase* Bug::FindBugToEat() const
{
	Bug* target = nullptr;
	float min_dist2 = std::numeric_limits<float>::max();
	for (auto object : g_Game->bugs)
	{
		if (object == this)
			continue;

		if (object->disabled)
			continue;

		if (object->id > id)
			continue; // Can't eat that

		float dist2 = position.Distance2(object->position);
		if (dist2 < min_dist2)
		{
			min_dist2 = dist2;
			target = static_cast<Bug*>(object);
		}
	}

	return target;
}

void Bug::OnEat(BugBase& first, BugBase& second)
{
	if (first.id > second.id)
	{
		second.disabled = true;
		second.visible = false;
	}
	else
	{
		first.disabled = true;
		first.visible = false;
	}
}

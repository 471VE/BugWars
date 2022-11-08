#include "pch.h"
#include "Bug.h"
#include "Game.h"

IMPLEMENT_RTTI(Bug);

void Bug::OnUpdate(float dt)
{
	int x_chunk = GetChunk(position.x);
	int y_chunk = GetChunk(position.y);

	if ((i == x_chunk && j == y_chunk) || x_chunk < 0)
		return;

	g_Game->chunks[i * chunkNum + j].erase(this);
	i = x_chunk;
	j = y_chunk;
	g_Game->chunks[i * chunkNum + j].insert(this);
}

BugBase* Bug::FindBugToEat() const
{
	Bug* target = nullptr;
	float min_dist2 = std::numeric_limits<float>::max();

	int x_chunk_max = std::min(chunkNum, GetChunk(position.x + radius) + 1);
	int x_chunk_min = std::max(0, GetChunk(position.x - radius));
	int y_chunk_max = std::min(chunkNum, GetChunk(position.y + radius) + 1);
	int y_chunk_min = std::max(0, GetChunk(position.y - radius));

	for (int i = x_chunk_min; i < x_chunk_max; ++i) {
		for (int j = y_chunk_min; j < y_chunk_max; ++j) {
			for (auto object : g_Game->chunks[i * chunkNum + j])
			{
				if (object == this)
					continue;

				if (object->disabled)
					continue;

				if (object->id > id)
					continue; // Can't eat that

				float dist2 = position.Distance2(object->position);
				if (dist2 < min_dist2 && dist2 < radius2)
				{
					min_dist2 = dist2;
					target = static_cast<Bug*>(object);
				}
			}
		}
	}

	if (target) {
		return target;
	}

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

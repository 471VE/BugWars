#include "pch.h"
#include "Tank.h"
#include "Globals.h"
#include "Game.h"
#include "Bullet.h"
#include "Bug.h"

IMPLEMENT_RTTI(Tank);

void Tank::OnUpdate(float dt)
{
}

BugBase* Tank::GetBugToShoot() const
{
	Bug* target_bug = nullptr;
	float min_distance2 = std::numeric_limits<float>::infinity();

	int x_chunk = GetChunk(position.x);
	int y_chunk = GetChunk(position.y);

	for (int i = std::max(0, x_chunk - 1); i < std::min(x_chunk + 2, chunkNum); ++i) {
		for (int j = std::max(0, y_chunk - 1); j < std::min(y_chunk + 2, chunkNum); ++j) {
			for (auto obj : g_Game->chunks[i * chunkNum + j]) {
				float distance2 = position.Distance2(obj->position);
				if (distance2 < min_distance2) {
					min_distance2 = distance2;
					target_bug = static_cast<Bug*>(obj);
				}
			}
		}
	}
	if (target_bug)
		return target_bug;

	for (auto obj : g_Game->bugs) {
		float distance2 = position.Distance2(obj->position);
		if (distance2 < min_distance2) {
			min_distance2 = distance2;
			target_bug = static_cast<Bug*>(obj);
		}
	}
	return target_bug;
}

Point Tank::CalcShootDirection(Point target_pos, Point target_dir, float target_vel, float bullet_vel) const
{
	Point direction_to_bug = target_pos - position;
	Point target_velocity = target_dir * target_vel;

	float a  = bullet_vel * bullet_vel - target_vel * target_vel;
	float b1 = -direction_to_bug.Dot(target_velocity);
	float c  = -direction_to_bug.Length2();

	float D1 = b1 * b1 - a * c;
	if (D1 <= 0)
		return direction_to_bug.Normalized();

	float D1sqrt = std::sqrt(D1);

	float t = (- b1 - D1sqrt) / a;
	if (t < 0)
		t = (- b1 + D1sqrt) / a;

	return (direction_to_bug + target_velocity * t).Normalized();
}

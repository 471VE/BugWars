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
	target_dir = target_dir.Normalized();
	Point direction_to_bug = target_pos - position;

	// Checks if vectors are parallel, if they are, no need to correct bullet direction:
	if (direction_to_bug.x * target_dir.y == direction_to_bug.y * target_dir.x)
		return direction_to_bug.Normalized();

	float a  = bullet_vel * bullet_vel - target_vel * target_vel;
	float b1 = -direction_to_bug.Dot(target_dir * target_vel);
	float c  = -direction_to_bug.Length2();

	float D1 = b1 * b1 - a * c;
	if (D1 < 0)
		return direction_to_bug.Normalized();

	float D1sqrt = std::sqrt(D1);

	float t = (b1 - D1sqrt) / a;
	if (t < 0)
		t = (b1 + D1sqrt) / a;

	Point future_target_pos = target_pos + target_dir * target_vel * t;
	return (future_target_pos - position).Normalized();
}

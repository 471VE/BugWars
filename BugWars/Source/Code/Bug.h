#pragma once
#include "GameBase/BugBase.h"

struct Bug : public BugBase
{
	DEFINE_RTTI;

	virtual void OnUpdate(float dt) override;
	virtual BugBase* FindBugToEat() const override;
	virtual void OnEat(BugBase& first, BugBase& second) override;

	int i = 0, j = 0;
};
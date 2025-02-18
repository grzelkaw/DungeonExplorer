#pragma once
#include "enemy.h"
class enemySlime : public enemy
{
public:
	enemySlime(Vector2f position, Vector2f velocity, Vector2f scale, int startingHP);
	void move(const player& player) override;
};


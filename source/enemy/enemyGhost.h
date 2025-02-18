#pragma once
#include "enemy.h"
class enemyGhost : public enemy
{
public:
	enemyGhost(Vector2f position, Vector2f velocity, int startingHP);
	void move(const player& player) override;
};


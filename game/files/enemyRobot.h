#pragma once
#include "enemy.h"
class enemyRobot : public enemy
{
public:
	enemyRobot(Vector2f position, Vector2f velocity, int startingHP);
	void move(const player& player) override;
};


#pragma once
#include "enemy.h"
class enemyDragon : public enemy
{
public:
	enemyDragon(Vector2f position, Vector2f velocity, int startingHP);
	void move(const player& player) override;
private:
	float speed = 3.f;
};


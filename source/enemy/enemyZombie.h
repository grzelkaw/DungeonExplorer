#pragma once
#include "enemy.h"
class enemyZombie : public enemy
{
public:
	enemyZombie(Vector2f position, Vector2f velocity, int startingHP);
	void move(const player& player) override;
	void stopMovementX();
	void startMovementX();


protected:
	float speed = 3.0f;
};


#pragma once
#include "enemy.h"
class enemyMage : public enemy
{
public:
	enemyMage(Vector2f position, Vector2f velocity, int startingHP);
	void move(const player& player) override;

private:
    float angularSpeed;
    float angle;
};


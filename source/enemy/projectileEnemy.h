#pragma once

#include "enemy.h"
class projectileEnemy : public enemy
{
public:
	projectileEnemy(Vector2f position, Vector2f velocity, int startingHP);
	void move(const player& player) override;

private:
	bool shooted = false;
	Vector2f direction;
	bool directionInitialized = false;
};


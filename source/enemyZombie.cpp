#include "enemyZombie.h"

enemyZombie::enemyZombie(Vector2f position, Vector2f velocity, int startingHP) : enemy(position, velocity, startingHP)
{
	if (!texture.loadFromFile("resources/textures/enemies/zombie/zombie.png")) {
		cout << "Brak tekstury zombie";
	}
	shape.setTexture(texture);
	shape.setScale(0.5f, 0.5f);
}


void enemyZombie::move(const player& player)
{
	Vector2f playerPosition = player.getPosition();
	playerPosition.x += 100.f;
	playerPosition.y += 70.f;
	Vector2f direction = playerPosition - position;
	Vector2f normalizedDirection = direction / std::sqrt(direction.x * direction.x + direction.y * direction.y);
	float offsetY = std::sin(clock() / 750.0) * 5.0f;
	position += Vector2f(normalizedDirection.x * speed, offsetY);
	shape.setPosition(position);
}

void enemyZombie::stopMovementX() {
	speed = 0.f;
}

void enemyZombie::startMovementX() {
	speed = 5.f;
}
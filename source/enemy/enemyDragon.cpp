#include "enemyDragon.h"

enemyDragon::enemyDragon(Vector2f position, Vector2f velocity, int startingHP) : enemy(position, velocity, startingHP)
{
	if (!texture.loadFromFile("resources/textures/enemies/dragon/dragon.png")) {
		cout << "Brak tekstury dragon";
	}
	shape.setTexture(texture);
	shape.setScale(0.5f, 0.5f);
}

void enemyDragon::move(const player& player)
{
	Vector2f playerPosition = player.getPosition();
	playerPosition.x += 100.f;
	playerPosition.y += 70.f;
	Vector2f direction = playerPosition - position;
	Vector2f normalizedDirection = direction / sqrt(direction.x * direction.x + direction.y * direction.y);
	float offsetX = sin(clock() / 750.0) * 5.0f;
	position += Vector2f(normalizedDirection.x  * 10 * offsetX, normalizedDirection.y * 15);
	shape.setPosition(position);
}
#include "enemyGhost.h"

enemyGhost::enemyGhost(Vector2f position, Vector2f velocity, int startingHP) : enemy(position, velocity, startingHP)
{
	if (!texture.loadFromFile("resources/textures/enemies/ghost/enemy1.png")) {
		cout << "Brak tekstury robot";
	}
	shape.setTexture(texture);
	shape.setScale(1.5f, 1.5f);
}

void enemyGhost::move(const player& player)
{
	Vector2f playerPosition = player.getPosition();
	playerPosition.x += 100.f;
	playerPosition.y += 70.f;
	Vector2f direction = playerPosition - position;
	Vector2f normalizedDirection = direction / std::sqrt(direction.x * direction.x + direction.y * direction.y);
	float speed = 1.5f;
	position += normalizedDirection * speed;
	shape.setPosition(position);
}
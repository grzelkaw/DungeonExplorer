#include "projectileEnemy.h"

projectileEnemy::projectileEnemy(Vector2f position, Vector2f velocity, int startingHP) : enemy(position, velocity, startingHP)
{
	if (!texture.loadFromFile("resources/textures/enemies/projectiles/fireball.png")) {
		cout << "Brak tekstury fireball";
	}
	shape.setTexture(texture);
	shape.setScale(0.3f, 0.3f);
}

void projectileEnemy::move(const player& player)
{
    if (!directionInitialized) {
        Vector2f playerPosition = player.getPosition();
        playerPosition.x += 200.f;
        playerPosition.y += 200.f;
        Vector2f direction = playerPosition - position;
        float magnitude = sqrt(direction.x * direction.x + direction.y * direction.y);
        if (magnitude != 0) {
            this->direction = direction / magnitude;
        }
        directionInitialized = true;
    }
    float speed = 10.f;
    position += direction * speed;
    shape.setPosition(position);
}
#include "enemySlime.h"
enemySlime::enemySlime(Vector2f position, Vector2f velocity, Vector2f scale, int startingHP) : enemy(position, velocity, startingHP)
{
	if (!texture.loadFromFile("resources/textures/enemies/slime/slime.png")) {
		cout << "Brak tekstury slime";
	}
	shape.setTexture(texture);
	shape.setScale(scale);
}


void enemySlime::move(const player& player)
{
	shape.move(velocity * 3.f);

	position = shape.getPosition();
}
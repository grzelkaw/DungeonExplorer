#include "enemyMage.h"

enemyMage::enemyMage(Vector2f position, Vector2f velocity, int startingHP) : enemy(position, velocity, startingHP)
{
	if (!texture.loadFromFile("resources/textures/enemies/mage/mage.png")) {
		cout << "Brak tekstury maga";
	}
	shape.setTexture(texture);
	shape.setScale(0.8f, 0.8f);
}

void enemyMage::move(const player& player)
{
    angle += angularSpeed;
    float x = position.x + 200 * cos(angle * 0.000000004 * (3.14159265f / 180.f));
    float y = position.y + 200 * sin(angle * 0.000000004 * (3.14159265f / 180.f));

    shape.setPosition(x, y);
}
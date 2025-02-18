#include "enemyRobot.h"
enemyRobot::enemyRobot(Vector2f position, Vector2f velocity, int startingHP) : enemy(position, velocity, startingHP)
{
	if (!texture.loadFromFile("resources/textures/enemies/robot/robot.png")) {
		cout << "Brak tekstury robot";
	}
	shape.setTexture(texture);
	shape.setScale(0.2f, 0.2f);
}


void enemyRobot::move(const player& player)
{
	shape.move(velocity * 3.f);

	position = shape.getPosition();
}
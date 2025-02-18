#pragma once
#include "object.h"
class objectChest : public object
{
public:
	objectChest(const Vector2f& position);
	void ifColliding(player& player, RenderWindow& window) override;
	void displayText(player& player, RenderWindow& window) override;

private:
	Texture texture;
	int random = rand() % 4;
};


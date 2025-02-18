#pragma once
#include "object.h"
class objectTrapdoor : public object
{
public:
	objectTrapdoor(const Vector2f& position, gameWorld& world);
	void ifColliding(player& player, RenderWindow& window) override;
	void displayText(player& player, RenderWindow& window) override;
	void openTrapdoor() override;

private:
	Texture texture1;
	Texture texture2;
	gameWorld& world;
	bool open = false;
};


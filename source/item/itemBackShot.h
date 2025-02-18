#pragma once
#include "item.h"
class itemBackShot : public item
{
public:
	itemBackShot(const Vector2f& position);
	void ifColliding(player& player, RenderWindow& window) override;
	void displayText(player& player, RenderWindow& window) override;

private:
	Texture texture;
};


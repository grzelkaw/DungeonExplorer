#pragma once
#include "item.h"
class itemHealing : public item
{
public:
	itemHealing(const Vector2f& position);
	void ifColliding(player& player, RenderWindow& window) override;
	void displayText(player& player, RenderWindow& window) override;

private:
	Texture texture;
};


#pragma once
#include "object.h"
class objectBarrel : public object
{
public:
	objectBarrel(const Vector2f& position);
	void ifColliding(player& player, RenderWindow& window) override;
	void displayText(player& player, RenderWindow& window) override;

private:
	Texture texture;
};


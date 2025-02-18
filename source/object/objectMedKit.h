#pragma once
#include "object.h"
class objectMedKit : public object
{
public:
	objectMedKit(const Vector2f& position);
	void ifColliding(player& player, RenderWindow& window) override;
	void displayText(player& player, RenderWindow& window) override;

private:
	Texture texture;
};


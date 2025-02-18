#pragma once
#include <SFML/Graphics.hpp>
#include "player.h"
#include "gameWorld.h"
#include <cstdlib>
#include <ctime>
#include <iostream>
using namespace sf;
using namespace std;
class object
{
public:
	object(const Vector2f& position);
	virtual ~object() = default;

	virtual void draw(RenderWindow& window);
	virtual bool checkCollision(player& player, RenderWindow& window);
	virtual void ifColliding(player& player, RenderWindow& window) = 0;
	virtual void displayText(player& player, RenderWindow& window);
	
	virtual void remove();
	virtual void openTrapdoor() {}

protected:
	RectangleShape shape;
	RenderWindow window;
	Font font;
	Text messageText;
};


#pragma once
#include <SFML/Graphics.hpp>
using namespace sf;
using namespace std;

class gameTile
{
public:
	Vector2f pos;
	Vector2f getPosition();
	Texture texture;
	Sprite sprite;
	gameTile(string, float, float, bool, bool, bool, bool);
	bool setUpSprite(string);
	FloatRect getGlobalBounds() const;
	bool checkCollision(const FloatRect& objectRect) const;
	bool isPassable() const;
	bool isSpike() const;
	bool isPortal() const;
	bool isSlime() const;
private:
	bool passable;
	bool spike;
	bool portal;
	bool slime;
	RectangleShape shape;
};


#include "gameTile.h"
#include <iostream>
using namespace sf;
using namespace std;

gameTile::gameTile(string textureName, float x, float y, bool passable, bool spike, bool portal, bool slime)
{
	if (!setUpSprite(textureName))
	{
		return;
	}
	pos = Vector2f(x, y);
	sprite.setPosition(pos);
	this->passable = passable;
	this->spike = spike;
	this->portal = portal;
	this->slime = slime;
}

bool gameTile::setUpSprite(string textureName)
{
	if (!texture.loadFromFile(textureName)) {
		return false;
	}
	texture.setSmooth(true);
	sprite.setTexture(texture);
	sprite.setTextureRect(IntRect(0, 0, 100, 100));
	return true;
}

Vector2f gameTile::getPosition() {
	return pos;
}

FloatRect gameTile::getGlobalBounds() const {
	return sprite.getGlobalBounds();
}

bool gameTile::checkCollision(const sf::FloatRect& objectRect) const {
	sf::FloatRect tileRect = getGlobalBounds();

	return tileRect.intersects(objectRect);
}

bool gameTile::isPassable() const
{
	return passable;
}

bool gameTile::isSpike() const
{
	return spike;
}

bool gameTile::isPortal() const
{
	return portal;
}

bool gameTile::isSlime() const
{
	return slime;
}
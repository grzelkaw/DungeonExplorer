#pragma once
#include <SFML/Graphics.hpp>
#include "animation.h"
#include "enemy.h"
#include "gameTile.h"
using namespace sf;
class projectile : public Drawable
{
public:
	projectile(float t_x, float t_y, char direction, int speed, float initialRotation, Texture& texture, animation& anim, int tripleShot);
	projectile() = delete;
	~projectile() = default;

	void update();
	void draw(RenderTarget& target, RenderStates state) const;
	void move();
	void updateTextureRect(const IntRect& rect);
	void updateAnimation(float deltaTime);
	FloatRect getGlobalBounds() const;
	bool checkCollision(const gameTile& tile) const;
	void bouncingShot(int speed);

	
private:
	Sprite sprite;
	Vector2f velocity;
	bool active;
	float rotation;
	animation& projectileAnimation;
	int changeCount = 0;
	
};


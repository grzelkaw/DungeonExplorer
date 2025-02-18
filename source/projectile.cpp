#include "projectile.h"
using namespace sf;
projectile::projectile(float t_x, float t_y,char direction,int speed, float initialRotation, Texture& texture, animation& anim, int tripleShot) : active(true), projectileAnimation(anim)
{
	sprite.setTexture(texture);
	sprite.setPosition(t_x, t_y);
	sprite.setRotation(initialRotation);
	sprite.setScale(3.f, 3.f);
	sprite.setOrigin(sprite.getGlobalBounds().width / 2, sprite.getGlobalBounds().height / 2);

	switch (direction)
	{
	case 'U':
		switch (tripleShot)
		{
		case 0:
			this->velocity.y = -speed;
			break;
		case 1:
			this->velocity.x = -speed;
			this->velocity.y = -speed;
			break;
		case 2:
			this->velocity.x = speed;
			this->velocity.y = -speed;
			break;
		
		}
		break;
	case 'D':
		switch (tripleShot)
		{
		case 0:
			this->velocity.y = speed;
			break;
		case 1:
			this->velocity.x = speed;
			this->velocity.y = speed;
			break;
		case 2:
			this->velocity.x = -speed;
			this->velocity.y = speed;
			break;
		}
		break;
	case 'R':
		switch (tripleShot)
		{
		case 0:
			this->velocity.x = speed;
			break;
		case 1:
			this->velocity.x = speed;
			this->velocity.y = -speed;
			break;
		case 2:
			this->velocity.x = speed;
			this->velocity.y = speed;
			break;
		}
		break;
	case 'L':
		switch (tripleShot)
		{
		case 0:
			this->velocity.x = -speed;
			break;
		case 1:
			this->velocity.x = -speed;
			this->velocity.y = -speed;
			break;
		case 2:
			this->velocity.x = -speed;
			this->velocity.y = speed;
			break;
		}
		break;
	case 'N':
		this->velocity.x = 0;
		this->velocity.y = 0;
		break;
	}
}
void projectile::draw(RenderTarget& target, RenderStates state) const
{
	target.draw(this->sprite, state);
}

void projectile::update()
{
	sprite.move(velocity);
}

FloatRect projectile::getGlobalBounds() const
{
	FloatRect originalBounds = sprite.getGlobalBounds();

	float newWidth = originalBounds.width / 2.0f;
	float newHeight = originalBounds.height / 2.0f;

	float newLeft = originalBounds.left + (originalBounds.width - newWidth) / 2.0f;
	float newTop = originalBounds.top + (originalBounds.height - newHeight) / 1;

	return FloatRect(newLeft, newTop, newWidth, newHeight);
}

void projectile::move() {
	sprite.setPosition(-2000,-2000);
}

void projectile::updateTextureRect(const IntRect& rect) {
	sprite.setTextureRect(rect);
}

void projectile::updateAnimation(float deltaTime) {
	projectileAnimation.update(0, deltaTime);
	sprite.setTextureRect(projectileAnimation.uvRect);
}

bool projectile::checkCollision(const gameTile& tile) const {
	FloatRect projectileRect = getGlobalBounds();
	FloatRect tileRect = tile.getGlobalBounds();
	return projectileRect.intersects(tileRect);
}

void projectile::bouncingShot(int speed)
{
	int randomDirectionX = (rand() % 3) - 1;
	int randomDirectionY = (rand() % 3) - 1;

	this->velocity.x = randomDirectionX * speed;
	this->velocity.y = randomDirectionY * speed;

	changeCount++;
	if (changeCount >= 5) {
		move();
	}
	
}
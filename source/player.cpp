#include "player.h"
#include <iostream>
player::player(Vector2f position, Vector2f size, int startingLvl, int startingDamage, int startingVelocity, int startingHp, Texture& texture, animation& anim) : lvl(startingLvl), damage(startingDamage), hp(startingHp), velocity(startingVelocity), playerAnimation(anim)
{
	sprite.setTexture(texture);
	sprite.setPosition(position);
	sprite.setScale(size.x / texture.getSize().x, size.y / texture.getSize().y);
	collisionRect = Rect<float>(position, size);
	lastDamageTime = Time::Zero;
	clock.restart();
}

void player::draw(RenderTarget& target, RenderStates states) const
{
	target.draw(sprite, states);
}

Rect<float> player::getCollisionRect() const
{
	float newWidth = collisionRect.width / 2.0f;
	float newHeight = collisionRect.height / 2.0f;
	float newLeft = collisionRect.left + (collisionRect.width - newWidth) / 2.0f;
	float newTop = collisionRect.top + (collisionRect.height - newHeight) / 2.0f;

	return Rect<float>(newLeft, newTop, newWidth, newHeight);
}

void player::setPosition(Vector2f newPos)
{
	position = newPos;
	sprite.setPosition(newPos);

	float hitboxWidth = 100.0f;
	float hitboxHeight = 160.f;

	float hitboxOffsetX = 90.f;
	float hitboxOffsetY = 56.f;

	collisionRect.left = newPos.x + hitboxOffsetX;
	collisionRect.top = newPos.y + hitboxOffsetY;
	collisionRect.width = hitboxWidth;
	collisionRect.height = hitboxHeight;
}

Vector2f player::getPosition() const
{
	return position;
}

void player::updateTextureRect(const sf::IntRect& rect) {
	sprite.setTextureRect(rect);
}

void player::updateAnimation(float deltaTime) {
	playerAnimation.update(0, deltaTime);
	sprite.setTextureRect(playerAnimation.uvRect);
}

bool player::checkCollision(const gameTile& tile) const {
	FloatRect playerRect = getCollisionRect();
	FloatRect tileRect = tile.getGlobalBounds();
	return playerRect.intersects(tileRect);
}

void player::setDamage(int newDamage) {
	damage = newDamage;
}

int player::getDamage() const {
	return damage;
}

void player::setHp(int newHp, RenderWindow& window) {
	if (hp >= 100) { hp = 100; }
	Time currentTime = clock.getElapsedTime();
	Time elapsedTime = currentTime - lastDamageTime;
	RectangleShape pauseRectangle(Vector2f(3840, 2160));
	Color pauseColor = Color::Red;
	pauseColor.a = 200;
	pauseRectangle.setFillColor(pauseColor);
	if (elapsedTime.asSeconds() >= 1.0) {
		window.draw(pauseRectangle);
		hp = newHp;
		lastDamageTime = currentTime;
	}
}

int player::getHp()
{
	return hp;
}

void player::setHpMedKit(int newHp) {
	if (newHp >= fullHp) { newHp = fullHp; }
	hp = newHp;
}

int player::getFullHp()
{
	return fullHp;
}

void player::setFullHp(int newFullHp) {
	fullHp = newFullHp;
	if (hp >= fullHp)
	{
		hp = fullHp;
	}
}

void player::setGold(int newGold)
{
	gold = newGold;
}

int player::getGold()
{
	return gold;
}

void player::setVelocity(int newVelocity)
{
	velocity = newVelocity;
}

int player::getVelocity() const
{
	return velocity;
}

int player::getLvl() const {
	return lvl;
}

void player::setLvl(int newLvl) {
	lvl = newLvl;
}


//items 

bool player::getItem(int itemID)
{
	switch (itemID)
	{
	case 0:
		return flying;
	case 1:
		return dashing;
	case 2:
		return healing;
	case 3:
		return tripleShot;
	case 4:
		return spectralShot;
	case 5:
		return penetrateShot;
	case 6:
		return bouncingShot;
	case 7:
		return backShot;
	}
}

void player::giveItem(int itemID)
{
	switch (itemID)
	{
	case 0:
		flying = true;
		break;
	case 1:
		dashing = true;
		break;
	case 2:
		healing = true;
		break;
	case 3:
		tripleShot = true;
		break;
	case 4:
		spectralShot = true;
		break;
	case 5:
		penetrateShot = true;
		break;
	case 6:
		bouncingShot = true;
		break;
	case 7:
		backShot = true;
		break;
	}
}




void player::setDefault()
{
	damage = 10;
	hp = 100;
	velocity = 5;
	lvl = 1;
	gold = 0;
	flying = false;
	dashing = false;
	healing = false;
	tripleShot = false;
	spectralShot = false;
	penetrateShot = false;
	bouncingShot = false;
	backShot = false;
}

void player::displayHp(RenderTarget& target, Font& font) {
	Text hpText;
	hpText.setFont(font);
	hpText.setCharacterSize(30);
	hpText.setFillColor(Color::Black);
	hpText.setString(to_string(hp) + "/" + to_string(fullHp));

	const float maxHpBarWidth = 285.f;
	const float textWidth = hpText.getLocalBounds().width;
	hpText.setPosition(10 + (maxHpBarWidth - textWidth) / 2.f, 33);

	const float maxHpBarLength = maxHpBarWidth;
	const float hpBarLength = min(static_cast<float>(hp) / fullHp * maxHpBarLength, maxHpBarLength);

	RectangleShape maxHpBar(Vector2f(maxHpBarWidth, 50));
	maxHpBar.setFillColor(Color(128, 128, 128));
	maxHpBar.setPosition(10, 25);
	
	RectangleShape hpBar(Vector2f(hpBarLength, 50));
	hpBar.setFillColor(Color::Red);
	hpBar.setPosition(10, 25);

	target.draw(maxHpBar);
	target.draw(hpBar);
	target.draw(hpText);
}

void player::displayExp(RenderTarget& target, Font& font, int& eliminations) {
	if (eliminations >= getLvl() * 100 && eliminations != 0) { eliminations = 0; };
	Text expText;
	expText.setFont(font);
	expText.setCharacterSize(30);
	expText.setFillColor(Color::Black);
	expText.setString(to_string(lvl));
	//expText.setString(to_string(eliminations) + "/" + to_string(lvl * 100));


	const float maxExpBarWidth = 285.f;
	const float textWidth = expText.getLocalBounds().width;
	expText.setPosition(310 + (maxExpBarWidth - textWidth) / 2.f, 33);

	const float maxExpBarLength = maxExpBarWidth;
	const float expBarLength = min(static_cast<float>(eliminations) / (lvl * 100) * maxExpBarLength, maxExpBarLength);

	RectangleShape maxExpBar(Vector2f(maxExpBarWidth, 50));
	maxExpBar.setFillColor(Color(128, 128, 128));
	maxExpBar.setPosition(310, 25);

	RectangleShape expBar(Vector2f(expBarLength, 50));
	expBar.setFillColor(Color::Yellow);
	expBar.setPosition(310, 25);

	target.draw(maxExpBar);
	target.draw(expBar);
	target.draw(expText);
}

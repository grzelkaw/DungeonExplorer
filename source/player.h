#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include "animation.h"
#include "gameTile.h"
using namespace sf;
class player : public Drawable
{
public:
	player(Vector2f position, Vector2f size, int startingLvl, int startingDamage, int startingVelocity, int startingHp, Texture& texture, animation& anim);
	player() = delete;
	~player() = default;

	Rect<float> getCollisionRect() const;
	void setPosition(Vector2f newPos);
	void updateTextureRect(const IntRect& rect);
	void updateAnimation(float deltaTime);
	virtual void draw(RenderTarget& target, RenderStates states) const override;
	bool checkCollision(const gameTile& tile) const;
	void setDefault();

	void setLvl(int newLvl);
	int getLvl() const;

	void setDamage(int newDamage);
	int getDamage() const;

	void setHp(int newHp, RenderWindow& window);
	void setHpMedKit(int newHp);
	int getHp();

	void setFullHp(int newFullHp);
	int getFullHp();

	void setGold(int newGold);
	int getGold();

	void setVelocity(int newVelocity);
	int getVelocity() const;


	void giveItem(int itemID);
	bool getItem(int itemID);

	void displayHp(RenderTarget& target, Font& font);
	void displayExp(RenderTarget& target, Font& font, int& eliminations);

	Vector2f getPosition() const;



private:
	Sprite sprite;
	Vector2f position;
	Rect<float> collisionRect;
	animation& playerAnimation;
	int damage;
	int hp;
	int fullHp = 100;
	int velocity;
	int lvl;
	int gold = 0;
	bool flying;
	bool dashing;
	bool healing;
	bool tripleShot;
	bool spectralShot;
	bool penetrateShot;
	bool bouncingShot;
	bool backShot;

	Time lastDamageTime;
	Clock clock;
};
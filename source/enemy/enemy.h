#pragma once
#include <SFML/Graphics.hpp>
#include <iostream>
#include "player.h"
#include "gameTile.h"
#include "animation.h"
#include "projectile.h"

using namespace sf;
using namespace std;

class enemy : public Drawable
{
public:
    enemy(Vector2f position, Vector2f velocity, int startingHP);
    virtual ~enemy() = default;

    virtual void draw(RenderTarget& target, RenderStates state) const;
    virtual void remove();
    virtual void move(const player& player) = 0;
    virtual void changeDirection();
    virtual FloatRect getGlobalBounds() const;
    bool checkCollision(const gameTile& tile) const;
    virtual bool checkPlayerCollision(const player& player) const;
    virtual void setHP(int newHP) { hp = newHP; }
    virtual int getHP() const { return hp; }
    virtual void displayHp(RenderTarget& target, Font& font, int fontSize, const Color& color);
    virtual void refreshHp(int newHp);
    virtual int getOriginalHp() const;
    virtual void killOfScreen(RenderWindow& window);

    Vector2f getPosition() const;

protected:
    Sprite shape;
    Vector2f position;
    Vector2f velocity;
    int hp;
    int originalHp;
    Texture texture;
};

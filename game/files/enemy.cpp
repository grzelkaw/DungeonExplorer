#include "enemy.h"

enemy::enemy(Vector2f position, Vector2f velocity, int startingHP) : position(position), velocity(velocity), hp(startingHP), originalHp(startingHP)
{
    shape.setPosition(position);
    shape.setOrigin(shape.getGlobalBounds().width / 2, shape.getGlobalBounds().height / 2);

}

void enemy::draw(RenderTarget& target, RenderStates state) const
{
    target.draw(shape, state);
}

void enemy::remove()
{
    position.x = -100000.f;
    position.y = -100000.f;
    shape.setPosition(position);
}

void enemy::changeDirection()
{
    float newXDir = (rand() % 3 - 1);
    while (newXDir == velocity.x) {
        newXDir = (rand() % 3 - 1);
    }
    float newYDir = (rand() % 3 - 1);
    while (newYDir == velocity.y) {
        newYDir = (rand() % 3 - 1);
    }

    velocity.x = newXDir;
    velocity.y = newYDir;
}

FloatRect enemy::getGlobalBounds() const
{
    return shape.getGlobalBounds();
}

bool enemy::checkCollision(const gameTile& tile) const {
    FloatRect enemyRect = getGlobalBounds();
    FloatRect tileRect = tile.getGlobalBounds();
    return enemyRect.intersects(tileRect);
}

bool enemy::checkPlayerCollision(const player& player) const
{
    Rect<float> playerRect = player.getCollisionRect();
    FloatRect enemyBounds = shape.getGlobalBounds();

    if (playerRect.intersects(enemyBounds))
        return true;

    return false;
}

Vector2f enemy::getPosition() const {
    return position;
}

void enemy::refreshHp(int newHP) {
    hp = newHP;
}

int enemy::getOriginalHp() const {
    return originalHp;
}

void enemy::displayHp(RenderTarget& target, Font& font, int fontSize, const Color& color) {
    Text hpText;
    hpText.setFont(font);
    hpText.setCharacterSize(fontSize);
    hpText.setFillColor(color);
    hpText.setString(to_string(hp) + "/" + to_string(originalHp));
    hpText.setPosition(position.x - shape.getGlobalBounds().width / 2, position.y - shape.getGlobalBounds().height / 2 - 10);
    target.draw(hpText);

    float maxHpBarLength = hpText.getGlobalBounds().width;
    float hpBarLength = std::min(static_cast<float>(hp) / originalHp * maxHpBarLength, maxHpBarLength);
    float maxHpBarLengthScaled = std::min(static_cast<float>(originalHp) / originalHp * maxHpBarLength, maxHpBarLength);

    RectangleShape maxHpBar(Vector2f(maxHpBarLengthScaled, 10));
    maxHpBar.setFillColor(Color(128, 128, 128));
    maxHpBar.setPosition(position.x - shape.getGlobalBounds().width / 2, position.y - shape.getGlobalBounds().height / 2 + fontSize);
    target.draw(maxHpBar);

    RectangleShape hpBar(Vector2f(hpBarLength, 10));
    hpBar.setFillColor(Color::Red);
    hpBar.setPosition(position.x - shape.getGlobalBounds().width / 2, position.y - shape.getGlobalBounds().height / 2 + fontSize);
    target.draw(hpBar);
}

void enemy::killOfScreen(RenderWindow& window)
{
    if (position.x > window.getSize().x + 7000 || position.x < -5000 || position.y > window.getSize().y + 5000 || position.y < -5000)
    {
        hp = -1;
    }
}
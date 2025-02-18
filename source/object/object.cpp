#include "object.h"
object::object(const Vector2f& position)
{
    shape.setPosition(position);
}

void object::draw(sf::RenderWindow& window) {
    window.draw(shape);
}

void object::remove()
{
    shape.setPosition(-20000, -20000);
}

bool object::checkCollision(player& player, RenderWindow& window)
{
    FloatRect playerBounds = player.getCollisionRect();
    FloatRect objectBounds = shape.getGlobalBounds();

    if (playerBounds.intersects(objectBounds)) {
        ifColliding(player, window);
        return true;
    }
    return false;
}
void object::displayText(player& player, RenderWindow& window) {}
#include "item.h"
item::item(const Vector2f& position)
{
    shape.setPosition(position);
}

void item::draw(RenderWindow& window) {
    window.draw(shape);
    if (!font.loadFromFile("resources/fonts/minecraft.ttf")) {
        cout << "brak czcionki";
    }
}

void item::remove()
{
    shape.setPosition(-20000, -20000);
}

bool item::checkCollision(player& player, RenderWindow& window)
{
    FloatRect playerBounds = player.getCollisionRect();
    FloatRect objectBounds = shape.getGlobalBounds();

    if (playerBounds.intersects(objectBounds)) {
        ifColliding(player, window);
        return true;
    }
    return false;
}
void item::displayText(player& player, RenderWindow& window) {}
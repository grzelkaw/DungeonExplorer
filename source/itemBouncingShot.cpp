#include "itemBouncingShot.h"
itemBouncingShot::itemBouncingShot(const Vector2f& position) : item(position)
{
    if (!texture.loadFromFile("resources/textures/items/bouncingShot.png")) {
        cout << "Brak tekstury bouncingShot";
    }
    shape.setTexture(&texture);
    shape.setSize(Vector2f(100, 100));

}

void itemBouncingShot::ifColliding(player& player, RenderWindow& window)
{
    if (player.getGold() >= 50)
    {
        player.setGold(player.getGold() - 50);
        player.giveItem(6);
        remove();
    }
}

void itemBouncingShot::displayText(player& player, RenderWindow& window)
{
    Text messageText;
    messageText.setFont(font);
    messageText.setCharacterSize(50);
    messageText.setFillColor(Color(255, 215, 0));
    messageText.setString(to_string(50));

    FloatRect textBounds = messageText.getLocalBounds();
    messageText.setOrigin(textBounds.left + textBounds.width / 2.0f, textBounds.top + textBounds.height / 2.0f);
    messageText.setPosition(window.getSize().x / 2, window.getSize().y / 2 + 100);
    window.draw(messageText);
}
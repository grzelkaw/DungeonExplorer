#include "itemHealing.h"
itemHealing::itemHealing(const Vector2f& position) : item(position)
{
    if (!texture.loadFromFile("resources/textures/items/healing.png")) {
        cout << "Brak tekstury healing";
    }
    shape.setTexture(&texture);
    shape.setSize(Vector2f(100, 100));
}

void itemHealing::ifColliding(player& player, RenderWindow& window)
{
    if (player.getHp() >= 25)
    {
        player.setHpMedKit(player.getHp() - 25);
        player.setFullHp(player.getFullHp() - 25);
        player.giveItem(2);
        remove();
    }    
}

void itemHealing::displayText(player& player, RenderWindow& window)
{
    Text messageText;
    messageText.setFont(font);
    messageText.setCharacterSize(50);
    messageText.setFillColor(Color::Red);
    messageText.setString(to_string(25));

    FloatRect textBounds = messageText.getLocalBounds();
    messageText.setOrigin(textBounds.left + textBounds.width / 2.0f, textBounds.top + textBounds.height / 2.0f);
    messageText.setPosition(window.getSize().x / 2, window.getSize().y / 2 + 100);
    window.draw(messageText);
}
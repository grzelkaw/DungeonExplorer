#include "objectMedKit.h"
objectMedKit::objectMedKit(const Vector2f& position) : object(position)
{
    if (!texture.loadFromFile("resources/textures/objects/medKit.png")) {
        cout << "Brak tekstury apteczki";
    }
    shape.setTexture(&texture);
    shape.setSize(Vector2f(100,80));
}

void objectMedKit::ifColliding(player& player, RenderWindow& window)
{
    if (player.getHp() < 100 && player.getGold() >= 10)
    {
        player.setGold(player.getGold() - 10);
        player.setHpMedKit(player.getHp() + 10);
        remove();
    }
}

void objectMedKit::displayText(player& player, RenderWindow& window)
{
    if (player.getHp() < 100 && player.getGold() >= 10)
    {
        if (!font.loadFromFile("resources/fonts/minecraft.ttf")) {
            cout << "brak czcionki";
        }

        messageText.setFont(font);
        messageText.setCharacterSize(50);
        messageText.setFillColor(Color::Red);
        messageText.setString("+HP");

        FloatRect textBounds = messageText.getLocalBounds();
        messageText.setOrigin(textBounds.left + textBounds.width / 2.0f, textBounds.top + textBounds.height / 2.0f);
        messageText.setPosition(player.getPosition().x + 190, player.getPosition().y + 80);
        window.draw(messageText);
    }
}
#include "objectChest.h"
objectChest::objectChest(const Vector2f& position) : object(position)
{
    if (!texture.loadFromFile("resources/textures/objects/chest.png")) {
        cout << "Brak tekstury apteczki";
    }
    shape.setTexture(&texture);
    shape.setSize(Vector2f(95, 95));
    srand(time(NULL));
}

void objectChest::ifColliding(player& player, RenderWindow& window)
{
    switch (random) {
    case 0:
        player.setHpMedKit(player.getHp() + 2);
        break;
    case 1:
        player.setGold(player.getGold() + 10);
        break;
    case 2:
        player.setDamage(player.getDamage() + 2);
        break;
    case 3:
        break;
    }
    remove();
}

void objectChest::displayText(player& player, RenderWindow& window)
{
    if (!font.loadFromFile("resources/fonts/minecraft.ttf")) {
        cout << "brak czcionki";
    }

    messageText.setFont(font);
    messageText.setCharacterSize(50);

    switch (random) {
    case 0:
        messageText.setFillColor(Color::Red);
        messageText.setString("HP");
        break;
    case 1:
        messageText.setFillColor(Color(255, 215, 0));
        messageText.setString("GOLD");
        break;
    case 2:
        messageText.setFillColor(Color(200, 200, 200));
        messageText.setString("DAMAGE");
        break;
    case 3:
        messageText.setFillColor(Color::White);
        messageText.setString("EMPTY");
        break;
    }

    FloatRect textBounds = messageText.getLocalBounds();
    messageText.setOrigin(textBounds.left + textBounds.width / 2.0f, textBounds.top + textBounds.height / 2.0f);
    messageText.setPosition(player.getPosition().x + 190, player.getPosition().y + 80);
    window.draw(messageText);
}
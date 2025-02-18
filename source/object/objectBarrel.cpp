#include "objectBarrel.h"
objectBarrel::objectBarrel(const Vector2f& position) : object(position)
{
    if (!texture.loadFromFile("resources/textures/objects/barrel.png")) {
        cout << "Brak tekstury barrel";
    }
    shape.setTexture(&texture);
    shape.setSize(Vector2f(50, 50));
}

void objectBarrel::ifColliding(player& player, RenderWindow& window)
{
    cout << "Kolizja z beczka" << endl;
}

void objectBarrel::displayText(player& player, RenderWindow& window)
{
    if (!font.loadFromFile("resources/fonts/minecraft.ttf")) {
        cout << "brak czcionki";
    }

    messageText.setFont(font);
    messageText.setCharacterSize(50);

    srand(time(NULL));
    int random = rand() % 4;
    switch (random) {
    case 0:
        messageText.setFillColor(Color::Red);
        messageText.setString("HP +5");
        break;
    case 1:
        messageText.setFillColor(Color(255, 215, 0));
        messageText.setString("GOLD +10");
        break;
    case 2:
        messageText.setFillColor(Color(200, 200, 200));
        messageText.setString("DAMAGE +10");
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
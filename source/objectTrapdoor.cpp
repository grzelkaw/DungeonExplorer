#include "objectTrapdoor.h"
objectTrapdoor::objectTrapdoor(const Vector2f& position, gameWorld& world) : object(position), world(world)
{
    if (!texture1.loadFromFile("resources/textures/objects/trapdoorClosed.png")) {
        cout << "Brak tekstury trapdoor";
    }

    if (!texture2.loadFromFile("resources/textures/objects/trapdoorOpen.png")) {
        cout << "Brak tekstury trapdoor";
    }
    shape.setTexture(&texture1);
    shape.setSize(Vector2f(202, 198));
}

void objectTrapdoor::openTrapdoor()
{
    open = true;
    shape.setTexture(&texture2);
}

void objectTrapdoor::ifColliding(player& player, RenderWindow& window)
{
    if (open == true)
    {
        world.resetMap();
        Vector2f center((world.mapWidth / 2.0f) - 200, (world.mapHeight / 2.0f) + 100);
        player.setPosition(center);
    }
}

void objectTrapdoor::displayText(player& player, RenderWindow& window)
{

}
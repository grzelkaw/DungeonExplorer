#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/Audio.hpp>
#include "player.h"
#include "projectile.h"
#include "projectileEnemy.h"
#include "enemy.h"
#include "enemyGhost.h"
#include "enemyZombie.h"
#include "enemyRobot.h"
#include "enemySlime.h"
#include "enemyDragon.h"
#include "enemyMage.h"
#include "gameTile.h"
#include "gameWorld.h"
#include "animation.h"

#include "object.h"
#include "objectMedKit.h"
#include "objectBarrel.h"
#include "objectChest.h"
#include "objectTrapdoor.h"

#include "item.h"
#include "itemHealing.h"
#include "itemTripleShot.h"
#include "itemSpectralShot.h"
#include "itemPenetrateShot.h"
#include "itemBouncingShot.h"
#include "itemBackShot.h"

#include <cstdlib>
#include <ctime>
#include <iostream>
#include <vector>
#include <fstream>
#include <cmath>

using namespace sf;
using namespace std;

int eliminations = 0;
const int tileSize = 100;
int startRow, endRow, startCol, endCol;
int levelDirection;
bool isWalking = false;
bool spawnLimitReached1 = false;
bool spawnLimitReached2 = false;
bool spawnLimitReached3 = false;
bool damageSoundPlayed = false;
bool canDash = true;
bool isPaused = false;
bool playMusic = true;
bool colorMusic = true;
int selectedOption = 0;
bool movePlayer = false;
bool giveLevel = true;
bool changeVelocity = true;
bool coop = false;

void showStartScreen(RenderWindow& window)
{
    Font fontPixel;
    if (!fontPixel.loadFromFile("resources/fonts/minecraft.ttf"))
    {
        cout << "brak czcionki";
    }

    Text textTitle("DungeonExplorer", fontPixel, 100);
    textTitle.setFillColor(Color::White);
    textTitle.setPosition(window.getSize().x / 2 - textTitle.getLocalBounds().width / 2, window.getSize().y / 2 - textTitle.getLocalBounds().height / 2 - 250);

    Text textCredits("by Wiktor Grzelka", fontPixel, 20);
    textCredits.setFillColor(Color::White);
    textCredits.setPosition(window.getSize().x / 2 - textTitle.getLocalBounds().width / 2, window.getSize().y / 2 - textTitle.getLocalBounds().height / 2 - 150);

    Text textStart("Singleplayer", fontPixel, 50);
    textStart.setFillColor(Color::White);
    textStart.setPosition(window.getSize().x / 2 - 145, window.getSize().y / 2 - textStart.getLocalBounds().height / 2 - 100);

    Text textCoop("Multiplayer", fontPixel, 50);
    textCoop.setFillColor(Color::White);
    textCoop.setPosition(window.getSize().x / 2 - 145, window.getSize().y / 2 - textCoop.getLocalBounds().height / 2- 30);

    Texture pauseBackgroundTexture;
    if (!pauseBackgroundTexture.loadFromFile("resources/textures/pause.png")) {
        cout << "Blad ladowania pauzy";
    }
    Sprite pauseBackground(pauseBackgroundTexture);

    RectangleShape selector(Vector2f(305, 65));
    selector.setFillColor(Color::Transparent);
    selector.setOutlineThickness(5);
    selector.setOutlineColor(Color::White);

    Clock startScreenClock;
    

    while (true)
    {
        Event event;
        while (window.pollEvent(event))
        {
            if (event.type ==  Event::Closed)
            {
                window.close();
                return;
            }
            if (startScreenClock.getElapsedTime() > seconds(0.15f)) {
                if (Keyboard::isKeyPressed(Keyboard::Up) || Keyboard::isKeyPressed(Keyboard::W)) {
                    selectedOption = (selectedOption - 1 + 2) % 2;
                    startScreenClock.restart();
                }
                else if (Keyboard::isKeyPressed(Keyboard::Down) || Keyboard::isKeyPressed(Keyboard::S)) {
                    selectedOption = (selectedOption + 1) % 2;
                    startScreenClock.restart();
                }
            }

            switch (selectedOption) {
            case 0:
                selector.setPosition(textStart.getPosition().x - 10, textStart.getPosition().y);
                if (Keyboard::isKeyPressed(Keyboard::Space) || Keyboard::isKeyPressed(Keyboard::Enter)) {
                    return;
                }
                break;
            case 1:
                selector.setPosition(textCoop.getPosition().x - 10, textCoop.getPosition().y);
                if (Keyboard::isKeyPressed(Keyboard::Space) || Keyboard::isKeyPressed(Keyboard::Enter))
                {
                    coop = true;
                    return;
                }
                break;
            }
        }

        window.clear();
        window.draw(pauseBackground);
        window.draw(selector);
        window.draw(textTitle);
        window.draw(textCredits);
        window.draw(textStart);
        window.draw(textCoop);
        window.display();
    }
}


void lvlUp(player& player, int& eliminations, int& playerLvl, int& playerDamage, int& playerVelocity, int& playerGold, float& minTimeBetweenShots)
{
    if (eliminations >= playerLvl * 100 && eliminations != 0)
    {
        
        playerLvl ++;
        player.setLvl(playerLvl);

        playerDamage += 5;
        player.setDamage(playerDamage);

        playerVelocity += 1;
        player.setVelocity(playerVelocity);

        playerGold += playerLvl * 2;    
        player.setGold(playerGold);

        if (playerVelocity >= 15)
        {
            player.setVelocity(15);
        }

        minTimeBetweenShots -= 0.033;
        if (minTimeBetweenShots <= 0.2)
        {
            minTimeBetweenShots = 0.2;
        }
        eliminations = 0;

    }
}

void saveGame(player& player, int& eliminations, int& playerHp, int& playerLvl, int& playerDamage, int& playerVelocity, int& gold, float& minTimeBeetweenShots, bool dash, bool fly)
{
    string save = "resources/save.txt";
    ofstream file(save);
    if (!file.is_open())
    {
        cout << "Nie mozna otworzyc pliku";
    }

    file << eliminations << endl;
    file << playerHp << endl;
    file << playerLvl << endl;
    file << playerDamage << endl;
    file << playerVelocity << endl;
    file << gold << endl;
    file << minTimeBeetweenShots << endl;
    file << dash << endl;
    file << fly << endl;
  
    file.close();
}

void loadGame(player& player, gameWorld& world, RenderWindow& window, int& eliminations, int& playerHp, int& playerLvl, int& playerDamage, int& playerVelocity, int& gold, float& minTimeBeetweenShots, bool dash, bool fly)
{
    string save = "resources/save.txt";
    ifstream file(save);
    if (!file.is_open())
    {
        cout << "Nie mozna otworzyc pliku";
    }

    file >> eliminations;
    file >> playerHp;
    file >> playerLvl;
    file >> playerDamage;
    file >> playerVelocity;
    file >> gold;
    file >> minTimeBeetweenShots;
    file >> dash;
    file >> fly;

    eliminations = eliminations;
    player.setHp(playerHp, window);
    player.setLvl(playerLvl);
    player.setDamage(playerDamage);
    player.setVelocity(playerVelocity);
    player.setGold(gold);
    minTimeBeetweenShots = minTimeBeetweenShots;

    world.setMapLayout(1);
    Vector2f center((world.mapWidth / 2.0f) - 200, (world.mapHeight / 2.0f) + 100);
    player.setPosition(center);

    file.close();
}

void calculateTileRange(const FloatRect& bounds, int tileSize, int numRows, int numCols, int& startRow, int& endRow, int& startCol, int& endCol) {
    startRow = max(0, static_cast<int>(bounds.top) / tileSize);
    endRow = min(numRows - 1, static_cast<int>(bounds.top + bounds.height) / tileSize);
    startCol = max(0, static_cast<int>(bounds.left) / tileSize);
    endCol = min(numCols - 1, static_cast<int>(bounds.left + bounds.width) / tileSize);
}

int main()
{

    srand(time(NULL));

    //okno
    RenderWindow window(VideoMode(1920, 1080), "DungeonExplorer", Style::Fullscreen);
    window.setFramerateLimit(60);

    View view(Vector2f(0.f, 0.f), Vector2f(1920.f, 1080.f));
    showStartScreen(window);

    //mapa
    gameWorld world;
    world.initializeLevelMatrix();
    world.setMapLayout(1);

    //gracz 1
    Texture playerTexture1, playerTextureAttack;
    if (!playerTexture1.loadFromFile("resources/textures/player/player/Idle.png")) {
        return -1;
    }
    if (!playerTextureAttack.loadFromFile("resources/textures/player/player/Attack.png")) {
        return -1;
    }
    
    animation playerAnimation1(&playerTexture1, Vector2u(10, 1), 0.1);
    player player1(Vector2f(800, 500), Vector2f(3000, 250), 1, 5, 5, 10, playerTexture1, playerAnimation1);
    Vector2f center1((world.mapWidth / 2.0f) - 150, (world.mapHeight / 2.0f) - 120);
    player1.setPosition(center1);
    player1.setDefault();
    player1.setGold(0);

    
    //gracz 2
    Texture playerTexture2;
    if (!playerTexture2.loadFromFile("resources/textures/player/player/Idle.png")) {
        return -1;
    }
    animation playerAnimation2(&playerTexture2, Vector2u(10, 1), 0.3f);
    player player2(Vector2f(500, 500), Vector2f(2000, 200), 1, 5, 5, 50, playerTexture2, playerAnimation2);
    Vector2f center((world.mapWidth / 2.0f) - 150, (world.mapHeight / 2.0f) - 120);
    player2.setPosition(center);
    player2.setDefault();

    float deltaTime = 0.0f;
    Clock clock;
    Clock gameClock;
    Clock textClock;
    float gameTimeSeconds = 0.0f;

    Texture textureWings;    
    if (!textureWings.loadFromFile("resources/textures/player/player/wings.png")) {
        cout << "Blad ladowania skrzydel";
    }
    Sprite spriteWings(textureWings);
    spriteWings.setScale(0.3f, 0.3f);


    //przeciwnicy
    vector<enemy*>enemies;

    //projectile przeciwnikow
    vector<projectileEnemy>projectilesEnemy;
    Clock projectileEnemyClock;
    float projectileEnemyInterval;
       
    //objekty na mapie
    vector<object*> objects;

    //itemy na mapie
    vector<item*> items;
    int itemsHpQuantity = 1;
    int itemsGoldQuantity = 5;
    int randomItemHp = rand() % itemsHpQuantity;
    int randomItemGold = rand() % itemsGoldQuantity;

    //pocisk
    Texture projectileTexture;
    if (!projectileTexture.loadFromFile("resources/textures/player/player/projectileMoving.png"))
    {
        return -1;
    }
    animation projectileAnimation(&projectileTexture, Vector2u(4, 1), 0.3f);
    vector<projectile> projectiles;
    Clock shootingClock;
    Clock shootingClock2;
    float minTimeBetweenShots = 1.f;


    //czcionka
    Font font;
    if (!font.loadFromFile("resources/fonts/arial.ttf")) {
        cout << "Blad ladowania czczionki";
    }

    Font fontPixel;
    if (!fontPixel.loadFromFile("resources/fonts/minecraft.ttf")) {
        cout << "Blad ladowania czczionki";
    }


    //ikony statystyk
    

    Texture textureDamage;
    if (!textureDamage.loadFromFile("resources/textures/icons/damage.png")) {
        cout << "Blad ladowania ikony damage";
    }
    Sprite spriteDamage(textureDamage);
    spriteDamage.setPosition(1126, 10);
    spriteDamage.setScale(5.f, 5.f);
    spriteDamage.setColor(Color(200, 200, 200));

    Texture textureSpeed;
    if (!textureSpeed.loadFromFile("resources/textures/icons/speed.png")) {
        cout << "Blad ladowania ikony speed";
    }
    Sprite spriteSpeed(textureSpeed);
    spriteSpeed.setPosition(1324, 15);
    spriteSpeed.setScale(5.f, 5.f);
    spriteSpeed.setColor(Color(200, 200, 200));

    Texture textureFireRate;
    if (!textureFireRate.loadFromFile("resources/textures/icons/fireRate.png")) {
        cout << "Blad ladowania ikony fireRate";
    }
    Sprite spriteFireRate(textureFireRate);
    spriteFireRate.setPosition(1526, 36);
    spriteFireRate.setScale(0.1f, 0.1f);
    spriteFireRate.setColor(Color(200, 200, 200));

    Texture textureGold;
    if (!textureGold.loadFromFile("resources/textures/icons/gold.png")) {
        cout << "Blad ladowania ikony gold";
    }
    Sprite spriteGold(textureGold);
    spriteGold.setPosition(617, 14);
    spriteGold.setScale(0.15f, 0.15f);
    spriteGold.setColor(Color(255, 215, 0));

    Texture pauseBackgroundTexture;
    if (!pauseBackgroundTexture.loadFromFile("resources/textures/pause.png")) {
        cout << "Blad ladowania pauzy";
    }
    Sprite pauseBackground(pauseBackgroundTexture);
    pauseBackground.setColor(Color(255, 255, 255, 50));
    Clock pauseClock;

    Texture miniMapFrameTexture;
    if (!miniMapFrameTexture.loadFromFile("resources/textures/icons/miniMap/miniMapFrame.png"))
    {
        cout << "Brak ladowania ramki mapy";
    }
    Sprite spriteMiniMapFrame(miniMapFrameTexture);
    spriteMiniMapFrame.setScale(0.68f, 0.68f);
    spriteMiniMapFrame.setPosition(627.f, 165.f);


    //dzwiek
    SoundBuffer walkBuffer;
    walkBuffer.loadFromFile("resources/sounds/walk.wav");
    Sound walk;
    walk.setBuffer(walkBuffer);

    SoundBuffer hitBuffer;
    hitBuffer.loadFromFile("resources/sounds/hit.wav");
    Sound hit;
    hit.setBuffer(hitBuffer);

    SoundBuffer shotBuffer;
    shotBuffer.loadFromFile("resources/sounds/shot.wav");
    Sound shot;
    shot.setBuffer(shotBuffer);
    shot.setVolume(30.f);

    SoundBuffer doorBuffer;
    doorBuffer.loadFromFile("resources/sounds/door.wav");
    Sound door;
    door.setBuffer(doorBuffer);

    SoundBuffer damageTakenBuffer;
    damageTakenBuffer.loadFromFile("resources/sounds/damageTaken.wav");
    Sound damageTaken;
    damageTaken.setBuffer(damageTakenBuffer);

    SoundBuffer enemyDeathBuffer;
    enemyDeathBuffer.loadFromFile("resources/sounds/enemyDeath.wav");
    Sound enemyDeath;
    enemyDeath.setBuffer(enemyDeathBuffer);
    enemyDeath.setVolume(50.f);

    Music music;
    music.openFromFile("resources/sounds/music.wav");
    music.play();
    music.setLoop(true);
    music.setVolume(20.f);


    //gameover
    RectangleShape pauseRectangle(Vector2f(window.getSize().x, window.getSize().y));
    Color pauseColor = Color::Black;
    pauseColor.a = 200;
    pauseRectangle.setFillColor(pauseColor);
    

    while (window.isOpen())
    {
        Event event;
        while (window.pollEvent(event))
        {
            if (event.type == Event::Closed)
            {
                window.close();
            }
            if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape) {
                isPaused = !isPaused;
            }
        }

        int playerHp = player1.getHp();
        int playerLvl = player1.getLvl();
        int playerDamage = player1.getDamage();
        int playerVelocity = player1.getVelocity();
        int playerGold = player1.getGold();
        player2.setPosition(Vector2f(-1000, -1000));

        if(!isPaused)
        {
        window.clear();
        deltaTime = clock.restart().asSeconds();

        //kamera
        float halfWidth = view.getSize().x / 2.f;
        float halfHeight = view.getSize().y / 2.f;
        Vector2f cameraPosition = player1.getPosition() + Vector2f(200.f,200.f);

        if (cameraPosition.x - halfWidth < 0)
        {
            cameraPosition.x = halfWidth;
        }
        if (cameraPosition.y - halfHeight < 0)
        {
            cameraPosition.y = halfHeight;
        }
        if (cameraPosition.x + halfWidth > world.mapWidth)
        {
            cameraPosition.x = world.mapWidth - halfWidth;
        }
        if (cameraPosition.y + halfHeight > world.mapHeight)
        {
            cameraPosition.y = world.mapHeight - halfHeight;
        }

        view.setCenter(cameraPosition);
        window.setView(view);
        
        //tekst na oknie
        Text textMapa;
        textMapa.setFont(fontPixel);
        textMapa.setString("Mapa: " + to_string(world.getMap()));
        textMapa.setCharacterSize(24);
        textMapa.setFillColor(Color::White);
        textMapa.setPosition(10, 10);

        Text textDamage;
        textDamage.setFont(fontPixel);
        textDamage.setString(to_string(player1.getDamage()));
        textDamage.setCharacterSize(50);
        textDamage.setFillColor(Color::White);
        textDamage.setPosition(1227, 17);

        Text textSpeed;
        textSpeed.setFont(fontPixel);
        textSpeed.setString(to_string(player1.getVelocity()));
        textSpeed.setCharacterSize(50);
        textSpeed.setFillColor(Color::White);
        textSpeed.setPosition(1427, 17);

        Text textFireRate;
        textFireRate.setFont(fontPixel);
        textFireRate.setString(to_string(int(10 / minTimeBetweenShots)));
        textFireRate.setCharacterSize(50);
        textFireRate.setFillColor(Color::White);
        textFireRate.setPosition(1627, 17);

        Text textGold;
        textGold.setFont(fontPixel);
        textGold.setString(to_string(player1.getGold()));
        textGold.setCharacterSize(50);
        textGold.setFillColor(Color::White);
        textGold.setPosition(720, 17);

        Text textMap;
        textMap.setFont(fontPixel);
        textMap.setString(to_string(world.getMap()));
        textMap.setCharacterSize(25);
        textMap.setFillColor(Color::White);
        textMap.setPosition(1890, 165);

        Text gameTimeText;
        gameTimeText.setFont(fontPixel);
        gameTimeText.setCharacterSize(39);
        gameTimeText.setFillColor(Color::White);
        gameTimeText.setPosition(912, 23);
        gameTimeSeconds = gameClock.getElapsedTime().asSeconds();

        int minutes = static_cast<int>(gameTimeSeconds) / 60;
        int seconds = static_cast<int>(gameTimeSeconds) % 60;
        string gameTimeString = "";
        if (minutes < 10)
        {
            gameTimeString += "0";
        }
        gameTimeString += to_string(minutes) + ":";

        if (seconds < 10)
        {
            gameTimeString += "0";
        }
        gameTimeString += to_string(seconds);
        gameTimeText.setString(gameTimeString);


        //rysowanie mapy
        for (int i = 0; i < world.getNumRows(); ++i) {
            for (int j = 0; j < world.getNumCols(); ++j) {
                window.draw(world.getTiles()[i][j]->sprite);
            }
        }

        

        //tutorial
        if (world.getMap() == 1)
        {
            Text textMove;
            textMove.setFont(fontPixel);
            textMove.setString("TO MOVE");
            textMove.setCharacterSize(50);
            textMove.setFillColor(Color::White);
            textMove.setPosition(535, 800);
            window.draw(textMove);

            Text textShot;
            textShot.setFont(fontPixel);
            textShot.setString("TO SHOOT");
            textShot.setCharacterSize(50);
            textShot.setFillColor(Color::White);
            textShot.setPosition(1130, 800);
            window.draw(textShot);

            Texture textureWsad;
            if (!textureWsad.loadFromFile("resources/textures/keys/wsad.png")) {
                cout << "Blad ladowania wsad";
            }
            Sprite spriteWsad(textureWsad);
            spriteWsad.setPosition(498, 580);
            spriteWsad.setScale(0.63f, 0.63f);
            window.draw(spriteWsad);

            Texture textureArrows;
            if (!textureArrows.loadFromFile("resources/textures/keys/arrows.png")) {
                cout << "Blad ladowania arrows";
            }
            Sprite spriteArrows(textureArrows);
            spriteArrows.setPosition(1105, 582);
            spriteArrows.setScale(0.63f, 0.63f);
            window.draw(spriteArrows);


        }

        //spawny na mapach
        //MAPA 2
        if (world.getMap() == 2)
        {
            //dodanie ghost
            if (!world.getSpawnLimitReached(2))
            {
                enemies.push_back(new enemyGhost(Vector2f(500.f, 200.f), Vector2f(0.f, 0.f), (rand() % 21 + 15) * player1.getLvl()));
                enemies.push_back(new enemyGhost(Vector2f(1400.f, 200.f), Vector2f(0.f, 0.f), (rand() % 21 + 15) * player1.getLvl()));
                enemies.push_back(new enemyGhost(Vector2f(500.f, 890.f), Vector2f(0.f, 0.f), (rand() % 21 + 15) * player1.getLvl()));
                enemies.push_back(new enemyGhost(Vector2f(1400.f, 890.f), Vector2f(0.f, 0.f), (rand() % 21 + 15) * player1.getLvl()));
                if (enemies.size() >= 4) 
                {
                    world.setSpawnLimitReached(2, true);
                }
            }

            //dodawanie obiektow
            if (!world.getObjectLimitReached(2))
            {
                objects.push_back(new objectChest(Vector2f(555, 493)));
                objects.push_back(new objectChest(Vector2f(1265, 493)));
                if (objects.size() >= 3)
                {
                    world.setObjectLimitReached(2, true);
                }
            }
        }

        //MAPA 3
        else if (world.getMap() == 3)
        {
            //dodanie zombie
            if (!world.getSpawnLimitReached(3))
            {
                enemies.push_back(new enemyZombie(Vector2f(500.f, 200.f), Vector2f(0.f, 0.f), (rand() % 21 + 15) * player1.getLvl()));
                enemies.push_back(new enemyZombie(Vector2f(1400.f, 200.f), Vector2f(0.f, 0.f), (rand() % 21 + 15) * player1.getLvl()));
                enemies.push_back(new enemyZombie(Vector2f(500.f, 890.f), Vector2f(0.f, 0.f), (rand() % 21 + 15) * player1.getLvl()));
                enemies.push_back(new enemyZombie(Vector2f(1400.f, 890.f), Vector2f(0.f, 0.f), (rand() % 21 + 15) * player1.getLvl()));
                if (enemies.size() >= 4) {
                    world.setSpawnLimitReached(3, true);
                }
            }
            if (!world.getObjectLimitReached(3))
            {
                objects.push_back(new objectChest(Vector2f(911, 493)));
                if (objects.size() >= 1)
                {
                    world.setObjectLimitReached(3, true);
                }
            }
        }

        //MAPA 4
        else if (world.getMap() == 4)
        {
            //dodanie robot
            if (!world.getSpawnLimitReached(4))
            {
                enemies.push_back(new enemyRobot(Vector2f(500.f, 700.f), Vector2f(-1.f, -1.f), (rand() % 21 + 15) * player1.getLvl()));
                enemies.push_back(new enemyRobot(Vector2f(1400.f, 500.f), Vector2f(1.f, 1.f), (rand() % 21 + 15) * player1.getLvl()));
                enemies.push_back(new enemyRobot(Vector2f(500.f, 890.f), Vector2f(-1.f, -1.f), (rand() % 21 + 15) * player1.getLvl()));
                enemies.push_back(new enemyRobot(Vector2f(1400.f, 890.f), Vector2f(1.f, 1.f), (rand() % 21 + 15) * player1.getLvl()));
                if (enemies.size() >= 4) {
                    world.setSpawnLimitReached(4, true);
                }
            }

            //dodawanie obiektow
            if (!world.getObjectLimitReached(4))
            {
                objects.push_back(new objectChest(Vector2f(1520, 200)));
                if (objects.size() >= 1)
                {
                    world.setObjectLimitReached(4, true);
                }
            }
        }

        //MAPA 5
        else if (world.getMap() == 5)
        {
            //dodanie ghost
            if (!world.getSpawnLimitReached(5))
            {
                enemies.push_back(new enemyGhost(Vector2f(500.f, 200.f), Vector2f(0.f, 0.f), (rand() % 21 + 15) * player1.getLvl()));
                enemies.push_back(new enemyZombie(Vector2f(1400.f, 890.f), Vector2f(0.f, 0.f), (rand() % 21 + 15)* player1.getLvl()));
                if (enemies.size() >= 2)
                {
                    world.setSpawnLimitReached(5, true);
                }
            }
            if (!world.getObjectLimitReached(5))
            {
                objects.push_back(new objectChest(Vector2f(912, 491)));
                if (objects.size() >= 1)
                {
                    world.setObjectLimitReached(5, true);
                }
            }
        }

        //MAPA 6
        else if (world.getMap() == 6)
        {
            //dodanie ghost
            if (!world.getSpawnLimitReached(6))
            {
                enemies.push_back(new enemyGhost(Vector2f(500.f, 200.f), Vector2f(0.f, 0.f), (rand() % 21 + 15) * player1.getLvl()));
                enemies.push_back(new enemyZombie(Vector2f(1000.f, 590.f), Vector2f(0.f, 0.f), (rand() % 21 + 15)* player1.getLvl()));
                enemies.push_back(new enemyZombie(Vector2f(1400.f, 890.f), Vector2f(0.f, 0.f), (rand() % 21 + 15)* player1.getLvl()));
                if (enemies.size() >= 3)
                {
                    world.setSpawnLimitReached(6, true);
                }
            }

            //dodawanie obiektow
            if (!world.getObjectLimitReached(6))
            {
                objects.push_back(new objectChest(Vector2f(710, 462)));
                objects.push_back(new objectChest(Vector2f(3034, 462)));
                if (objects.size() >= 2)
                {
                    world.setObjectLimitReached(6, true);
                }
            }
        }

        //MAPA 7
        else if (world.getMap() == 7)
        {
            //dodanie ghost
            if (!world.getSpawnLimitReached(7))
            {
                enemies.push_back(new enemyGhost(Vector2f(500.f, 200.f), Vector2f(0.f, 0.f), (rand() % 21 + 15) * player1.getLvl()));
                enemies.push_back(new enemyGhost(Vector2f(1400.f, 200.f), Vector2f(0.f, 0.f), (rand() % 21 + 15) * player1.getLvl()));
                enemies.push_back(new enemyRobot(Vector2f(500.f, 890.f), Vector2f(-1.f, -1.f), (rand() % 21 + 15)* player1.getLvl()));
                enemies.push_back(new enemyRobot(Vector2f(1400.f, 890.f), Vector2f(1.f, 1.f), (rand() % 21 + 15)* player1.getLvl()));
                if (enemies.size() >= 4)
                {
                    world.setSpawnLimitReached(7, true);
                }
            }

            //dodawanie obiektow
            if (!world.getObjectLimitReached(7))
            {
                objects.push_back(new objectChest(Vector2f(205, 200)));
                objects.push_back(new objectMedKit(Vector2f(1617, 205)));
                objects.push_back(new objectChest(Vector2f(1619, 1870)));
                objects.push_back(new objectMedKit(Vector2f(203, 1875)));
                if (objects.size() >= 4)
                {
                    world.setObjectLimitReached(7, true);
                }
            }
        }

        //special room 1
        else if (world.getMap() == 9)
        {
            if (!world.getObjectLimitReached(9))
            {
                objects.push_back(new objectChest(Vector2f(608, 200)));
                objects.push_back(new objectChest(Vector2f(1214, 200)));
                objects.push_back(new objectChest(Vector2f(608, 790)));
                objects.push_back(new objectChest(Vector2f(1214, 790)));

                objects.push_back(new objectMedKit(Vector2f(708, 502)));
                objects.push_back(new objectMedKit(Vector2f(909, 502)));
                objects.push_back(new objectMedKit(Vector2f(1111, 502)));
                if (objects.size() >= 7)
                {
                    world.setObjectLimitReached(9, true);
                }
            }
        }

        //MAPA 8 BOSS
        else if (world.getMap() == 8)
        {
            //dodanie slime 1 do wektora
            if (!world.getSpawnLimitReached(8))
            {
                enemies.push_back(new enemySlime(Vector2f(rand() % (3590 - 250 + 1) + 250, rand() % (1910 - 250 + 1) + 250), Vector2f(1.f,1.f), Vector2f(0.7f, 0.7f), (rand() % 21 + 15) * player1.getLvl()));
                if (enemies.size() >= 1)
                {
                    world.setSpawnLimitReached(8, true);
                }
            }
            
            if (!enemies.empty())
            {
                if (enemies[0]->getHP() <= 0)
                {
                    //dodanie dwoch nastepnych
                    Vector2f previousPosition = enemies[0]->getPosition();
                    if (!spawnLimitReached1)
                    {
                        enemies.push_back(new enemySlime(Vector2f(rand() % (3590 - 250 + 1) + 250, rand() % (1910 - 250 + 1) + 250), Vector2f(1.f, 1.f), Vector2f(0.6f, 0.6f), (rand() % 21 + 15) * player1.getLvl()));
                        enemies.push_back(new enemySlime(Vector2f(rand() % (3590 - 250 + 1) + 250, rand() % (1910 - 250 + 1) + 250), Vector2f(-1.f, -1.f), Vector2f(0.6f, 0.6f), (rand() % 21 + 15) * player1.getLvl()));

                        if (enemies.size() >= 3)
                        {
                            spawnLimitReached1 = true;
                        }
                    }

                    if (enemies[1]->getHP() <= 0 || enemies[2]->getHP() <= 0)
                    {
                        //dodanie dwoch nastepnycha
                        if (!spawnLimitReached2)
                        {
                            enemies.push_back(new enemySlime(Vector2f(rand() % (3590 - 250 + 1) + 250, rand() % (1910 - 250 + 1) + 250), Vector2f(1.f, 1.f), Vector2f(0.5f, 0.5f), (rand() % 21 + 15) * player1.getLvl()));
                            enemies.push_back(new enemySlime(Vector2f(rand() % (3590 - 250 + 1) + 250, rand() % (1910 - 250 + 1) + 250), Vector2f(-1.f, -1.f), Vector2f(0.5f, 0.5f), (rand() % 21 + 15) * player1.getLvl()));

                            if (enemies.size() >= 7)
                            {
                                spawnLimitReached2 = true;
                            }
                        }
                        if (enemies[3]->getHP() <= 0 || enemies[4]->getHP() <= 0)
                        {
                            //dodanie dwoch nastepnycha
                            if (!spawnLimitReached3)
                            {
                                enemies.push_back(new enemySlime(Vector2f(rand() % (3590 - 250 + 1) + 250, rand() % (1910 - 250 + 1) + 250), Vector2f(1.f, 1.f), Vector2f(0.4f, 0.4f), (rand() % 21 + 15) * player1.getLvl()));
                                enemies.push_back(new enemySlime(Vector2f(rand() % (3590 - 250 + 1) + 250, rand() % (1910 - 250 + 1) + 250), Vector2f(-1.f, -1.f), Vector2f(0.4f, 0.4f), (rand() % 21 + 15) * player1.getLvl()));

                                if (enemies.size() >= 15)
                                {
                                    spawnLimitReached3 = true;
                                }
                            }
                        }
                    }

                    else if (enemies[1]->getHP() <= 0 || enemies[2]->getHP() <= 0)
                    {
                        if (!spawnLimitReached2)
                        {
                            enemies.push_back(new enemySlime(Vector2f(rand() % (3590 - 250 + 1) + 250, rand() % (1910 - 250 + 1) + 250), Vector2f(1.f, 1.f), Vector2f(0.5f, 0.5f), (rand() % 21 + 15) * player1.getLvl()));
                            enemies.push_back(new enemySlime(Vector2f(rand() % (3590 - 250 + 1) + 250, rand() % (1910 - 250 + 1) + 250), Vector2f(-1.f, -1.f), Vector2f(0.5f, 0.5f), (rand() % 21 + 15) * player1.getLvl()));

                            if (enemies.size() >= 7)
                            {
                                spawnLimitReached2 = true;
                            }
                        }
                        if (enemies[5]->getHP() <= 0 || enemies[6]->getHP() <= 0)
                        {
                            //dodanie dwoch nastepnycha
                            if (!spawnLimitReached3)
                            {
                                enemies.push_back(new enemySlime(Vector2f(rand() % (3590 - 250 + 1) + 250, rand() % (1910 - 250 + 1) + 250), Vector2f(1.f, 1.f), Vector2f(0.4f, 0.4f), (rand() % 21 + 15) * player1.getLvl()));
                                enemies.push_back(new enemySlime(Vector2f(rand() % (3590 - 250 + 1) + 250, rand() % (1910 - 250 + 1) + 250), Vector2f(-1.f, -1.f), Vector2f(0.4f, 0.4f), (rand() % 21 + 15) * player1.getLvl()));

                                if (enemies.size() >= 15)
                                {
                                    spawnLimitReached3 = true;
                                }
                            }
                        }
                    }
                }
            }

            bool allSlimesDead = true;
            for (int i = 0; i < enemies.size(); ++i)
            {
                if (enemies[i]->getHP() > 0)
                {
                    allSlimesDead = false;
                    break;
                }
            }

            if (!world.getObjectLimitReached(8))
            {
                objects.push_back(new objectTrapdoor(Vector2f(1819, 393), world));
                if (objects.size() >= 1)
                {
                    world.setObjectLimitReached(8, true);
                }
            }

            if (allSlimesDead) {
                View currentView = window.getView();
                window.setView(window.getDefaultView());
                player1.giveItem(1);
                Text textDash;
                textDash.setFont(fontPixel);
                textDash.setString("UNLOCKED DASHING, PRESS SHIFT");
                textDash.setCharacterSize(50);
                textDash.setFillColor(Color::White);
                textDash.setPosition(520, 100);
                window.draw(textDash);
                window.setView(currentView);
                if (!objects.empty()) {
                    objects[0]->openTrapdoor();
                }
            }

            if (enemies.empty())
            {
                objects.push_back(new objectTrapdoor(Vector2f(1819, 393), world));
                objects.back()->openTrapdoor();
            }
        }

        else if (world.getMap() == 10)
        {
            projectileEnemyInterval = 0.7f;
            //dodanie dragon
            if (!world.getSpawnLimitReached(10))
            {
                enemies.push_back(new enemyDragon(Vector2f(player1.getPosition().x, player1.getPosition().y - 500), Vector2f(0.f, 0.f), 10 * player1.getLvl() * player1.getLvl()));
                if (enemies.size() >= 1)
                {
                    world.setSpawnLimitReached(10, true);
                }
            }
            
            if (!enemies.empty())
            {
                if (projectileEnemyClock.getElapsedTime().asSeconds() >= projectileEnemyInterval) {
                    enemies.push_back(new projectileEnemy(enemies[0]->getPosition(), Vector2f(0.f, 0.f), 1));
                    projectileEnemyClock.restart();
                }
            }
            if (!world.getObjectLimitReached(10))
            {
                objects.push_back(new objectTrapdoor(Vector2f(1819, 393), world));
                if (objects.size() >= 1)
                {
                    world.setObjectLimitReached(10, true);
                }
            }
            
            if (!enemies.empty())
            {
                if (enemies[0]->getHP() < 0)
                {
                    if (giveLevel == true)
                    {
                        player1.setLvl(player1.getLvl() + 1);
                        player1.giveItem(0);
                    }
                    View currentView = window.getView();
                    window.setView(window.getDefaultView());
                    Text textFlying;
                    textFlying.setFont(fontPixel);
                    textFlying.setString("UNLOCKED FLYING");
                    textFlying.setCharacterSize(50);
                    textFlying.setFillColor(Color::White);
                    textFlying.setPosition(720, 100);
                    window.draw(textFlying);
                    window.setView(currentView);
                    giveLevel = false;

                    if (!objects.empty()) {
                        objects[0]->openTrapdoor();
                        if (objects[0]->checkCollision(player1, window))
                        {

                        }
                    }
                }
            }
            if (enemies.empty())
            {
                objects.push_back(new objectTrapdoor(Vector2f(1819, 393), world));
                objects.back()->openTrapdoor();
            }
        }

        else if (world.getMap() == 11)
        {
            projectileEnemyInterval = 2.f;
            if (!world.getSpawnLimitReached(11))
            {
                enemies.push_back(new enemyMage(Vector2f(3270.f, 390.f), Vector2f(0.f, 0.f), (rand() % 21 + 15)* player1.getLvl()));
                enemies.push_back(new enemyMage(Vector2f(3370.f, 1660.f), Vector2f(0.f, 0.f), (rand() % 21 + 15)* player1.getLvl()));
                enemies.push_back(new enemyMage(Vector2f(2260.f, 1660.f), Vector2f(0.f, 0.f), (rand() % 21 + 15)* player1.getLvl()));

                enemies.push_back(new enemyRobot(Vector2f(3000.f, 1500.f), Vector2f(1.f, 1.f), (rand() % 21 + 15)* player1.getLvl()));
                enemies.push_back(new enemyRobot(Vector2f(1500.f, 500.f), Vector2f(-1.f, -1.f), (rand() % 21 + 15)* player1.getLvl()));
                if (enemies.size() >= 5)
                {
                    world.setSpawnLimitReached(11, true);
                }
            }

            if (!enemies.empty())
            {
                if (projectileEnemyClock.getElapsedTime().asSeconds() >= projectileEnemyInterval) {
                    enemies.push_back(new projectileEnemy(enemies[0]->getPosition(), Vector2f(0.f, 0.f), 1));
                    enemies.push_back(new projectileEnemy(enemies[1]->getPosition(), Vector2f(0.f, 0.f), 1));
                    enemies.push_back(new projectileEnemy(enemies[2]->getPosition(), Vector2f(0.f, 0.f), 1));
                    projectileEnemyClock.restart();
                }
            }
        }

        else if (world.getMap() == 12)
        {
            if (!world.getObjectLimitReached(12))
            {
                switch (randomItemHp)
                {
                case 0:
                    items.push_back(new itemHealing(Vector2f(window.getSize().x / 2 - 50, window.getSize().y / 2 - 50)));
                    break;
                }
                if (items.size() >= 1)
                {
                    world.setObjectLimitReached(12, true);
                }
            }
        }

        else if (world.getMap() == 13)
        {
            if (!world.getObjectLimitReached(13))
            {
                switch (randomItemGold)
                {
                case 0:
                    items.push_back(new itemTripleShot(Vector2f(window.getSize().x / 2 - 50, window.getSize().y / 2 - 50)));
                    break;
                case 1:
                    items.push_back(new itemSpectralShot(Vector2f(window.getSize().x / 2 - 50, window.getSize().y / 2 - 50)));
                    break;
                case 2:
                    items.push_back(new itemPenetrateShot(Vector2f(window.getSize().x / 2 - 50, window.getSize().y / 2 - 50)));
                    break;
                case 3:
                    items.push_back(new itemBouncingShot(Vector2f(window.getSize().x / 2 - 50, window.getSize().y / 2 - 50)));
                    break;
                case 4:
                    items.push_back(new itemBackShot(Vector2f(window.getSize().x / 2 - 50, window.getSize().y / 2 - 50)));
                    break;
                }
                if (items.size() >= 1)
                {
                    world.setObjectLimitReached(13, true);
                }
            }
            }

        else
        {
            enemies.clear();
            objects.clear();
            items.clear();
        }


        Vector2f movement(0.f, 0.f);
        Vector2f previousPlayerPosition = player1.getPosition();
        Vector2f movement2(0.f, 0.f);
        Vector2f previousPlayerPosition2 = player2.getPosition();

        //movement gracza
        if (player1.getItem(1))
        {
            if (Keyboard::isKeyPressed(Keyboard::LShift) && canDash) {
                if (Keyboard::isKeyPressed(Keyboard::A) && Keyboard::isKeyPressed(Keyboard::W)) {
                    movement.x -= 200.f;
                    movement.y -= 200.f;
                    canDash = false;
                }
                else if (Keyboard::isKeyPressed(Keyboard::A) && Keyboard::isKeyPressed(Keyboard::S)) {
                    movement.x -= 200.f;
                    movement.y += 200.f;
                    canDash = false;
                }
                else if (Keyboard::isKeyPressed(Keyboard::D) && Keyboard::isKeyPressed(Keyboard::W)) {
                    movement.x += 200.f;
                    movement.y -= 200.f;
                    canDash = false;
                }
                else if (Keyboard::isKeyPressed(Keyboard::D) && Keyboard::isKeyPressed(Keyboard::S)) {
                    movement.x += 200.f;
                    movement.y += 200.f;
                    canDash = false;
                }
                else if (Keyboard::isKeyPressed(Keyboard::A)) {
                    movement.x -= 200.f;
                    canDash = false;
                }
                else if (Keyboard::isKeyPressed(Keyboard::D)) {
                    movement.x += 200.f;
                    canDash = false;
                }
                else if (Keyboard::isKeyPressed(Keyboard::W)) {
                    movement.y -= 200.f;
                    canDash = false;
                }
                else if (Keyboard::isKeyPressed(Keyboard::S)) {
                    movement.y += 200.f;
                    canDash = false;
                }
            }

            if (!Keyboard::isKeyPressed(Keyboard::LShift)) {
                canDash = true;
            }
        }

        // Normalny ruch
        if (Keyboard::isKeyPressed(Keyboard::A) && !isWalking) {
            isWalking = true;
            walk.play();
        }

        if (Keyboard::isKeyPressed(Keyboard::D) && !isWalking) {
            isWalking = true;
            walk.play(); 
        }

        if (Keyboard::isKeyPressed(Keyboard::W) && !isWalking) {
            isWalking = true;
            walk.play();
        }

        if (Keyboard::isKeyPressed(Keyboard::S) && !isWalking) {
            isWalking = true;
            walk.play();
        }

        if (isWalking) {
            if (walk.getStatus() == Sound::Playing) {

                if (Keyboard::isKeyPressed(Keyboard::A)) {
                    levelDirection = 1;
                    movement.x -= playerVelocity;
                    playerTexture1.loadFromFile("resources/textures/player/player/Runleft.png");
                    //playerAnimation1.changeTexture(&playerTexture1, Vector2u(8, 1), 0.1);
                }
                if (Keyboard::isKeyPressed(Keyboard::D)) {
                    levelDirection = 0;
                    movement.x += playerVelocity;
                    playerTexture1.loadFromFile("resources/textures/player/player/Runright.png");
                    //playerAnimation1.changeTexture(&playerTexture1, Vector2u(8, 1), 0.1);
                }
                if (Keyboard::isKeyPressed(Keyboard::W)) {
                    levelDirection = 2;
                    movement.y -= playerVelocity;
                }
                if (Keyboard::isKeyPressed(Keyboard::S)) {
                    levelDirection = 3;
                    movement.y += playerVelocity;
                }
                if (!Keyboard::isKeyPressed(Keyboard::A) && !Keyboard::isKeyPressed(Keyboard::D) && !Keyboard::isKeyPressed(Keyboard::W) && !Keyboard::isKeyPressed(Keyboard::S))
                {
                    playerTexture1.loadFromFile("resources/textures/player/player/Idle.png");
                    playerAnimation1.changeTexture(&playerTexture1, Vector2u(10, 1), 0.1);
                }
                player1.setPosition(player1.getPosition() + movement);
            }
            else {
                isWalking = false;  
            }
        }
        
        Vector2f playerPosition = player1.getPosition();


        //wystrzelenie pocisku
        if (event.type == Event::KeyPressed)
        {
            if (shootingClock.getElapsedTime().asSeconds() >= minTimeBetweenShots)
            {
                if (event.key.code == Keyboard::Up)
                {
                    shot.play();
                    projectiles.push_back(projectile(playerPosition.x + 1105 , playerPosition.y + 150, 'U', 10, 270.f, projectileTexture, projectileAnimation, 0));
                    if (player1.getItem(7))
                    {
                        projectiles.push_back(projectile(playerPosition.x + -660, playerPosition.y + 70, 'D', 10, 90.f, projectileTexture, projectileAnimation, 0));
                    }
                    if (player1.getItem(3))
                    {
                        projectiles.push_back(projectile(playerPosition.x + 1105, playerPosition.y + 150, 'U', 10, 270.f, projectileTexture, projectileAnimation, 1));
                        projectiles.push_back(projectile(playerPosition.x + 1105, playerPosition.y + 150, 'U', 10, 270.f, projectileTexture, projectileAnimation, 2));
                    }
                    playerTexture1.loadFromFile("resources/textures/player/player/Attack.png");
                    playerAnimation1.changeTexture(&playerTexture1, Vector2u(13, 1), minTimeBetweenShots / 13);
                }
                else if (event.key.code == Keyboard::Down)
                {
                    shot.play();
                    projectiles.push_back(projectile(playerPosition.x + -660, playerPosition.y + 70, 'D', 10, 90.f, projectileTexture, projectileAnimation, 0));
                    if (player1.getItem(7))
                    {
                        projectiles.push_back(projectile(playerPosition.x + 1105, playerPosition.y + 150, 'U', 10, 270.f, projectileTexture, projectileAnimation, 0));
                    }
                    if (player1.getItem(3))
                    {
                        projectiles.push_back(projectile(playerPosition.x + -660, playerPosition.y + 70, 'D', 10, 90.f, projectileTexture, projectileAnimation, 1));
                        projectiles.push_back(projectile(playerPosition.x + -660, playerPosition.y + 70, 'D', 10, 90.f, projectileTexture, projectileAnimation, 2));
                    }
                    playerTexture1.loadFromFile("resources/textures/player/player/Attack.png");
                    playerAnimation1.changeTexture(&playerTexture1, Vector2u(13, 1), minTimeBetweenShots / 13);
                }
                else if (event.key.code == Keyboard::Left)
                {
                    shot.play();
                    projectiles.push_back(projectile(playerPosition.x + -570, playerPosition.y + 65, 'L', 10, 180.f, projectileTexture, projectileAnimation, 0));
                    if (player1.getItem(7))
                    {
                        projectiles.push_back(projectile(playerPosition.x + 1050, playerPosition.y + 140, 'R', 10, 0.f, projectileTexture, projectileAnimation, 0));
                    }
                    if (player1.getItem(3))
                    {
                        projectiles.push_back(projectile(playerPosition.x + -570, playerPosition.y + 65, 'L', 10, 180.f, projectileTexture, projectileAnimation, 1));
                        projectiles.push_back(projectile(playerPosition.x + -570, playerPosition.y + 65, 'L', 10, 180.f, projectileTexture, projectileAnimation, 2));
                    }
                    playerTexture1.loadFromFile("resources/textures/player/player/Attack.png");
                    playerAnimation1.changeTexture(&playerTexture1, Vector2u(13, 1), minTimeBetweenShots / 13);
                }
                else if (event.key.code == Keyboard::Right)
                {
                    shot.play();
                    projectiles.push_back(projectile(playerPosition.x + 1050, playerPosition.y + 140, 'R', 10, 0.f, projectileTexture, projectileAnimation, 0));
                    if (player1.getItem(7))
                    {
                        projectiles.push_back(projectile(playerPosition.x + -570, playerPosition.y + 65, 'L', 10, 180.f, projectileTexture, projectileAnimation, 0));
                    }
                    if (player1.getItem(3))
                    {
                        projectiles.push_back(projectile(playerPosition.x + 1050, playerPosition.y + 140, 'R', 10, 0.f, projectileTexture, projectileAnimation, 1));
                        projectiles.push_back(projectile(playerPosition.x + 1050, playerPosition.y + 140, 'R', 10, 0.f, projectileTexture, projectileAnimation, 2));
                    }
                    playerTexture1.loadFromFile("resources/textures/player/player/Attack.png");
                    playerAnimation1.changeTexture(&playerTexture1, Vector2u(13, 1), minTimeBetweenShots / 13);
                }
                
                shootingClock.restart();
            }
        }

        if (event.type == Event::KeyReleased)
        {
            if (event.key.code == Keyboard::Up || event.key.code == Keyboard::Down || event.key.code == Keyboard::Left || event.key.code == Keyboard::Right)
            {
                playerTexture1.loadFromFile("resources/textures/player/player/Idle.png");
                playerAnimation1.changeTexture(&playerTexture1, Vector2u(10, 1), 0.1);
            }
        }

        // gracza2
        if (coop == true)
        {
            if (player2.getItem(1))
            {
                if (Keyboard::isKeyPressed(Keyboard::LShift) && canDash) {
                    if (Keyboard::isKeyPressed(Keyboard::F) && Keyboard::isKeyPressed(Keyboard::T)) {
                        movement2.x -= 200.f;
                        movement2.y -= 200.f;
                        canDash = false;
                    }
                    else if (Keyboard::isKeyPressed(Keyboard::F) && Keyboard::isKeyPressed(Keyboard::G)) {
                        movement2.x -= 200.f;
                        movement2.y += 200.f;
                        canDash = false;
                    }
                    else if (Keyboard::isKeyPressed(Keyboard::H) && Keyboard::isKeyPressed(Keyboard::T)) {
                        movement2.x += 200.f;
                        movement2.y -= 200.f;
                        canDash = false;
                    }
                    else if (Keyboard::isKeyPressed(Keyboard::H) && Keyboard::isKeyPressed(Keyboard::G)) {
                        movement2.x += 200.f;
                        movement2.y += 200.f;
                        canDash = false;
                    }
                    else if (Keyboard::isKeyPressed(Keyboard::F)) {
                        movement2.x -= 200.f;
                        canDash = false;
                    }
                    else if (Keyboard::isKeyPressed(Keyboard::H)) {
                        movement2.x += 200.f;
                        canDash = false;
                    }
                    else if (Keyboard::isKeyPressed(Keyboard::T)) {
                        movement2.y -= 200.f;
                        canDash = false;
                    }
                    else if (Keyboard::isKeyPressed(Keyboard::G)) {
                        movement2.y += 200.f;
                        canDash = false;
                    }
                }

                if (!Keyboard::isKeyPressed(Keyboard::LShift)) {
                    canDash = true;
                }
            }

            // Normalny ruch
            if (Keyboard::isKeyPressed(Keyboard::F) && !isWalking) {
                isWalking = true;
                walk.play();
            }

            if (Keyboard::isKeyPressed(Keyboard::H) && !isWalking) {
                isWalking = true;
                walk.play();
            }

            if (Keyboard::isKeyPressed(Keyboard::T) && !isWalking) {
                isWalking = true;
                walk.play();
            }

            if (Keyboard::isKeyPressed(Keyboard::G) && !isWalking) {
                isWalking = true;
                walk.play();
            }

            if (isWalking) {
                if (walk.getStatus() == Sound::Playing) {
                    if (Keyboard::isKeyPressed(Keyboard::F)) {
                        movement2.x -= playerVelocity;
                    }
                    if (Keyboard::isKeyPressed(Keyboard::H)) {
                        movement2.x += playerVelocity;
                    }
                    if (Keyboard::isKeyPressed(Keyboard::T)) {
                        movement2.y -= playerVelocity;
                    }
                    if (Keyboard::isKeyPressed(Keyboard::G)) {
                        movement2.y += playerVelocity;
                    }

                    player2.setPosition(player2.getPosition() + movement2);
                }
                else {
                    isWalking = false;
                }
            }
            Vector2f playerPosition2 = player2.getPosition();

            //wystrzelenie pocisku gracza 2
            if (shootingClock2.getElapsedTime().asSeconds() >= minTimeBetweenShots)
            {
                if (Keyboard::isKeyPressed(Keyboard::I))
                {
                    cout << 1;
                    shot.play();
                    projectiles.push_back(projectile(playerPosition2.x + 1750, playerPosition2.y + -150, 'U', 10, 270.f, projectileTexture, projectileAnimation, 0));
                }
                else if (Keyboard::isKeyPressed(Keyboard::K))
                {
                    shot.play();
                    projectiles.push_back(projectile(playerPosition2.x + -1250, playerPosition2.y + 450, 'D', 10, 90.f, projectileTexture, projectileAnimation, 0));
                }
                else if (Keyboard::isKeyPressed(Keyboard::J))
                {
                    shot.play();
                    projectiles.push_back(projectile(playerPosition2.x + -1250, playerPosition2.y + -150, 'L', 10, 180.f, projectileTexture, projectileAnimation, 0));
                }
                else if (Keyboard::isKeyPressed(Keyboard::L))
                {
                    shot.play();
                    projectiles.push_back(projectile(playerPosition2.x + 1750, playerPosition2.y + 450, 'R', 10, 0.f, projectileTexture, projectileAnimation, 0));
                }
                shootingClock2.restart();
            }
        }
        Vector2f playerPosition2 = player2.getPosition();


        for (auto& proj : projectiles)
        {
            proj.update();
            proj.updateAnimation(deltaTime);
            proj.draw(window, RenderStates::Default);
        }

        for (auto& projEnemy : projectilesEnemy)
        {
            projEnemy.draw(window, RenderStates::Default);
        }



        //sprawdzanie kolizji wszystkich obiektow

        //sprawdzanie kolizji enemy z graczem
        for (auto& enemy : enemies)
        {
            if (enemy->checkPlayerCollision(player1))
            {
                damageTaken.play();
                player1.setHp(player1.getHp() - (rand() % 11 + 5), window);
            }
            if (enemy->checkPlayerCollision(player2))
            {
                //damageTaken.play();
                player2.setHp(player2.getHp() - (rand() % 11 + 5), window);
            }
        }


        //sprawdzanie czy przeciwnicy zyja
        bool allEnemiesDead = true;
        for (int i = 0; i < enemies.size(); ++i)
        {
            if (enemies[i]->getHP() > 0)
            {
                allEnemiesDead = false;
            }
        }

        //sprawdzanie kolizji gracza z mapa
        int previousVelocity = player1.getVelocity();
        int previousVelocity2 = player2.getVelocity();
        if (!player1.getItem(0))
        {
            calculateTileRange(player1.getCollisionRect(), tileSize, world.getNumRows(), world.getNumCols(), startRow, endRow, startCol, endCol);
            for (int i = startRow; i <= endRow; i++) {
                for (int j = startCol; j <= endCol; j++) {
                    gameTile* currentTile = world.getTiles()[i][j];

                    if (currentTile->isPassable() && player1.checkCollision(*currentTile))
                    {
                        cout << 1;
                        player1.setPosition(previousPlayerPosition);
                    }
                    else if (currentTile->isSpike() && player1.checkCollision(*currentTile))
                    {
                        //damageTaken.play();
                        player1.setHp(player1.getHp() - (rand() % 11 + 5), window);
                    }
                    else if (currentTile->isPortal() && player1.checkCollision(*currentTile) && allEnemiesDead)
                    {
                        movePlayer = true;
                    }
                    else if (currentTile->isPortal() && player1.checkCollision(*currentTile) && !allEnemiesDead)
                    {
                        player1.setPosition(previousPlayerPosition);
                    }
                    else if (currentTile->isSlime() && player1.checkCollision(*currentTile))
                    {
                        player1.setVelocity(1);
                    }
                    else if (!currentTile->isSlime() && player1.checkCollision(*currentTile))
                    {
                        player1.setVelocity(10);
                    }
                }
            }
            calculateTileRange(player2.getCollisionRect(), tileSize, world.getNumRows(), world.getNumCols(), startRow, endRow, startCol, endCol);
            for (int i = startRow; i <= endRow; i++) {
                for (int j = startCol; j <= endCol; j++) {
                    gameTile* currentTile = world.getTiles()[i][j];

                    if (currentTile->isPassable() && player2.checkCollision(*currentTile))
                    {
                        cout << 1;
                        player2.setPosition(previousPlayerPosition2);
                    }
                    else if (currentTile->isSpike() && player2.checkCollision(*currentTile))
                    {
                        //damageTaken.play();
                        player2.setHp(player2.getHp() - (rand() % 11 + 5), window);
                    }
                    else if (currentTile->isPortal() && player2.checkCollision(*currentTile) && allEnemiesDead)
                    {
                        movePlayer = true;
                    }
                    else if (currentTile->isPortal() && player2.checkCollision(*currentTile) && !allEnemiesDead)
                    {
                        player2.setPosition(previousPlayerPosition);
                    }
                    else if (currentTile->isSlime() && player2.checkCollision(*currentTile))
                    {
                        //player2.setVelocity(1);
                    }
                    else if (!currentTile->isSlime() && player2.checkCollision(*currentTile))
                    {
                        //player2.setVelocity(10);
                    }
                }
            }
        }
        else
        {
            spriteWings.setPosition(playerPosition.x + 110, playerPosition.y + 110);
            window.draw(spriteWings);
            calculateTileRange(player1.getCollisionRect(), tileSize, world.getNumRows(), world.getNumCols(), startRow, endRow, startCol, endCol);
            for (int i = startRow; i <= endRow; i++) {
                for (int j = startCol; j <= endCol; j++) {
                    gameTile* currentTile = world.getTiles()[i][j];
                    if (currentTile->isPortal() && player1.checkCollision(*currentTile) && allEnemiesDead)
                    {
                        movePlayer = true;
                    }
                    else if (currentTile->isPortal() && player1.checkCollision(*currentTile) && !allEnemiesDead)
                    {
                        player1.setPosition(previousPlayerPosition);
                    }
                }
            }
            if (coop == true)
            {
                spriteWings.setPosition(playerPosition2.x + 110, playerPosition2.y + 110);
                window.draw(spriteWings);
                calculateTileRange(player2.getCollisionRect(), tileSize, world.getNumRows(), world.getNumCols(), startRow, endRow, startCol, endCol);
                for (int i = startRow; i <= endRow; i++) {
                    for (int j = startCol; j <= endCol; j++) {
                        gameTile* currentTile = world.getTiles()[i][j];
                        if (currentTile->isPortal() && player2.checkCollision(*currentTile) && allEnemiesDead)
                        {
                            movePlayer = true;
                        }
                        else if (currentTile->isPortal() && player2.checkCollision(*currentTile) && !allEnemiesDead)
                        {
                            player2.setPosition(previousPlayerPosition);
                        }
                    }
                }
            }
        }

        //nowa mapa po przejsciu przez portal
        
        if (movePlayer == true)
        {
            door.play();
            projectiles.clear();
            spawnLimitReached1 = false;
            spawnLimitReached2 = false;
            spawnLimitReached3 = false;
            giveLevel = true;
            enemies.clear();
            objects.clear();
            items.clear();
            world.loadNextLevel(levelDirection);
            Vector2f center((world.mapWidth / 2.0f) - 200, (world.mapHeight / 2.0f) + 100);
            randomItemHp = rand() % itemsHpQuantity;
            randomItemGold = rand() % itemsGoldQuantity;
            if (world.getMap() == 11)
            {
                player1.setPosition(Vector2f(50,300));
                //player2.setPosition(Vector2f(50, 300));
            }
            else
            {
                switch (levelDirection) {
                case 0:
                    player1.setPosition(Vector2f(world.mapWidth - 1900, world.mapHeight / 2.f - 120));
                    break;
                case 1:
                    player1.setPosition(Vector2f(world.mapWidth - 330, world.mapHeight / 2.f - 120));
                    break;
                case 2:
                    player1.setPosition(Vector2f(world.mapWidth / 2.f - 150, world.mapHeight - 300));
                    break;
                case 3:
                    player1.setPosition(Vector2f(world.mapWidth / 2.f - 150, world.mapHeight - 1025));
                    break;
                }
            }
            

            movePlayer = false;
        }

        //sprawdzanie kolizji enemyZombie ze scianami
        if (world.getMap() == 3)
        {
            for (auto& enemy : enemies)
            {
                enemyZombie* zombie = dynamic_cast<enemyZombie*>(enemy);
                calculateTileRange(enemy->getGlobalBounds(), tileSize, world.getNumRows(), world.getNumCols(), startRow, endRow, startCol, endCol);

                for (int i = startRow; i <= endRow; i++)
                {
                    for (int j = startCol; j <= endCol; j++)
                    {
                        gameTile* currentTile = world.getTiles()[i][j];
                        if (currentTile && currentTile->isPassable() && enemy->checkCollision(*currentTile) || currentTile && currentTile->isPortal() && enemy->checkCollision(*currentTile))
                        {
                            zombie->stopMovementX();
                        }
                        else
                        {
                            zombie->startMovementX();
                        }
                    }
                }
            }
        }
        
        //sprawdzanie kolizji enemyRobot i slime ze scianami
        for (auto& enemy : enemies)
        {
            calculateTileRange(enemy->getGlobalBounds(), tileSize, world.getNumRows(), world.getNumCols(), startRow, endRow, startCol, endCol);

            for (int i = startRow; i <= endRow; i++)
            {
                for (int j = startCol; j <= endCol; j++)
                {
                    gameTile* currentTile = world.getTiles()[i][j];
                    if (currentTile && currentTile->isPassable() && enemy->checkCollision(*currentTile) || currentTile && currentTile->isPortal() && enemy->checkCollision(*currentTile))
                    {
                        enemy->changeDirection();
                    }
                }
            }
        }
        

        //sprawdzanie trafienia pociskiem        
        for (auto& projectile : projectiles)
        {
            for (auto& enemy : enemies)
            {
                if (projectile.getGlobalBounds().intersects(enemy->getGlobalBounds()))
                {
                    hit.play();
                    int currentHP = enemy->getHP();
                    int newHP = currentHP - playerDamage;
                    enemy->setHP(newHP);
                    if (!player1.getItem(5)) { projectile.move(); }
                    if (newHP <= 0)
                    {
                        enemyDeath.play();
                        enemy->remove();
                        eliminations += abs(enemy->getOriginalHp());
                        lvlUp(player1, eliminations, playerLvl, playerDamage, playerVelocity, playerGold, minTimeBetweenShots);
                        if (player1.getItem(2))
                        {
                            player1.setHpMedKit(player1.getHp() + 5);
                        }
                    }
                }
            }
        }


        //trafienie z mapa
        for (auto& projectile : projectiles)
        {
            calculateTileRange(projectile.getGlobalBounds(), tileSize, world.getNumRows(), world.getNumCols(), startRow, endRow, startCol, endCol);
            for (int i = startRow; i <= endRow; i++)
            {
                for (int j = startCol; j <= endCol; j++)
                {
                    gameTile* currentTile = world.getTiles()[i][j];
                    if (currentTile && currentTile->isPassable() && projectile.checkCollision(*currentTile) || currentTile && currentTile->isPortal() && projectile.checkCollision(*currentTile))
                    {
                        if (player1.getItem(6)) { projectile.bouncingShot(10); }
                        else if (!player1.getItem(4)) { projectile.move(); }
                    }
                }
            }
        }
        
        //rysowanie
        if (coop == true)
        {
            //player2.updateAnimation(deltaTime);
            //window.draw(player2);
        }

        for (const auto& enemy : enemies)
        {
            enemy->move(player1);
            enemy->displayHp(window, fontPixel, 40, Color::Red);
            enemy->killOfScreen(window);
            window.draw(*enemy);
        }

        for (const auto& object : objects) {
            object->draw(window);
            if (object->checkCollision(player1, window))
            {
                object->displayText(player1, window);
            }
        }

        for (const auto& item : items) {
            item->draw(window);
            item->checkCollision(player1, window);
            item->displayText(player1, window);
        }

        player1.updateAnimation(deltaTime);
        window.draw(player1);

        View currentView = window.getView();
        window.setView(window.getDefaultView());

        //statystyki
        player1.displayHp(window, fontPixel);
        player1.displayExp(window, fontPixel, eliminations);
        window.draw(spriteGold);
        window.draw(textGold);
        window.draw(spriteDamage);
        window.draw(textDamage);
        window.draw(spriteSpeed);
        window.draw(textSpeed);
        window.draw(spriteFireRate);
        window.draw(textFireRate);  
        window.draw(gameTimeText);
        if (Keyboard::isKeyPressed(Keyboard::Tab)) {
            window.draw(spriteMiniMapFrame);
            world.drawMiniMap(window, 120, 255, Vector2f(3.f, 3.f), Vector2f(666.f , 200.f));
        }
        else
        {
            world.drawMiniMap(window, 40, 175, Vector2f(1.f, 1.f), Vector2f(1720.f, 0.f));
        }
        
        window.draw(textMap);

        


        //game over
        if (player1.getHp() <= 0 || player2.getHp() <= 0)
        {
            player1.setHp(0, window);
            player1.setVelocity(0);

            player2.setHp(0, window);
            player2.setVelocity(0);
            projectiles.clear();
            enemies.clear();
            items.clear();

            spawnLimitReached1 = false;
            spawnLimitReached2 = false;
            spawnLimitReached3 = false;
            giveLevel = true;

            Text textDeath;
            textDeath.setFont(font);
            textDeath.setString("GAME OVER");
            textDeath.setCharacterSize(310);
            textDeath.setFillColor(Color::Red);
            textDeath.setPosition(10, 340);

            Text textReset;
            textReset.setFont(font);
            textReset.setString("Press space to reset");
            textReset.setCharacterSize(50);
            textReset.setFillColor(Color::Red);
            textReset.setPosition(742, 650);

            window.draw(pauseRectangle);
            window.draw(textDeath);
            window.draw(textReset);
            
            if (Keyboard::isKeyPressed(Keyboard::Space))
            {
                randomItemHp = rand() % itemsHpQuantity;
                randomItemGold = rand() % itemsGoldQuantity;
                gameClock.restart();
                player1.setDefault();
                player2.setDefault();
                eliminations = 0;
                minTimeBetweenShots = 0.5;
                world.resetMap();
                Vector2f center((world.mapWidth / 2.0f) - 150, (world.mapHeight / 2.0f) - 120);
                player1.setPosition(center);
                player2.setPosition(center);
                
            }
        }

        window.setView(currentView);
        
        }

        if (isPaused) {
            View currentView = window.getView();
            window.setView(window.getDefaultView());

            Text pauseText, resumeText, saveText, loadText, musicText, exitText;

            pauseText.setFont(fontPixel);
            pauseText.setString("PAUSE");
            pauseText.setCharacterSize(200);
            pauseText.setFillColor(Color::White);
            FloatRect textBounds = pauseText.getLocalBounds();
            pauseText.setOrigin(textBounds.left + textBounds.width / 2.f, textBounds.top + textBounds.height / 2.f);
            pauseText.setPosition(window.getSize().x / 2, 150);

            resumeText.setFont(fontPixel);
            resumeText.setString("Resume");
            resumeText.setCharacterSize(30);
            resumeText.setFillColor(Color::White);
            resumeText.setPosition(window.getSize().x / 2 - 60, 250);

            saveText.setFont(fontPixel);
            saveText.setString("Save");
            saveText.setCharacterSize(30);
            saveText.setFillColor(Color::White);
            saveText.setPosition(window.getSize().x / 2 - 60, 300);

            loadText.setFont(fontPixel);
            loadText.setString("Load");
            loadText.setCharacterSize(30);
            loadText.setFillColor(Color::White);
            loadText.setPosition(window.getSize().x / 2 - 60, 350);

            musicText.setFont(fontPixel);
            musicText.setString("Music");
            musicText.setCharacterSize(30);
            musicText.setFillColor(colorMusic ? Color::White : Color(80, 80, 80));
            musicText.setPosition(window.getSize().x / 2 - 60, 400);

            exitText.setFont(fontPixel);
            exitText.setString("Exit");
            exitText.setCharacterSize(30);
            exitText.setFillColor(Color::White);
            exitText.setPosition(window.getSize().x / 2 - 60, 450);

            RectangleShape selector(Vector2f(150, 43));
            selector.setFillColor(Color::Transparent);
            selector.setOutlineThickness(3);
            selector.setOutlineColor(Color::White);

            if (pauseClock.getElapsedTime() > seconds(0.15f)) {
                if (Keyboard::isKeyPressed(Keyboard::Up) || Keyboard::isKeyPressed(Keyboard::W)) {
                    selectedOption = (selectedOption - 1 + 5) % 5;
                    pauseClock.restart();
                }
                else if (Keyboard::isKeyPressed(Keyboard::Down) || Keyboard::isKeyPressed(Keyboard::S)) {
                    selectedOption = (selectedOption + 1) % 5;
                    pauseClock.restart();
                }
            }

            switch (selectedOption) {
            case 0:
                selector.setPosition(resumeText.getPosition().x - 10, resumeText.getPosition().y);
                if (Keyboard::isKeyPressed(Keyboard::Space) || Keyboard::isKeyPressed(Keyboard::Enter)) {
                    isPaused = false;
                }
                break;
            case 1:
                selector.setPosition(saveText.getPosition().x - 10, saveText.getPosition().y);
                if (Keyboard::isKeyPressed(Keyboard::Space) || Keyboard::isKeyPressed(Keyboard::Enter))
                {
                    saveGame(player1, eliminations, playerHp, playerLvl, playerDamage, playerVelocity, playerGold, minTimeBetweenShots, player1.getItem(1), player1.getItem(0));
                }
                break;
            case 2:
                selector.setPosition(loadText.getPosition().x - 10, loadText.getPosition().y);
                if (Keyboard::isKeyPressed(Keyboard::Space) || Keyboard::isKeyPressed(Keyboard::Enter))
                {
                    loadGame(player1, world, window, eliminations, playerHp, playerLvl, playerDamage, playerVelocity, playerGold, minTimeBetweenShots, player1.getItem(1), player1.getItem(0));
                    enemies.clear();
                    isPaused = false;
                }
                break;
            case 3:
                selector.setPosition(musicText.getPosition().x - 10, musicText.getPosition().y);
                if (pauseClock.getElapsedTime() > seconds(0.15f))
                {
                    if (Keyboard::isKeyPressed(Keyboard::Space) || Keyboard::isKeyPressed(Keyboard::Enter))

                    {
                        if (playMusic)
                        {
                            music.stop();
                        }
                        else
                        {
                            music.play();
                        }
                        playMusic = !playMusic;
                        colorMusic = !colorMusic;
                        pauseClock.restart();
                    }
                }
                break;
            case 4:
                selector.setPosition(exitText.getPosition().x - 10, exitText.getPosition().y);
                if (Keyboard::isKeyPressed(Keyboard::Space) || Keyboard::isKeyPressed(Keyboard::Enter)) {
                    window.close();
                }
                break;
            }

            window.draw(pauseBackground);
            window.draw(pauseText);
            window.draw(resumeText);
            window.draw(saveText);
            window.draw(loadText);
            window.draw(musicText);
            window.draw(exitText);
            window.draw(selector);

            window.setView(currentView);
        }

        window.display();
        }
        

    return 0;
}
#pragma once
#include <SFML/Graphics.hpp>
#include "gameTile.h"
#include "player.h"
#include "enemy.h"
#include <vector>
#include <cstdlib>
#include <ctime>
#include <algorithm>
#include <random>

using namespace sf;
using namespace std;

class gameWorld
{
public:

    gameWorld();
    void setMapLayout(int layoutIndex);
    int getNumRows() const { return numRows; }
    int getNumCols() const { return numCols; }
    const vector<vector<gameTile*>>& getTiles() const { return tiles; }

    void setLayouts();
    void initializeLevelMatrix();
    bool isValid(int x, int y);
    void dfs(int x, int y, int& count, vector<int>& numberSequence, vector<pair<int, int>>& visited);
    void resetMap();

    void loadNextLevel(int direction);
    void updatePortal(int layoutIndex);
    void removeTiles();
    int getMap() const;
    void drawMiniMap(RenderWindow& window, int size, int transparent, Vector2f scale, Vector2f position);

    bool getSpawnLimitReached(int index) const;
    void setSpawnLimitReached(int index, bool value);

    bool getObjectLimitReached(int index) const;
    void setObjectLimitReached(int index, bool value);

    int mapWidth;
    int mapHeight;

private:
    vector<vector<int>> levelMatrix;

    vector<vector<int>> layout1;
    vector<vector<int>> layout2;
    vector<vector<int>> layout3;
    vector<vector<int>> layout4;
    vector<vector<int>> layout5;
    vector<vector<int>> layout6;
    vector<vector<int>> layout7;
    vector<vector<int>> layout8;
    vector<vector<int>> layoutSpecial1;
    vector<vector<int>> layout10;
    vector<vector<int>> layout11;
    vector<vector<int>> layout12;
    vector<vector<int>> layout13;

    vector<vector<gameTile*>> tiles;

    int numRows, numCols;
    int currentLevelX, currentLevelY;
    int mapa;

    void setUpTiles(const vector<vector<int>>& layout);

    Texture textureNotMap;
    Texture textureMap;
    Texture texturePlayerIcon;
    Texture textureSlimeIcon;
    Texture textureDragonIcon;
    Texture textureSpecialIcon;
    Texture textureItemIcon;
    Texture textureDevilIcon;

    static const int roomCount = 14;
    bool spawnLimitReached[roomCount];
    bool objectLimitReached[roomCount];
};
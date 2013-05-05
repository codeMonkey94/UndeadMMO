// See the file COPYRIGHT.txt for authors and copyright information.
// See the file LICENSE.txt for copying conditions.

#include <fstream>
#include "map.h"

using namespace std;

Map::Map()
{
    ready = false;
    LoadTileTextures();
}

Map::Map(vector<vector<TileID> > & mapData)
{
    LoadTileTextures();
    LoadMapFromMemory(mapData);
}

Map::Map(const string& filename)
{
    LoadTileTextures();
    LoadMapFromFile(filename);
}

void Map::LoadTileTextures()
{
    // 16 = 1024 / 128
    int tileCount = 1024 / tileWidth;
    textures.resize(tileCount * tileCount);
    for (int y = 0; y < tileCount; y++)
    {
        for (int x = 0; x < tileCount; x++)
        {
            if (textures[y * tileCount + x].loadFromFile("data/images/tiles/tiles.png",
                sf::IntRect(x * tileWidth, y * tileWidth,
                (x + 1) * tileWidth, (y + 1) * tileWidth)))
                    exit(3);
            textures[y * tileCount + x].setSmooth(true);
        }
    }
}

void Map::LoadMapFromMemory(vector<vector<TileID> > & mapData)
{
    tiles.resize(mapData.size());
    for (uint i = 0; i < mapData.size(); i++)
    {
        for (uint j = 0; j < mapData[i].size(); j++)
        {
            tiles[i].push_back(Tile(mapData[i][j], i * tileWidth, j * tileHeight));
        }
    }

    ready = true;
}

bool Map::LoadMapFromFile(const string& filename)
{
    ifstream in(filename);
    if (!in.is_open())
        return false;

    string temp;
    int tmpID = 0;
    int width, height;
    in >> width >> height;

    tiles.resize(height);
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            in >> tmpID;
            tiles[i].push_back(Tile(tmpID, i * tileWidth, j * tileHeight));
        }
    }
    in.close();

    ready = true;

    return true;
}

void Map::draw(sf::RenderTarget& window, sf::RenderStates states) const
{
    if (!ready)
        return;

    // No need to pass the view in separately, it is already stored inside the window
    sf::View viewWindow = window.getView();
    // TODO: Actually calculate the rectangle from the sf::View of the window
    sf::FloatRect viewRect(0, 0, 800, 600);

    // Convert coordinates of view to logical tile coordinates
    int startX = viewRect.left / tileWidth;
    int startY = viewRect.top / tileHeight;

    int endX = (viewRect.left + viewRect.width) / tileWidth + 1;
    int endY = (viewRect.top + viewRect.height) / tileHeight + 1;

    // Check if these values are within bounds of the array
    if (startX < 0)
        startX = 0;
    if (startY < 0)
        startY = 0;
    if (endX >= (int)tiles[startX].size())
        endX = tiles[startX].size() - 1;
    if (endY >= (int)tiles.size())
        endY = tiles.size() - 1;

    // Draw all of the tiles within view
    for (int y = startY; y < endY; y++)
    {
        for (int x = startX; x < endX; x++)
        {
            window.draw(tiles[y][x]);
        }
    }
}

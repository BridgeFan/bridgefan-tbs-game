#pragma once
#ifndef MAP_H_INCLUDED
#define MAP_H_INCLUDED
#include "tile.hpp"
#include "basic/constants.hpp"
class Unit; class UnitKind;
class BuildingType; class Building;
namespace bfUI{class MegaTexture;};
//extern MegaTexture mainMega;
extern std::string logStr;
extern const int tileSize;
extern std::vector<Unit> units;
class Data;
//extern std::vector<std::shared_ptr<UnitKind> > unitTypes;
//extern std::vector<std::shared_ptr<BuildingType> >buildingTypes;
//extern std::vector<Building>buildings;
class City; extern std::vector<City>cities;
extern float countSqrDifference(my_vector2f a, my_vector2f b);
class Map
{
private:
    const Data* data;
    std::vector<std::vector<tile> > mapa;
    std::vector<std::vector<uint64_t> > isVis;
    std::vector<std::vector<uint64_t> > everVis;
    my_vector2i mapSize;
    bool init(int x,int y);
    bool postInit(const State& state);
    //uint64_t isVis[384][384]; ///i-th bite (counting from least important) describes i-th player
    //uint64_t everVisible[384][384]; ///i-th bite (counting from least important) describes i-th player
    sf::Image miniMapBase;
    sf::Image miniMap;
    sf::Texture miniMapTexture;
public:
    void setData(const Data& data) {this->data=&data;}
    sf::Sprite miniMapSprite;
    void actualizeMiniMap(int playerIndex);
    bool wasEverVisible(int x, int y, int playerIndex) const {return (everVis[x][y]>>playerIndex)%2;}
    bool wasEverVisible(const my_vector2i& pos, int playerIndex) const {return wasEverVisible(pos.x,pos.y,playerIndex);}
    bool isVisible(int x, int y, int playerIndex) const {return (isVis[x][y]>>playerIndex)%2;}
    bool isVisible(const my_vector2i& pos, int playerIndex) const {return isVisible(pos.x,pos.y,playerIndex);}
    void draw(const State& state, sf::RenderTarget& target, bool isBackground=false) const;
    bool loadFromFile(State& state, std::string name);
    bool generate(my_vector2i mSize, const State& state);
    bool generate(int mSizeX,int mSizeY, const State& state) {return generate(my_vector2i(mSizeX,mSizeY),state);}
    tile& operator()(int x,int y) {return mapa[x][y];}
    tile& operator()(const my_vector2i& pos) {return mapa[pos.x][pos.y];}
    const tile& operator()(int x,int y) const {return mapa[x][y];}
    const tile& operator()(const my_vector2i& pos) const {return mapa[pos.x][pos.y];}
    const my_vector2i& getSize() const {return mapSize;}
    void setSize(int x, int y) {mapSize=my_vector2i(x,y);}
    void setRoad(my_vector2i pos, int8_t level=1);
    bool saveToFile(const State& state, std::string name);
    void actualizeVisibility(uint8_t actPlayer, const State& state);
    void actualizeInfluence();
    //bool isVisible(const my_vector2i& pos, short playerInd) {return isVis[pos.x][pos.y]}
    //bool changeVisible(const my_vector2i& pos, short playerInd) {return isVis[pos.x][pos.y]^=(1<<playerInd)}
    void updateOwners();
    void clear();
    void toIso();
    void toTopDown();
};
float rand(float rCent, float rDiff);
#endif // MAP_H_INCLUDED

#pragma once
#ifndef IMPROVEMENT_H_INCLUDED
#define IMPROVEMENT_H_INCLUDED
#include <SFML/Graphics.hpp>
#include <string>
#include "basic/constants.hpp"

class Player;
class MegaTexture;
class UI;
class Data;
class Unit; class UnitKind;
extern std::vector<Unit> units;
extern std::vector<Player> players;
extern UI gameUI;
class Map;
extern Map mapa;
class City;
extern std::vector<City> cities;
class BuildingType
{
public:
	std::string name;
	int buildingCost[BUILDING_STAGES_NUMBER][RESOURCE_TYPES_NUMBER+1]; //cost of i+1 stage, last cost is worker MP cost
	int16_t maxStage;
	int16_t requiredTechnologyId[BUILDING_STAGES_NUMBER];
	uint16_t maxEmployedNr[BUILDING_STAGES_NUMBER];
	std::unique_ptr<MegaTexture> itMega;
	MegaTexture* otMega;
	short iconIndFirst, iconIndLast;
	short ownerTextureInd;
	virtual ~BuildingType() {}
	BuildingType(std::string imName="");
	BuildingType(std::string imName,int build[BUILDING_STAGES_NUMBER][RESOURCE_TYPES_NUMBER+1], MegaTexture& imMega, MegaTexture& ocMega,int megaBegin);
	virtual void info()=0;
};
class ResourceBuilding: public BuildingType
{
public:
    virtual void info() {}
    int16_t givenResourceID, givenResourceNumber[BUILDING_STAGES_NUMBER];
    ResourceBuilding(std::string imName=""): BuildingType(imName) {}
};
class Farm: public BuildingType
{
public:
    virtual void info() {}
    int16_t givenFood[BUILDING_STAGES_NUMBER];
    Farm(std::string imName=""): BuildingType(imName) {}
    //ResourceBuilding(string imName,int build[BUILDING_STAGES_NUMBER][RESOURCE_TYPES_NUMBER+1], sf::Texture imIcon[BUILDING_STAGES_NUMBER]): BuildingType(imName,build,imIcon) {}
};
class RecruitBuilding: public BuildingType
{
public:
    virtual void info() {}
    int16_t recruitedUnit, maxNrRecruited[BUILDING_STAGES_NUMBER];
    int16_t recruitCost[RESOURCE_TYPES_NUMBER];
    std::string desc1, desc2;
    RecruitBuilding(std::string imName=""): BuildingType(imName) {}
    //RecruitBuilding(string imName,int build[BUILDING_STAGES_NUMBER][RESOURCE_TYPES_NUMBER+1], sf::Texture imIcon[BUILDING_STAGES_NUMBER]): BuildingType(imName,build,imIcon) {}
};
class HouseBuilding: public BuildingType
{
public:
    virtual void info() {}
    int16_t heldPopulation[BUILDING_STAGES_NUMBER];
    HouseBuilding(std::string imName=""): BuildingType(imName) {}
    //HouseBuilding(string imName,int build[BUILDING_STAGES_NUMBER][RESOURCE_TYPES_NUMBER+1], sf::Texture imIcon[BUILDING_STAGES_NUMBER]): BuildingType(imName,build,imIcon) {}
};
class RoadBuilding: public BuildingType
{
public:
    virtual void info() {}
    int16_t roadMoveCost[BUILDING_STAGES_NUMBER];
    //sf::Texture mapIcons[2*BUILDING_STAGES_NUMBER];
    MegaTexture* mapIconsMega;
    short mapIconsIndexes[2*BUILDING_STAGES_NUMBER];
    RoadBuilding(std::string imName=""): BuildingType(imName) {}
    //RoadBuilding(string imName,int build[BUILDING_STAGES_NUMBER][RESOURCE_TYPES_NUMBER+1], sf::Texture imIcon[BUILDING_STAGES_NUMBER]): BuildingType(imName,build,imIcon) {}
};
class Building: public sf::Drawable
{
private:
	sf::Sprite icon, ownerSprite;
	sf::CircleShape underConstrCircle;
public:
    const State* state;
    const Data* data;
	std::shared_ptr<BuildingType> type;
	int8_t stage; //0-not improved
	short owner; ///index of city that owns building
	short lastManCost;
	uint16_t employed; ///number of employed population
	int16_t tmp1;
	///methods
    //void setOwner(short num);
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
	Building(std::shared_ptr<BuildingType> t, short owner, const my_vector2f& pos, const State& state, const Data& data);
    void setPosition(const int x, const int y);
    void setPosition(const my_vector2f& pos) {setPosition(pos.x,pos.y);}
    void newTurn();
	bool improve(const State& state);
	bool recruit(const State& state, int kount);
	void setStage(int8_t s);
	bool tryAddEmployee();
	bool tryRemoveEmployee();
	void clear();
    my_vector2f getPosition() const {return icon.getPosition();}
};

#endif // IMPROVEMENT_H_INCLUDED

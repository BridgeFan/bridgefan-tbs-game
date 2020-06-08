#pragma once
#ifndef TILE_H_INCLUDED
#define TILE_H_INCLUDED
#include <SFML/Graphics.hpp>
#include "basic/constants.hpp"
#include <iostream>
class MegaTexture;
class Building; class RoadBuilding; class UI;
//extern MegaTexture mainMega, mainIsoMega;
//extern sf::Texture beach;
//extern sf::Texture nullTexture;
//extern bool isGridVisible;
//extern RoadBuilding roadInfo;
extern UI gameUI;
class Player;
extern std::vector<Player> players;
class City;
extern std::vector<City> cities;
class Data;
class tile
{
private:
    sf::Sprite mainSprite;
	sf::Sprite riverSprite;
	sf::Sprite hDiffSprite;
	sf::Sprite hPartSprite;
	//sf::RectangleShape ownerRect;
	uint8_t roadStage; //0-none, 1-4 - level of road
	sf::Sprite roadSprites[8];
	int16_t owner; ///-1 - no owner
	std::vector<std::pair<int,int> > influencePts; //first-city owner, second-amount
	const Data& data;
public:
	sf::RectangleShape background;
	sw::Line line[4];
	int8_t level; //over sea level
	short cityField;
	uint8_t forestType; //0-nothing, 1-forest, 2-taiga
	uint8_t isRiver;
	uint8_t river; //(1-2-4-8-16-32-64-128): W,N,E,S,NW,NE,SE,SW
	uint8_t hDiff; //analogical
	mainMegaIDs terrainType;
	int8_t resource;
	void draw(sf::RenderTarget& target, const State& state) const;
	tile(const Data& data);
	int8_t getRoadStage() const {return roadStage;}
	void setRoadStage(const int8_t stage) {roadStage=stage;}
	void UpdateSprites();
	void setRoadTexture(int8_t index);
    void unsetRoadTexture(int8_t index) {roadSprites[index].setTexture(sf::Texture());}
	int tileValue(int l, bool isDiagonal=false, bool diagCrosRiv=false, int8_t fromRoadStage=0) const;
	void setPosition(int x, int y, bool isIso=false);
	void setOwner(int16_t owner);
	int16_t getOwner() const {return owner;}
	void actualizeColor();
	void clearInfluence(){influencePts.clear();}
	void addInfluence(short owner, int pts);
	int getInfluence (short owner) const;
	void setIsoTextures (bool isIso);
	void writeInfluencers ()
	{
	    /*std::cout << influencePts.size() << "\n";
	    for(std::size_t i=0u;i<influencePts.size();i++)
        {
            std::cout << influencePts[i].first << " " << influencePts[i].second << "\t";
        }
        system("pause");*/
	}
	int InflSize () const {return influencePts.size();};
	std::vector<std::pair<int,int> >::const_iterator inflEnd() const {return influencePts.end();}
	std::optional<std::pair<int,int> > maxInfluenceIt(){return MaxEl(influencePts);}
protected:
	void riverScript();
	void heightScript();
	void setPositionTmp(int x, int y, sf::Sprite& tmpSpr);
};

#endif // TILE_H_INCLUDED

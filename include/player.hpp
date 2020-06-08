#pragma once
#ifndef PLAYER_H_INCLUDED
#define PLAYER_H_INCLUDED
#include "basic/constants.hpp"
#include <vector>
#include <SFML/Graphics.hpp>
class City;
class Building;
namespace bfUI{class MegaTexture;};
//extern MegaTexture uiTexture;
class Player
{
public:
    int resourceQuantity[RESOURCE_TYPES_NUMBER]; //gold, wood, stone, iron, diamond, fireMana, earthMana, airMana, waterMana
    int techPts;
    sf::String name;
    //vector<Building*> playerImpros;
    std::vector<City*> cities;
    sf::Color playerColor;
    void updateResources();
    bool isComp;
    Player(bool isComp=false, const sf::String& plName=sf::String());
    Player(int begRes[], bool isComp=false, const sf::String& plName=sf::String());
    void setResource(int a,int8_t resID=gold) {resourceQuantity[resID-gold]=a;}
    void addResource(int a, int8_t resID=gold) {resourceQuantity[resID-gold]+=a;}
    void substractResource(int a, int8_t resID=gold) {resourceQuantity[resID-gold]-=a;}
    int getResource(int8_t resID) const {return resourceQuantity[resID-gold];}
};


#endif // PLAYER_H_INCLUDED

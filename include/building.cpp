#include "building.hpp"
#include "basic/megaTexture.hpp"
#include "player.hpp"
#include "ui.hpp"
#include "unit.hpp"
#include "map.hpp"
#include "city.hpp"
#include "Data.hpp"
#include <typeinfo>
#include <iostream>
BuildingType::BuildingType(string imName): name(imName)
{
    for(int i=0;i<BUILDING_STAGES_NUMBER;i++)
        maxEmployedNr[i]=10;
    ownerTextureInd=circleBack;
    for(int i=0;i<BUILDING_STAGES_NUMBER;i++)
	{
	    for(int j=0;j<RESOURCE_TYPES_NUMBER;j++)
        {
            buildingCost[i][j]=0;
        }
		iconIndFirst=-1;
	}
}
BuildingType::BuildingType(string imName,int build[BUILDING_STAGES_NUMBER][RESOURCE_TYPES_NUMBER+1], MegaTexture& imMega, MegaTexture& ocMega, int megaBegin): name(imName), iconIndFirst(megaBegin)
{
    itMega.reset(&imMega);
    otMega=&ocMega;
	for(int i=0;i<BUILDING_STAGES_NUMBER;i++)
	{
	    for(int j=0;j<RESOURCE_TYPES_NUMBER;j++)
        {
            buildingCost[i][j]=build[i][j];
        }
		//icon[i]=imIcon[i];
	}
}
void Building::newTurn()
{
    if(owner<0) return;
    lastManCost=std::max(0,lastManCost-employed);
    if(lastManCost>0)
        return;
    if(typeid(*type.get())==typeid(ResourceBuilding))
    {
        ResourceBuilding* tmpType=dynamic_cast<ResourceBuilding*>(type.get());
        players[cities[owner].owner].resourceQuantity[tmpType->givenResourceID-gold]+=tmpType->givenResourceNumber[stage]*employed;
    }
    else if(typeid(*type.get())==typeid(RecruitBuilding))
    {
        tmp1=std::min(dynamic_cast<RecruitBuilding*>(type.get())->maxNrRecruited[stage],int16_t(tmp1+employed));
    }
}
/*void Building::setOwner(short num)
{
    if(typeid(*type)!=typeid(HouseBuilding))
    {
        owner=num;
        return;
    }
    if(owner>=0)
        cities[owner].removeBuilding(*this);
    owner=num;
    if(owner>=0)
        cities[owner].addBuilding(*this);
    //ownerSprite.setColor(players[num].playerColor);
}*/
Building::Building(std::shared_ptr<BuildingType> t, short owner, const my_vector2f& pos, const State& state, const Data& data): data(&data), type(t), owner(owner)
{
    this->state=&state;
    employed=0;
    t->itMega->assignTexture(icon,t->iconIndFirst);
    setStage(0);
    setPosition(pos);
    //icon.setTexture(t->icon[0]);
    //ownerSprite.setTexture(t->ownerTexture);
    t->otMega->assignTexture(ownerSprite,t->ownerTextureInd);
    lastManCost=t->buildingCost[0][RESOURCE_TYPES_NUMBER];
    ownerSprite.setScale(.75f,.75f);
    underConstrCircle.setFillColor(sf::Color::Transparent);
    underConstrCircle.setOutlineColor({255,127,0});
    underConstrCircle.setOutlineThickness(16.f);
    underConstrCircle.setRadius(56.f);
    tmp1=0;
    owner=-1;
}
bool Building::improve(const State& state)
{
    if(stage>=type->maxStage-1) return EXIT_FAILURE;
    for(int i=0;i<RESOURCE_TYPES_NUMBER;i++)
    {
        if(players[owner].getResource(i+gold)<type->buildingCost[stage][i])
            return EXIT_FAILURE;
    }
    for(int i=0;i<RESOURCE_TYPES_NUMBER;i++)
    {
        players[owner].substractResource(type->buildingCost[stage][i],i+gold);
    }
    stage++;
    lastManCost=type->buildingCost[stage][RESOURCE_TYPES_NUMBER];
    type->itMega->assignTexture(icon,type->iconIndFirst+stage);
    //icon.setTexture(type->icon[stage]);
    gameUI.actualizePlayer(state);
    gameUI.improInfo(state);
    return EXIT_SUCCESS;
}
bool Building::recruit(const State& state, int kount)
{
    if(typeid(*type.get())!=typeid(RecruitBuilding)) return EXIT_FAILURE;
    RecruitBuilding* rType=dynamic_cast<RecruitBuilding*>(type.get());
    for(int i=0;i<RESOURCE_TYPES_NUMBER;i++)
    {
        if(players[owner].getResource(i+gold)<rType->recruitCost[i]*kount)
            return EXIT_FAILURE;
    }
    for(int i=0;i<RESOURCE_TYPES_NUMBER;i++)
    {
        players[owner].substractResource(rType->recruitCost[i]*kount,i+gold);
    }
    //units.push_back(Unit(unitTypes[dynamic_cast<RecruitBuilding*>(buildings[state.pressedImproIndex].type)->recruitedUnit]));
    units.back().kount=kount;
    units.back().setOwner(state.pressedImproCityIndex);
    units.back().setPosition(cities[state.pressedImproCityIndex].getBuilding(state.pressedImproIndex).getPosition());
    gameUI.actualizePlayer(state);
    return EXIT_SUCCESS;
}
void Building::setStage(int8_t s)
{
    if(s>=type->maxStage-1) return;
    stage=s;
    type->itMega->assignTexture(icon,type->iconIndFirst+stage);
}

void Building::setPosition(const int x, const int y)
{
    icon.setPosition(x,y);
    ownerSprite.setPosition(x+72,y);
    underConstrCircle.setPosition(x+8,y+8);
    mapa.actualizeVisibility(owner, *state);
    mapa.actualizeInfluence();
}
void Building::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    target.draw(icon,states);
    if(lastManCost>0)
        target.draw(underConstrCircle, states);
    //target.draw(ownerSprite,states);
}
bool Building::tryAddEmployee()
{
    if(owner>=0 && cities[owner].freePop>0 && employed<type->maxEmployedNr[stage])
    {
        employed++;
        cities[owner].freePop--;
        return true;
    }
    return false;
}
bool Building::tryRemoveEmployee()
{
    if(employed>0)
    {
        employed--;
        cities[owner].freePop++;
        return true;
    }
    return false;
}

void Building::clear()
{
    icon.setTexture(data->getNullTexture());
}

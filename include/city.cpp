#include "city.hpp"
#include "building.hpp"
#include "player.hpp"
#include "basic/megaTexture.hpp"
#include "map.hpp"
#include "ui.hpp"
#include "Data.hpp"
#include <iostream>
int City::actualCity=-1;
City::City(const Data& data, int index, State& state, unsigned char owner, const my_vector2i& pos, int popul): data(data), pop(popul), state(state), owner(owner)
{
	this->index=index;
    stage=0;
    maxPop=30;
    freePop=popul;
    food=20;
    data.getMainMegaTexture().assignTexture(icon,cityTex);
	setPosition(pos);
	nameBackground.setPosition(tileToMap(pos).x-196.f,tileToMap(pos).y+128.f);
	nameBackground.setSize({512.,96.});
	sf::Color t=players[owner].playerColor;
	nameBackground.setFillColor({t.r,t.g,t.b,127});
	nameText.setPosition(tileToMap(pos).x-196.f,tileToMap(pos).y+128.f);
	nameText.setFont(data.getFont());
	nameText.setStyle(sf::Text::Bold);
	nameText.setCharacterSize(80);
	if(nameBackground.getFillColor().r+nameBackground.getFillColor().g+nameBackground.getFillColor().b<150)
        nameText.setFillColor(sf::Color::White);
    else
        {nameText.setFillColor(sf::Color::Black);}
	view.setCenter(tileToMap(pos).x+data.getMainMegaTexture().getTileSize()*.5f,tileToMap(pos).y+data.getMainMegaTexture().getTileSize()*.5f);
	view.setSize(11.f*data.getMainMegaTexture().getTileSize()*state.view.getSize().x/state.view.getSize().y,11.f*data.getMainMegaTexture().getTileSize());
    //view.setSize(11.f*data.getMainMegaTexture().getTileSize()*appWindow.getSize().x/appWindow.getSize().y,11.f*data.getMainMegaTexture().getTileSize());
}
void City::setStage(int8_t s)
{
    maxPop+=30*(s-stage);
    stage=s;
    data.getMainMegaTexture().assignTexture(icon,cityTex+stage);
    mapa.updateOwners();
}
void City::addBuilding(Building&& building)
{
    cityBuildings.push_back(building);
    if(typeid(*(building.type.get()))==typeid(HouseBuilding))
    {
        auto tmpHB=dynamic_cast<HouseBuilding*>(building.type.get());
        maxPop+=tmpHB->heldPopulation[short(building.stage)];
    }
}
void City::removeBuilding(int index)
{
    if(typeid(*(cityBuildings[index].type.get()))==typeid(HouseBuilding))
    {
        auto tmpHB=dynamic_cast<HouseBuilding*>(cityBuildings[index].type.get());
        maxPop-=tmpHB->heldPopulation[short(cityBuildings[index].stage)];
    }
    auto tmp=cityBuildings[index];
    cityBuildings[index]=cityBuildings.back();
    cityBuildings.back()=tmp;
    //std::swap(cityBuildings[index], cityBuildings.back());
    cityBuildings.pop_back();
}
void City::actualizeBuilding(int index, int oldL)
{
    if(typeid(*(cityBuildings[index].type.get()))==typeid(HouseBuilding))
    {
        auto tmpHB=dynamic_cast<HouseBuilding*>(cityBuildings[index].type.get());
        maxPop-=tmpHB->heldPopulation[oldL];
        maxPop+=tmpHB->heldPopulation[short(cityBuildings[index].stage)];
    }
}
void City::setName(const sf::String& n)
{
    my_vector2f pos = getPosition();
    if(n.getSize()>32)
    {
        name=n.substring(0,32);
    }
    else
        name=n;
    nameText.setString(name);
	nameText.setPosition(pos.x-0.5f*nameText.getLocalBounds().width+64.f,pos.y+128.f);
	nameBackground.setSize({nameText.getLocalBounds().width,96.f});
	nameBackground.setPosition(nameText.getPosition());
}
void City::setPopulation(int population)
{
    freePop+=(population-pop);
    pop=population;
    //nameText.setString(name+" ("+my_to_string(pop)+")");
}
void City::setPosition(int x, int y)
{
    icon.setPosition(tileToMap(x,y));
    mapa.actualizeVisibility(owner, state);
}
void City::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    target.draw(icon,states);
    target.draw(nameBackground,states);
    target.draw(nameText,states);
}
void City::CityView(State& state)
{
    actualCity=index;
    sf::View oldView=state.view;
    state.view=view;
    gameUI.cityInfo(index);
    uint8_t oldState=state.drawState;
    state.drawState=CITY_VIEW;
    while(true)
    {
        bool tmp614=false;
        while (appWindow.pollEvent(state.event))
        {
            if(state.event.type==sf::Event::KeyPressed)
            {
                tmp614=true;
                break;
            }
        }
        if(tmp614)
            break;
    }
    state.drawState = oldState;
    state.view = oldView;
    actualCity=-1;
    gameUI.cityInfo(-1);
}
void City::newTurn()
{
    ///2% w skali tury (dla ludzi) - ok.35 tur do podwojenia populacji
    int r=rand()%50;
    pop+=std::floor((pop+r)/50);
    setPopulation(std::min(pop,maxPop));
    for(std::size_t i=0u;i<cityBuildings.size();i++)
        cityBuildings[i].newTurn();
}
void City::improveBuildingOfIndex(int index, State& state)
{
    cityBuildings[index].improve(state);
}

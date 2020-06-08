#pragma once
#ifndef CITY_H_INCLUDED
#define CITY_H_INCLUDED
#include "basic/constants.hpp"
#include <SFML/Graphics.hpp>
class Player;
extern std::vector<Player> players;
class Map;
extern Map mapa;
class UI;
extern UI gameUI;
class Building;
class Data;
class City: public sf::Drawable
{
private:
    const Data& data;
    sf::String name;
    int index;
	int8_t stage;
	sf::Sprite icon;
	sf::RectangleShape nameBackground;
	sf::Text nameText;
	int pop;
    int maxPop;
	std::vector<Building> cityBuildings;
public:
    int freePop;
    State& state;
	sf::View view;
    static int actualCity;
    short food;
	uint8_t owner;
	void setStage(int8_t s);
	int8_t getStage() {return stage;}
	City(const Data& data, int index, State& state, uint8_t owner=255, const my_vector2i& pos=my_vector2i(), int popul=20);
	void setPosition(int x, int y);
    void setPosition(const my_vector2i& pos) {setPosition(pos.x,pos.y);}
    void setPosition(const my_vector2f& pos) {setPosition(pos.x,pos.y);}
    void setPopulation(int population);
    int getPopulation() const {return pop;}
    my_vector2f getPosition() const {return icon.getPosition();}
    //const my_vector2u& getPosition() const {return position[index];}
    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
    const sf::String& getName() const {return name;}
    sf::String& getName() {return name;}
    void setName(const sf::String& n);
    void CityView(State& state);
    void newTurn();
    int getMaxPop() {return maxPop;}
    void addBuilding(Building&& building);
    void addBuilding(Building& building) {addBuilding(std::move(building));}
    void removeBuilding(int building);
    void improveBuildingOfIndex(int index, State& state);
    int16_t getBuildingNum() {return cityBuildings.size();}
    void actualizeBuilding(int index, int oldL);
    Building& getBuilding(int index) {return cityBuildings[index];}
};

#endif // CITY_H_INCLUDED

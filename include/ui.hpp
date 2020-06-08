#pragma once
#define MAX_ROUTE_LENGTH 100
#ifndef UI_H_INCLUDED
#define UI_H_INCLUDED
#include <SFML/Graphics.hpp>
#include "bfUI.hpp"
#include <vector>
#include "basic/constants.hpp"
#include <mutex>

class Unit;
class Building; class BuildingType;
class Player; class Map;
class City;
extern Map mapa;
class Data;
class MegaTexture;
extern std::vector<Unit> units;
extern std::vector<City> cities;
extern const int tileSize;
extern std::vector<Player> players;
extern sf::RenderWindow appWindow;
extern sf::Texture bfUI::uiBackground;
class UI
{
    sf::Sprite miniature, uiBack, miniatureOwnerSprite, addSprite[MAX_NUMBER_OF_PARAMETERS], addRSprite[MAX_NUMBER_OF_PARAMETERS];
    sf::Text countText, actualPlayerText, addText[MAX_NUMBER_OF_PARAMETERS], addRText[MAX_NUMBER_OF_PARAMETERS], otherParamText;
    sf::Sprite resourceSprite[RESOURCE_TYPES_NUMBER];
    sf::Text resourceText[RESOURCE_TYPES_NUMBER];
    sf::Text nameText;
    sf::RectangleShape pauseBack, menuBack;
    void showUpgradeCost(const State& state);
    uint8_t isPaused;
    bool isOfficer;
    const Data* data;
public:
    UI(): data(nullptr) {}
    sf::Texture UIBack;
    //sf::Texture targetTexture;
    sf::CircleShape targetCircle;
    sf::Sprite grid;
    bool canBeBuilt;
    uint8_t pressedType;
    ///PAUSE MENU BUTTONS
    bfUI::Button escapeButton, returnToGameButton, saveGameButton, restartGameButton;
    ///MAIN UI BUTTONS
    bfUI::Button unitButton, buildButton, upgradeButton, destroyButton/*, recruitButton*/;
    bfUI::Button openImprovMenuButton;
    ///MAKE SURE BUTTONS
    sf::RectangleShape makeSureBack;
    sf::Text makeSureText;
    bfUI::Button makeSureYesButton, makeSureNoButton;
    ///INSERT TEXT
    bfUI::InsertText insertText;
    ///IMPROVEMENT MENU BUTTONS
    bfUI::Button improvementChooseButton[50], improvementOkButton, improvementReturnButton;
    ///BUILD IMPROVEMENT
    bfUI::Button buildOk, buildRet;
    sf::Sprite buildPropos;
    ///BUILDING EMPLOYEES BUTTONS
    bfUI::Button workUp, workDown;
    ///RECRUIT MENU BUTTONS
    bfUI::Button recruitOkButton, recruitReturnButton;
    bfUI::Slider recruitSlider;
    sf::Text recruitNrText;
    void setData(const Data& data) {this->data=&data;}
    void setPause(uint8_t is) {isPaused=is; escapeButton.setActive(is); returnToGameButton.setActive(is); if(is==2) {pressedType=255;canBeBuilt=false;}}
    uint8_t getPause() const {return isPaused;}
    void unitInfo(const State& state);
    void improInfo(const State& state);
    void cityInfo(int index);
    bool isGridVisible;
    bool init(sf::View);
    bool makeSure(State& state, std::string text); //true - yes, false - no
    sf::String insertTxt(State& state, std::string text);
    void uiReseting(sf::View myView);
    void setTargetPosition(const State& state, int x, int y, int dis=0);
    void setTargetPosition(const State& state, my_vector2i pos, int dis=0);
    void setTargetPosition(const State& state, my_vector2i pos, int dis, short attType);
    void unsetTarget();
    void actualizePlayer(const State& state);
    void actualizeTileInfo(const State& state);
    void showInfo();
    void checkIfCanBeBuilt(const State& state);
    void draw(const State& state, sf::RenderTarget& target) const;
    bool loadUI();
    int getUiWidth() {return UIBack.getSize().x*(appWindow.getSize().y/1080.f);}
};

#endif // UI_H_INCLUDED

#pragma once
#ifndef MAINMENU_H_INCLUDED
#define MAINMENU_H_INCLUDED
#include "basic/constants.hpp"
#include "bfUI.hpp"
#include "LoadingScreen.hpp"
#include <TGUI/TGUI.hpp>
using std::string;
extern sf::RenderWindow appWindow;
//extern sf::Font font;
class Data;
//class MegaTexture;
//extern MegaTexture uiMega;
class State;
class MainMenu: public sf::Drawable
{
private:
	std::vector<sf::Text> texts;
	std::vector<sf::Text> dateTexts;
	static sf::String VideoModeToString(const sf::VideoMode& v) {return my_to_string(v.width)+"x"+my_to_string(v.height);}
	const Data& data;
	LoadingScreen ls;
    int8_t state;
public:
    tgui::Gui gui;
    MainMenu(const Data& data, const State& state, const sf::Window* window);
    void init(const State&);
    int16_t mapSize;
    bfUI::WidgetList list;
    tgui::Button::Ptr newGameButton, loadGameButton, settingsButton;
    tgui::Button::Ptr playerNumButton[8], mapSizeButton[8], generateMapButton;
    bfUI::Button escapeButton;
    bfUI::Button upButton, downButton, saveSettingsButton, revertSettingsButton, graphicSettingsButton, languageSettingsButton;
    bfUI::CheckBox fullscreenCheckBox;
    sf::Text numOfPlayers, fullscreenName;
    void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
    void actualizeNames();
    std::string chooseSave(State& state, const std::string& path, const std::string& ext);
    sf::VideoMode chooseResolution(State& state, const std::vector<sf::VideoMode>& validModes);
    void actualizeLoadingScreenSize() {ls.actualizeSize();}
    void setState(int8_t);
    int8_t getState() const {return state;}
    void init_functions(short& playersNum, const Data& data, State& state, my_vector2i& winSize, bool& wasFullscreen, std::string& oldLanguageName, my_vector2i& oldWinSize);
};

void newGameFunction(MainMenu& mainMenu, short& playersNum);
void loadGameFunction(MainMenu& mainMenu, const Data& data, State& state, my_vector2i winSize);
void playerNumFunction(MainMenu& mainMenu, int i, short& playersNum);
void mapSizeFunction(MainMenu& mainMenu, int i);
void revertSettingsFunction(std::vector<std::any> params);
void settingsFunction(MainMenu& mainMenu, State& state, bool& wasFullscreen, std::string& oldLanguageName, my_vector2i& winSize, my_vector2i& oldWinSize);
void fullscreenCheckBoxFunction(std::vector<std::any> params);
extern bool mainGameLoop(const Data& data, State& state, MainMenu& mainMenu, std::string fileName, short playersNum);
extern void endGameLoop(my_vector2i winSize, MainMenu& mainMenu, State& state);
extern bool loadLanguage(const std::string& name, MainMenu& mainMenu, bool isActualizing);
extern void createWindow(sf::RenderWindow& window, const sf::Vector2i& size, bool isFullscreen, State& state, tgui::Gui& mainMenuGui, sf::ContextSettings settings);
extern void generateMapFunction(MainMenu& mainMenu, const Data& data, State& state, short& playersNum, my_vector2i& winSize);
#endif // MAINMENU_H_INCLUDED

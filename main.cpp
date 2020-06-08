#include <SFML/Graphics.hpp>
#include <vector>
#include <set>
#include <fstream>
#include <string>
#include <typeinfo>
#include <ctime>
#include <cstdlib>
#include <iostream>
#include <filesystem>
//#include <thread>
//#include <mutex>
#ifdef __linux__
#include <jsoncpp/json/reader.h>
#include <jsoncpp/json/writer.h>
#include <jsoncpp/json/value.h>
#else
#include "json/json.h"
#endif
#include "include/city.hpp"
#include "include/unit.hpp"
#include "include/map.hpp"
#include "include/player.hpp"
#include "include/building.hpp"
#include "include/ui.hpp"
#include "include/path.hpp"
#include "include/basic/megaTexture.hpp"
#include "include/mainMenu.hpp"
#include "include/LoadingScreen.hpp"
#include "include/bfUI.hpp"
#include "include/Battle.h"
#include "include/Data.hpp"
#include "SelbaWard/ProgressBar.hpp"
#ifdef __linux__
//#include <X11/Xlib.h>
#endif
#define NO_ANIMATION 255
///FOR A*
int ** mapFCost; // koszty do A*
int ** mapGCost; // koszty do A*
bool ** mapClosed; // odwiedzone A*
signed char ** from;

my_vector2i destinationTile; // kafelek, do ktorego idziemy
void calculatePath(int,int);
void calculatePath(my_vector2i&& a) {calculatePath(a.x,a.y);}
bool inBounds(my_vector2i node);
///SFML Types Global
sf::Texture bfUI::uiBackground;
sf::RenderWindow appWindow;
//std::mutex appWindowMutex;
my_vector2i proposedMove;
my_vector2i mousePos;
///Own classes (not "singleton" classes)
std::vector<Unit> units;
std::vector<City> cities;
std::vector<Player> players;
///Own singleton classes
Map mapa;
UI gameUI;
Path path;
///Other global variables
std::string logStr;
std::map<std::string,sf::String> lang;
///Functions
void AppDraw(const State& state);
short loadSettings(my_vector2i& winSize, MainMenu& mainMenu, State& state); ///-1 if error, 0 or more if success
bool saveSettings(State& state);
void newTurn(State& state);
bool isDiagCrosRiv(int,int,int,int);
bool isDiagCrosRiv(const my_vector2i&,const my_vector2i&);
void ZoomIn(float& zoomOutInfo, State& state);
void ZoomOut(float& zoomOutInfo, State& state);
void showPath();
void saveLog();
bool isDrawn(const my_vector2f& position, const sf::View& view);
bool initGame(State& state, MainMenu& mainMenu, string fileName="", short playersNum=0);
void DeleteGameMemory();
void endGameLoop(my_vector2i winSize, MainMenu& mainMenu, State& state);
float countSqrDifference(my_vector2f a, my_vector2f b) {return (a.x-b.x)*(a.x-b.x)+(a.y-b.y)*(a.y-b.y);}
bool mainGameLoop(const Data& data, State& state, MainMenu& mainMenu, string fileName="", short playersNum=0); //true - end the game, false - to main menu
bool loadLanguage(const std::string& name, MainMenu& mainMenu, bool isActualizing=false);
void mainMenuEventHandling(const Data& data, MainMenu& mainMenu, State& state, short& playersNum, my_vector2i winSize, const std::vector<sf::VideoMode>& validModes, bool& wasFullscreen, my_vector2i& oldWinSize, std::string& oldLanguageName);
void createWindow(sf::RenderWindow& window, const sf::Vector2i& size, bool isFullscreen, State& state, tgui::Gui& mainMenuGui, sf::ContextSettings settings=sf::ContextSettings())
{
    sf::Vector2f fSize=static_cast<sf::Vector2f>(size);
    if(isFullscreen)
        window.create(sf::VideoMode(size.x,size.y),"TBS game",sf::Style::Fullscreen,settings);
    else
        window.create(sf::VideoMode(size.x,size.y),"TBS game",sf::Style::Titlebar | sf::Style::Close,settings);
    mainMenuGui.setTarget(window);
    std::cout << "Window created\n";
    state.view.setCenter(fSize/2.f);
    state.view.setSize(fSize);
    state.uiView.setCenter(fSize/2.f);
    state.uiView.setSize(fSize);
    appWindow.setView(state.view);
    std::cout << "Views set\n";
}
void centreOnUnit(State& state)
{
    for(std::size_t i=0u;i<units.size();i++)
        if(units[i].owner==state.actualPlayer)
        {
            state.view.setCenter(units[i].getPosition());
            return;
        }
}
void foundCity(State& state, const Data& data, int& MousePressingPhase)
{
    if(state.pressedUnitIndex<0 || typeid(*units[state.pressedUnitIndex].type.get())!=typeid(settler)) return;
    {
        if(mapa(mapToTile(units[state.pressedUnitIndex].getPosition())).level<0)
            //if(mapa(units[state.pressedUnitIndex].getPosition().x/tileSize,units[state.pressedUnitIndex].getPosition().y/tileSize).level<0)
            return;
        else if(mapa(mapToTile(units[state.pressedUnitIndex].getPosition())).isRiver)
            //else if(mapa(units[state.pressedUnitIndex].getPosition().x/tileSize,units[state.pressedUnitIndex].getPosition().y/tileSize).isRiver)
            return;
        else if(mapa(mapToTile(units[state.pressedUnitIndex].getPosition())).resource)
            //else if(mapa(units[state.pressedUnitIndex].getPosition().x/tileSize,units[state.pressedUnitIndex].getPosition().y/tileSize).resource)
            return;
        else if(mapToTile(units[state.pressedUnitIndex].getPosition()).x<6 || mapToTile(units[state.pressedUnitIndex].getPosition()).x>=mapa.getSize().x-6 ||
                mapToTile(units[state.pressedUnitIndex].getPosition()).y<6 || mapToTile(units[state.pressedUnitIndex].getPosition()).y>=mapa.getSize().x-6)
            return;
        bool tmp599=false;
        if(tmp599) return;
        for(std::size_t i=0u;i<cities.size();i++)
        {
            if(sqrTileDist(cities[i].getPosition(),units[state.pressedUnitIndex].getPosition())<=144)
            {tmp599=true;return;}
            for(int j=0;j<cities[i].getBuildingNum();j++)
                if(cities[i].getBuilding(j).getPosition()==units[state.pressedUnitIndex].getPosition())
                {tmp599=true;return;}
        }
        if(tmp599) return;
        sf::String tmpName=gameUI.insertTxt(state,"Write city name");
        if(tmpName.getSize()>0)
        {
            cities.push_back(City(data, cities.size(),state,units[state.pressedUnitIndex].owner,mapToTile(units[state.pressedUnitIndex].getPosition()),20));
            cities.back().setName(tmpName);
            cities.back().setStage(0);
            units.erase(units.begin()+state.pressedUnitIndex);
            mapa.updateOwners();
            mapa.actualizeVisibility(state.actualPlayer, state);
            mapa.actualizeInfluence();
        }
        state.pressedUnitIndex=-1;
        MousePressingPhase=0;
        path.clear();
        gameUI.actualizeTileInfo(state);
        return;
    }
}

void generateMapFunction(MainMenu& mainMenu, const Data& data, State& state, short& playersNum, my_vector2i& winSize)
{
    mapa.setSize(mainMenu.mapSize,mainMenu.mapSize);
    auto widgets=mainMenu.gui.getWidgets();
    for(size_t i=0;i<widgets.size();i++)
        widgets[i]->setVisible(false);
    if(mainGameLoop(data, state,mainMenu,"",playersNum))
        appWindow.close();
    else
        endGameLoop(winSize,mainMenu,state);
    for(size_t i=0;i<widgets.size();i++)
        widgets[i]->setEnabled(true);
}
void saveSettingsFunction(std::vector<std::any> params)
{
    //Loading parameters
    MainMenu* mainMenu=std::any_cast<MainMenu*>(params[0]);
    State* state=std::any_cast<State*>(params[1]);
    //Main function
    saveSettings(*state);
    mainMenu->setState(0);
}
void graphicSettingsFunction(std::vector<std::any> params)
{
    //Loading parameters
    MainMenu* mainMenu=std::any_cast<MainMenu*>(params[0]);
    State* state=std::any_cast<State*>(params[1]);
    auto validModes=std::any_cast<const std::vector<sf::VideoMode>*>(params[2]);
    my_vector2i winSize;
    //Main function
    mainMenu->setState(4);
    sf::VideoMode newMode=mainMenu->chooseResolution(*state, *validModes);
    if(newMode!=sf::VideoMode())
    {
        winSize=my_vector2i((uint32_t)newMode.width,(uint32_t)newMode.height);
        appWindow.close();
        createWindow(appWindow,winSize,state->isFullscreen,*state, mainMenu->gui, sf::ContextSettings());
        mainMenu->init(*state);
    }
    mainMenu->setState(3);
}
void languageSettingsFunction(std::vector<std::any> params) {
    //Loading parameters
    MainMenu *mainMenu = std::any_cast<MainMenu *>(params[0]);
    State *state = std::any_cast<State *>(params[1]);
    //Main function
    mainMenu->setState(5);
    std::string tmpLang = mainMenu->chooseSave(*state, "./data/lang/", ".json");
    if (!tmpLang.empty()) {
        state->languageName = tmpLang;
        loadLanguage(state->languageName, *mainMenu, true);
    }
    mainMenu->setState(3);
}
///Beginning of main
int main()
{
    //XInitThreads();
    LoadingScreen loadingScreen;
    srand(time(0));
    State state;
    Data data(state);
    std::cout << "Data loaded successfully\n";
    MainMenu mainMenu(data,state,&appWindow);
    std::cout << "Main menu created\n";
    my_vector2i winSize;
    short settingsInfo=loadSettings(winSize, mainMenu, state); ///last bit - is fullscreen
	if(settingsInfo==-1) {saveLog(); return EXIT_FAILURE;}
	logStr+="Settings loaded successfully\n";
    std::cout << "Video mode loading began\n";
	std::vector<sf::VideoMode> videoModes;
    videoModes = sf::VideoMode::getFullscreenModes();
    sf::VideoMode videoMode;
    for(unsigned i = 0; i < videoModes.size(); i++)
    {
        if(!videoModes[i].isValid() || videoModes[i].bitsPerPixel!=32 || videoModes[i].height<600)
        {
            videoModes.erase(videoModes.begin()+i);
            i--;
        }
    }
    std::cout << "Valid video modes found\n";
	createWindow(appWindow,winSize,state.isFullscreen,state,mainMenu.gui);
    std::cout << "Window created\n";
    ///loading resources
    bfUI::Slider::setData(data);
    bfUI::Button::setData(data);
    std::cout << "bfUI data set\n";
    Unit::setData(data);
    std::cout << "Unit data set\n";
    mapa.setData(data);
    std::cout << "Map data set\n";
    gameUI.setData(data);
    std::cout << "GameUI data set\n";
    mainMenu.init(state);
    std::cout << "MainMenu initialized\n";
    if(loadingScreen.init()) {return EXIT_FAILURE;}
    loadingScreen.isBarShown=true;
	appWindow.clear(); appWindow.draw(loadingScreen); appWindow.display();
	loadingScreen.addPoints(1);
	logStr+="Successful main menu initialization\n\n";
	loadingScreen.addPoints(3);
    loadingScreen.isBarShown=false;
    short playersNum=2;
    bool wasFullscreen;
    my_vector2i oldWinSize;
    std::string oldLanguageName;
    std::cout << "OK\n";
    mainMenu.init_functions(playersNum, data, state, winSize, wasFullscreen, oldLanguageName, oldWinSize);
    while (appWindow.isOpen())
    {
        mainMenuEventHandling(data, mainMenu,state, playersNum, winSize, videoModes, wasFullscreen, oldWinSize, oldLanguageName);
        appWindow.clear();
        appWindow.draw(loadingScreen);
        appWindow.draw(mainMenu);
        mainMenu.gui.draw();
        appWindow.display();
    }
    return EXIT_SUCCESS;
}

void mainMenuEventHandling(const Data& data, MainMenu& mainMenu, State& state, short& playersNum, my_vector2i winSize, const std::vector<sf::VideoMode>& validModes, bool& wasFullscreen, my_vector2i& oldWinSize, std::string& oldLanguageName)
{
	static bool isMouseButtonPressed=true;
    while (appWindow.pollEvent(state.event))
        {
            if(state.event.type==sf::Event::Closed)
                appWindow.close();
            else if(state.event.type==sf::Event::MouseButtonReleased)
                isMouseButtonPressed=true;
            mainMenu.gui.handleEvent(state.event);
        }
        if(mainMenu.getState()==0)
        {
            if(isMouseButtonPressed && mainMenu.escapeButton.wasPressed(state.event, appWindow))
                appWindow.close();
        }
        else if(mainMenu.getState()==1)
        {
            if(isMouseButtonPressed && mainMenu.escapeButton.wasPressed(state.event, appWindow))
            {
                isMouseButtonPressed=false;
                mainMenu.setState(0);
            }
        }
        else if(mainMenu.getState()==3)
        {
            if(isMouseButtonPressed && mainMenu.fullscreenCheckBox.checkPressed(state.event, appWindow))
            {
                isMouseButtonPressed=false;
                fullscreenCheckBoxFunction({&mainMenu,&state,&oldLanguageName});
            }
            if(isMouseButtonPressed && mainMenu.revertSettingsButton.wasPressed(state.event, appWindow))
            {
                isMouseButtonPressed=false;
                revertSettingsFunction({&mainMenu,&state,wasFullscreen,&oldLanguageName,&winSize,&oldWinSize});
            }
            if(isMouseButtonPressed && mainMenu.saveSettingsButton.wasPressed(state.event, appWindow))
            {
                isMouseButtonPressed=false;
                saveSettingsFunction({&mainMenu, &state});
            }
            if(isMouseButtonPressed && mainMenu.graphicSettingsButton.wasPressed(state.event, appWindow))
            {
                isMouseButtonPressed=false;
                graphicSettingsFunction({&mainMenu,&state,&validModes});
            }
            if(isMouseButtonPressed && mainMenu.languageSettingsButton.wasPressed(state.event, appWindow))
            {
                isMouseButtonPressed=false;
                languageSettingsFunction({&mainMenu,&state});
            }
        }

}
bool mainGameLoop(const Data& data, State& state, MainMenu& mainMenu, string fileName, short playersNum)
{
	path.init(appWindow.getSize().y/1080.f);
	if(initGame(state,mainMenu,fileName,playersNum)) {saveLog();return EXIT_FAILURE;}
	gameUI.actualizePlayer(state);
	gameUI.isGridVisible=false;
    // Start the game loop
	int MousePressingPhase=0; //0 - no unit/improvement, 1 - unit/impro chosen, 2 - move proposed
	bool isMouseButtonPressed=false;
	bool isKeyPressed=false;
    float zoomOutInfo=1.f;
	double scrollDelta=0.0;
	float deltaTime=.0f;
	sf::Clock clock;
    Battle battlefield(data);
    saveLog();
    //std::thread drawThread(AppDraw, state);
	//beginning of main game loop
    while(true)
    {
        //appWindowMutex.lock();
        my_vector2u appWindowSize=appWindow.getSize();
        while (appWindow.pollEvent(state.event))
        {
            //appWindowMutex.unlock();
            if (state.event.type == sf::Event::Closed)
            {
                /*state.signal=BFUISIG_KILL_DRAW_THREAD;
                drawThread.join();
                state.signal=0;*/
                return true;
            }
            if(!gameUI.getPause() || gameUI.getPause()==3)
            {
                if(!isKeyPressed && state.event.type == sf::Event::KeyPressed)
                {
                    isKeyPressed=true;
                    switch(state.event.key.code)
                    {
                    case sf::Keyboard::Escape:
                    case sf::Keyboard::P:
                        gameUI.setPause(true);
                        break;
                    case sf::Keyboard::F5:
                        mapa.saveToFile(state, AUTOSAVE_LOCATION);
                        break;
                    case sf::Keyboard::F6: //load autosave
                        if(gameUI.makeSure(state,"Are you sure you want\nto load autosave?"))
                            mainGameLoop(data, state, mainMenu, AUTOSAVE_LOCATION, players.size());
                        break;
                    #if SFML_VERSION_MAJOR==2 && SFML_VERSION_MINOR>=5
                    case sf::Keyboard::Enter:
                    #else
                    case sf::Keyboard::Return:
                    #endif
                        newTurn(state);
                        break;
                    case sf::Keyboard::G:
                        gameUI.isGridVisible=!gameUI.isGridVisible;
                        break;
                    case sf::Keyboard::A:
                        state.drawAll=!state.drawAll;
                        break;
                    case sf::Keyboard::I:
                        /*state.isIso=!state.isIso;
                        if(state.isIso)
                        {
                            mapa.toIso();
                            for(std::size_t i=0u;i<units.size();i++)
                            {
                                my_vector2i tmp517Tile=mapToTile(units[i].getPosition(), false);
                                units[i].setPosition(tileToMap(tmp517Tile,true));
                                //units[i].setPosition(units[i].getPosition(),true);
                            }
                            for(std::size_t i=0u;i<cities.size();i++)
                            {
                                cities[i].setPosition(tileToMap(mapToTile(cities[i].getPosition()),true));
                                for(int j=0;j<cities[i].getBuildingNum();j++)
                                    cities[i].getBuilding(j).setPosition(tileToMap(mapToTile(cities[i].getBuilding(j).getPosition()),true));
                            }
                        }
                        else
                        {
                            mapa.toTopDown();
                            for(std::size_t i=0u;i<units.size();i++)
                            {
                                my_vector2i tmp517Tile=mapToTile(units[i].getPosition(), true);
                                units[i].setPosition(tileToMap(tmp517Tile,false));
                                //units[i].setPosition(units[i].getPosition(),true);
                            }
                            for(std::size_t i=0u;i<cities.size();i++)
                            {
                                cities[i].setPosition(tileToMap(mapToTile(cities[i].getPosition(), true)));
                                for(int j=0;j<cities[i].getBuildingNum();j++)
                                    cities[i].getBuilding(j).setPosition(tileToMap(mapToTile(cities[i].getBuilding(j).getPosition(), true)));
                            }
                        }*/
                        break;
                    case sf::Keyboard::C:
                        centreOnUnit(state);
                        break;
                    /*case Keyboard::R: //building a road
                        if(state.pressedUnitIndex<0 || typeid(*units[state.pressedUnitIndex].type)!=typeid(worker)) break;
                        {
                            int8_t tmpSize967=min(mapa(units[state.pressedUnitIndex].getPosition().x/tileSize,units[state.pressedUnitIndex].getPosition().y/(int)tileSize).getRoadStage()+1,(int)roadInfo.maxStage);
                            mapa.setRoad((Vector2i)units[state.pressedUnitIndex].getPosition()/tileSize,tmpSize967);
                        }
                        break;*/
                    case sf::Keyboard::B:
                        foundCity(state,data,MousePressingPhase);
                        break;
                    default:
                        break;
                    }
                }
                else if(state.event.type != sf::Event::KeyPressed)
                    isKeyPressed=false;
                my_vector2i mapPressPosition = (my_vector2i)sf::Mouse::getPosition(appWindow)-(my_vector2i)(appWindow.getSize()/2u)+(my_vector2i)state.view.getCenter();
                if(!gameUI.getPause())
                {
                    if(sf::Mouse().getPosition(appWindow).x<=(int)appWindowSize.x-gameUI.getUiWidth()
                       && mapToTile(mapPressPosition).x>=0
                       && mapToTile(mapPressPosition).x<mapa.getSize().x
                       && mapToTile(mapPressPosition).y>=0
                       && mapToTile(mapPressPosition).y<mapa.getSize().y
                       && mapa.wasEverVisible(mapToTile(mapPressPosition),state.actualPlayer))
                    {
                        if(sf::Mouse::isButtonPressed(sf::Mouse::Left) && !isMouseButtonPressed)
                        {
                            isMouseButtonPressed = true;
                            if (MousePressingPhase == 0 && state.pressedUnitIndex < 0 && state.pressedImproIndex<0)
                            {
                                for(std::size_t i=0u; i<units.size(); i++)
                                {
                                    if(units[i].type==nullptr) continue;
                                    if(mapToTile(units[i].getPosition())==mapToTile((my_vector2f)mapPressPosition))
                                    //if ((int)(units[i].getPosition().x) / tileSize == mapPressPosition.x / tileSize && (int)(units[i].getPosition().y) / tileSize == mapPressPosition.y / tileSize)
                                    {
                                        state.pressedUnitIndex = i;
                                        MousePressingPhase = 1;
                                        break;
                                    }
                                }
                                for(std::size_t i=0u;i<cities.size();i++)
                                {
                                    if(mapToTile(cities[i].getPosition())==mapToTile((my_vector2f)mapPressPosition))
                                    {
                                        cities[i].CityView(state);
                                        break;
                                    }
                                    for(int j=0;j<cities[i].getBuildingNum();j++)
                                    {
                                        if(cities[i].getBuilding(j).type==nullptr) continue;
                                        if(mapToTile(cities[i].getBuilding(j).getPosition())==mapToTile((my_vector2f)mapPressPosition))
                                        {
                                            state.pressedImproCityIndex = i;
                                            state.pressedImproIndex = j;
                                            MousePressingPhase = 1;
                                            break;
                                        }
                                    }
                                }
                                gameUI.actualizeTileInfo(state);
                            }
                            else if(gameUI.pressedType==1 && units[state.pressedUnitIndex].type
                                    && units[state.pressedUnitIndex].owner==state.actualPlayer
                                    && (MousePressingPhase==1 ||
                                        (MousePressingPhase==2 && destinationTile!=mapToTile(mapPressPosition))))
                            //else if(gameUI.pressedType==1 && units[state.pressedUnitIndex].type && units[state.pressedUnitIndex].owner==state.actualPlayer &&
                            //(MousePressingPhase==1 || (MousePressingPhase==2 && destinationTile!=my_vector2i(mapPressPosition.x/tileSize,mapPressPosition.y/tileSize))))
                            {
                                destinationTile = mapToTile(mapPressPosition);

                                //destinationTile = my_vector2i((int)mapPressPosition.x / tileSize, (int)mapPressPosition.y / tileSize);
                                unsigned char isUnitOnTarget=0; //0 false, 1 different owner, 2 same owner
                                int distance=15;
                                for(int j=0;j<(int)units.size();j++)
                                {
                                    if(j==state.pressedUnitIndex || units[j].type==nullptr) continue;
                                    if(mapToTile(units[j].getPosition())==destinationTile)
                                    //if((int)units[j].getPosition().x/tileSize==destinationTile.x && (int)units[j].getPosition().y/tileSize==destinationTile.y)
                                    {
                                        //float tmpDis571 = sqrTileDist(units[state.pressedUnitIndex].getPosition(),units[state.targetUnitIndex].getPosition());
                                        if(units[j].owner==units[state.pressedUnitIndex].owner)
                                        {
                                            isUnitOnTarget=2; break;
                                        }
                                        isUnitOnTarget=1;
                                        path.clear();
                                        proposedMove=destinationTile;
                                        state.targetUnitIndex=j;
                                        break;
                                    }
                                }
                                //only if no unit is on that tile
                                if(isUnitOnTarget==0)
                                {
                                    state.targetUnitIndex=-1;
                                    if (inBounds(destinationTile)) calculatePath(mapToTile(units[state.pressedUnitIndex].getPosition()));
                                    //if (inBounds(destinationTile)) calculatePath(units[state.pressedUnitIndex].getPosition().x / tileSize, units[state.pressedUnitIndex].getPosition().y / tileSize);
                                    proposedMove=tileSize*mapToTile(mapPressPosition);
                                    //proposedMove = my_vector2i(tileSize*((int)mapPressPosition.x / tileSize), tileSize*((int)mapPressPosition.y / tileSize));

                                    if (inBounds(destinationTile)) distance = mapFCost[destinationTile.x][destinationTile.y];
                                    else distance = INT_MAX / 2;
                                    gameUI.setTargetPosition(state,proposedMove,distance);
                                    MousePressingPhase=2;
                                }
                                else if(isUnitOnTarget==1)
                                {
                                    if(sqrTileDist(units[state.pressedUnitIndex].getPosition(),units[state.targetUnitIndex].getPosition())<=2)
                                    //if(countSqrDifference(units[state.pressedUnitIndex].getPosition(),units[state.targetUnitIndex].getPosition())<2.1f*tileSize*tileSize)
                                        gameUI.setTargetPosition(state,proposedMove,15,AttackTypes::meleeAttack);
                                    //else gameUI.setTargetPosition(state,proposedMove,15,AttackTypes::rangedAttack);
                                    MousePressingPhase=2;
                                }
                            }
                            else if(gameUI.pressedType==1 && MousePressingPhase==2 && state.targetUnitIndex<0)
                            {
                                int distance = mapFCost[mapToTile(mapPressPosition).x][mapToTile(mapPressPosition).y];
                                //int distance = mapFCost[(int)mapPressPosition.x/tileSize][(int)mapPressPosition.y/tileSize];
                                if(distance<INT_MAX/2)
                                {
                                    //units[state.pressedUnitIndex].mp-=distance;
                                    units[state.pressedUnitIndex].setPosition(my_vector2f(proposedMove),1);
                                    MousePressingPhase=1;
                                    gameUI.unsetTarget();
                                    int i,j;
                                    bool tmpWas=true;
                                    for(i=0;i<(int)cities.size();i++)
                                    {
                                        for(int j=0;j<cities[i].getBuildingNum();j++)
                                        {
                                            if(cities[i].getBuilding(j).getPosition()==(my_vector2f)proposedMove)
                                            {tmpWas=true;break;}
                                        }
                                        if(tmpWas) break;
                                    }
                                    if(i==(int)cities.size()) {i=-1;j=-1;}
                                    state.pressedImproCityIndex=i;
                                    state.pressedImproIndex=j;
                                    gameUI.actualizeTileInfo(state);
                                    //gameUI.uiInfo(state.pressedUnitIndex);
                                    path.clear();
                                }
                            }
                            else if(gameUI.pressedType==1 && MousePressingPhase==2)
                            {
                                if(state.targetUnitIndex>=0)
                                {
                                    float tmpDis=countSqrDifference(units[state.pressedUnitIndex].getPosition(),units[state.targetUnitIndex].getPosition());
                                    MousePressingPhase=1;
                                    if(tmpDis<2.1f*tileSize*tileSize)
                                    {
                                        if(units[state.pressedUnitIndex].attack(state,units[state.targetUnitIndex],AttackTypes::meleeAttack))
                                        {
                                            uint8_t tmpDir=tileDir(units[state.pressedUnitIndex].getPosition(),units[state.targetUnitIndex].getPosition());
                                            if(battlefield.beginBattle(state, units[state.pressedUnitIndex],units[state.targetUnitIndex],tmpDir,false))
                                            {
                                                /*state.signal=BFUISIG_KILL_DRAW_THREAD;
                                                drawThread.join();
                                                state.signal=0;*/
                                                return true;
                                            }
                                            if(typeid(*(units[state.targetUnitIndex].type.get()))==typeid(officer))
                                            {
                                                if(units[state.targetUnitIndex].commandedUnits.empty())
                                                    units.erase(units.begin()+state.targetUnitIndex);
                                            }
                                            else if(units[state.targetUnitIndex].kount==0)
                                                    units.erase(units.begin()+state.targetUnitIndex);
                                            else if(units[state.pressedUnitIndex].commandedUnits.empty())
                                                    units.erase(units.begin()+state.pressedUnitIndex);
                                        }
                                        if(units[state.pressedUnitIndex].type.get()==nullptr)
                                        {
                                            state.pressedUnitIndex=-1; MousePressingPhase=0;
                                        }
                                    }
                                    //else if(typeid(*units[state.pressedUnitIndex].type)==typeid(ranged))
                                    //    units[state.pressedUnitIndex].attack(units[state.targetUnitIndex],AttackTypes::rangedAttack);
                                }
                                gameUI.unsetTarget();
                                gameUI.actualizeTileInfo(state);
                                path.clear();
                            }
                        }
                    }
                    if(sf::Mouse::isButtonPressed(sf::Mouse::Right) &&
                       (state.pressedUnitIndex>=0 || state.pressedImproIndex>=0))
                    {
                        state.targetUnitIndex=-1;
                        state.pressedUnitIndex=-1;
                        state.pressedImproIndex=-1;
                        state.pressedImproCityIndex=-1;
                        MousePressingPhase=0;
                        gameUI.pressedType=0;
                        gameUI.unsetTarget();
                        path.clear();
                        gameUI.actualizeTileInfo(state);
                    }

                    if(!isMouseButtonPressed && gameUI.pressedType!=1 &&
                       state.pressedUnitIndex>=0 && gameUI.unitButton.wasPressed(state.event, appWindow))
                    {
                        isMouseButtonPressed=true;
                        gameUI.pressedType=1;
                        gameUI.actualizeTileInfo(state);
                        gameUI.unitButton.setUnactiveColor();
                    }
                    if(!isMouseButtonPressed && gameUI.pressedType!=2 &&
                       state.pressedImproIndex>=0 && gameUI.buildButton.wasPressed(state.event, appWindow))
                    {
                        isMouseButtonPressed=true;
                        gameUI.pressedType=2;
                        MousePressingPhase=1;
                        gameUI.actualizeTileInfo(state);
                        gameUI.unitButton.setUnactiveColor();
                    }
                    if(!isMouseButtonPressed &&
                       gameUI.openImprovMenuButton.wasPressed(state.event, appWindow))
                    {
                        isMouseButtonPressed=true;
                        state.pressedUnitIndex=-1;
                        gameUI.unitInfo(state);
                        state.pressedImproIndex=-1;
                        gameUI.improInfo(state);
                        gameUI.setPause(2);
                    }
                    if(!isMouseButtonPressed && gameUI.pressedType==2 &&
                       cities[state.pressedImproCityIndex].getBuilding(state.pressedImproIndex).owner==state.actualPlayer)
                    {
                        Building& tmpBuild=cities[state.pressedImproCityIndex].getBuilding(state.pressedImproIndex);
                        if(tmpBuild.stage<tmpBuild.type->maxStage-1 &&
                       gameUI.upgradeButton.wasPressed(state.event, appWindow))
                        {
                            isMouseButtonPressed=true;
                            if(gameUI.makeSure(state, "Are you sure you want\n to upgrade the building?"))
                                cities[state.pressedImproCityIndex].improveBuildingOfIndex(state.pressedImproIndex, state);
                        }
                        if(cities[state.pressedImproCityIndex].freePop>0 && tmpBuild.employed<tmpBuild.type->maxEmployedNr[tmpBuild.stage] && gameUI.workUp.wasPressed(state.event, appWindow))
                        {
                            isMouseButtonPressed=true;
                            tmpBuild.tryAddEmployee();
                            gameUI.improInfo(state);
                        }
                        if(tmpBuild.employed>0 && gameUI.workDown.wasPressed(state.event, appWindow))
                        {
                            isMouseButtonPressed=true;
                            tmpBuild.tryRemoveEmployee();
                            gameUI.improInfo(state);
                        }
                    }
                    /*if(!isMouseButtonPressed && gameUI.pressedType==2 && buildings[state.pressedImproIndex].tmp1>0 &&
                       buildings[state.pressedImproIndex].owner==state.actualPlayer && typeid(*buildings[state.pressedImproIndex].type)==typeid(RecruitBuilding)
                       && gameUI.recruitButton.wasPressed(state))
                    {
                        gameUI.recruitSlider.setActive(true);
                        gameUI.recruitSlider.setMaxValue(buildings[state.pressedImproIndex].tmp1);
                        gameUI.recruitSlider.setValue(2);
                        gameUI.recruitNrText.setString(my_to_string(gameUI.recruitSlider.getValue())+"/"+my_to_string(gameUI.recruitSlider.getMaxValue()));
                        isMouseButtonPressed=true;
                        gameUI.setPause(4);
                    }*/
                    if(!isMouseButtonPressed && ((gameUI.pressedType==1 && units[state.pressedUnitIndex].owner==state.actualPlayer)
                    || (gameUI.pressedType==2 && cities[state.pressedImproCityIndex].owner==state.actualPlayer))
                       && gameUI.destroyButton.wasPressed(state.event, appWindow))
                    {
                        isMouseButtonPressed=true;
                        if(gameUI.makeSure(state,"Are you sure you\nwant to delete?"))
                        {
                            if(gameUI.pressedType==1)
                            {
                                std::swap(units[state.pressedUnitIndex],units.back());
                                units.pop_back();
                            }
                            else if(gameUI.pressedType==2)
                            {
                                cities[state.pressedImproCityIndex].removeBuilding(state.pressedImproIndex);
                                //std::swap(buildings[state.pressedImproIndex],buildings.back());
                                //buildings.pop_back();
                            }
                            state.pressedUnitIndex=-1;
                            state.pressedImproIndex=-1;
                            state.pressedImproCityIndex=-1;
                            gameUI.unsetTarget();
                            path.clear();
                            gameUI.actualizeTileInfo(state);
                            MousePressingPhase=0;
                        }
                    }
                }
                else
                {
                    if(sf::Mouse().getPosition(appWindow).x<=(int)appWindowSize.x-gameUI.getUiWidth())
                    {
                        if(sf::Mouse::isButtonPressed(sf::Mouse::Left) && !isMouseButtonPressed)
                        {
                            gameUI.buildPropos.setPosition(tileToMap(mapToTile(mapPressPosition)));
                            //gameUI.buildPropos.setPosition(my_vector2f((my_vector2i(mapPressPosition) / tileSize) * tileSize));
                            gameUI.checkIfCanBeBuilt(state);
                            isMouseButtonPressed=true;
                        }
                    }
                    if(!isMouseButtonPressed && gameUI.buildRet.wasPressed(state.event, appWindow))
                    {
                        isMouseButtonPressed=true;
                        gameUI.buildPropos.setTexture(data.getNullTexture());
                        auto tmp618 = state.pressedImproIndex;
                        state.pressedImproIndex=-1;
                        gameUI.improInfo(state);
                        state.pressedImproIndex=tmp618;
                        gameUI.setPause(0);
                    }
                    if(gameUI.canBeBuilt && !isMouseButtonPressed && gameUI.buildOk.wasPressed(state.event, appWindow))
                    {
                        if(gameUI.makeSure(state, "Are you sure you\nwant to build?"))
                        {
                            short tmpOwner=mapa(mapToTile(gameUI.buildPropos.getPosition())).getOwner();
                            ///find index of a city proper for the building
                            cities[tmpOwner].addBuilding(Building(data.getBuildingType(gameUI.pressedType),tmpOwner,gameUI.buildPropos.getPosition(), state, data));
                            /*buildings.back().setStage(0);*/
                            /*if(mapa(mapToTile(gameUI.buildPropos.getPosition())).cityField>=0)
                                buildings.back().setOwner(mapa(mapToTile(gameUI.buildPropos.getPosition())).cityField);
                            else
                                buildings.back().setOwner(state.actualPlayer);*/
                            //buildings.back().setPosition(gameUI.buildPropos.getPosition());*/
                            for(int i=0;i<RESOURCE_TYPES_NUMBER;i++)
                            {
                                players[state.actualPlayer].substractResource(data.getBuildingType(gameUI.pressedType)->buildingCost[0][i],i+gold);
                            }
                            gameUI.actualizePlayer(state);
                        }
                        isMouseButtonPressed=true;
                        gameUI.buildPropos.setTexture(data.getNullTexture());
                        auto tmp618 = state.pressedImproIndex;
                        state.pressedImproIndex=-1;
                        gameUI.improInfo(state);
                        state.pressedImproIndex=tmp618;
                        gameUI.setPause(0);
                    }
                }
                if(scrollDelta>=0.3 && state.event.type == sf::Event::MouseWheelScrolled/*  event.mouseWheelScroll.wheel=Mouse::VerticalWheel*/)
                {
                    //double scrollDelta=0.0;
                    if(state.event.mouseWheelScroll.delta>0) ZoomIn(zoomOutInfo,state);
                    else ZoomOut(zoomOutInfo,state);
                }
                if(!sf::Mouse::isButtonPressed(sf::Mouse::Left)) isMouseButtonPressed=false;
            }
            else if(gameUI.getPause()==1)
            {
                if(!isKeyPressed && state.event.type == sf::Event::KeyPressed && state.event.key.code==sf::Keyboard::Escape)
                {
                    /*state.signal=BFUISIG_KILL_DRAW_THREAD;
                    drawThread.join();
                    state.signal=0;*/
                    return false;
                }
                else if(!isKeyPressed && state.event.type == sf::Event::KeyPressed && state.event.key.code==sf::Keyboard::P)
                {
                    isKeyPressed=true;
                    gameUI.setPause(0);
                }
                else if(state.event.type!=sf::Event::KeyPressed)
                    isKeyPressed=false;
                if(!isMouseButtonPressed && gameUI.escapeButton.wasPressed(state.event, appWindow))
                {
                    /*state.signal=BFUISIG_KILL_DRAW_THREAD;
                    drawThread.join();
                    state.signal=0;*/
                    return false;
                }
                if(!isMouseButtonPressed && gameUI.returnToGameButton.wasPressed(state.event, appWindow))
                {
                    isMouseButtonPressed=true;
                    gameUI.setPause(false);
                }
                if(!isMouseButtonPressed && gameUI.saveGameButton.wasPressed(state.event, appWindow))
                {
                    isMouseButtonPressed=true;
                    std::string tmpName=gameUI.insertTxt(state,"Write name of save file").toAnsiString();
					if(!tmpName.empty())
					{
						tmpName=std::string("saves/")+tmpName+std::string(".sav");
						if(!std::filesystem::exists(tmpName) || gameUI.makeSure(state,"Are you sure you want to\noverwrite file?"))
							mapa.saveToFile(state, tmpName);
					}
                    //if(gameUI.makeSure(state, "Are you sure you\nwant to save the game?"))
                    //    mapa.saveToFile(state, AUTOSAVE_LOCATION);
                }
                if(!isMouseButtonPressed && gameUI.restartGameButton.wasPressed(state.event, appWindow))
                {
                    isMouseButtonPressed=true;
                    if(gameUI.makeSure(state, "Are you sure you\nwant to restart the game?"))
                        mainGameLoop(data, state, mainMenu, fileName, players.size());
                }
                if(state.event.type!=sf::Event::MouseButtonPressed)
                    isMouseButtonPressed=false;
                gameUI.escapeButton.wasPressed(state.event, appWindow);

            }
            else if(gameUI.getPause()==2)
            {
                if(state.event.type != sf::Event::KeyPressed) isKeyPressed=false;
                for(int i=0;i<std::min(55,(int)data.getBuildingTypesSize());i++)
                {
                    if (!isMouseButtonPressed && gameUI.improvementChooseButton[i].wasPressed(state.event, appWindow))
                    {
                        auto tmp618 = state.pressedImproIndex;
                        state.pressedImproIndex=-1;
                        gameUI.improInfo(state);
                        gameUI.pressedType=i;
                        state.pressedImproIndex=-2;
                        gameUI.improInfo(state);
                        state.pressedImproIndex=tmp618;
                        //gameUI.setPause(0);
                        isMouseButtonPressed=true;
                    }
                }
                if(!isMouseButtonPressed && gameUI.improvementReturnButton.wasPressed(state.event, appWindow))
                {
                    auto tmp618 = state.pressedImproIndex;
                    state.pressedImproIndex=-1;
                    gameUI.improInfo(state);
                    state.pressedImproIndex=tmp618;
                    gameUI.setPause(0);
                    isMouseButtonPressed=true;
                }
                if(!isMouseButtonPressed && gameUI.pressedType<255 && gameUI.improvementOkButton.wasPressed(state.event, appWindow))
                {
                    gameUI.setPause(3); //improvement position choosing
                    auto buildType = data.getBuildingType(gameUI.pressedType);
                    buildType->itMega->assignTexture(gameUI.buildPropos,buildType->iconIndFirst);
                    //gameUI.buildPropos.setTexture(buildingTypes[gameUI.pressedType]->icon[0]);
                    isMouseButtonPressed=true;
                }
                if(state.event.type!=sf::Event::MouseButtonPressed)
                    isMouseButtonPressed=false;
            }
            else if(gameUI.getPause()==4)
            {
                if(!isMouseButtonPressed && gameUI.recruitOkButton.wasPressed(state.event, appWindow))
                {
                    if(cities[state.pressedImproCityIndex].getBuilding(state.pressedImproIndex).recruit(state,gameUI.recruitSlider.getValue()))
                    //if(buildings[state.pressedImproIndex].recruit(state, gameUI.recruitSlider.getValue()))
                        cities[state.pressedImproCityIndex].getBuilding(state.pressedImproIndex).tmp1-=gameUI.recruitSlider.getValue();
                    gameUI.setPause(0);
                    gameUI.recruitSlider.setActive(false);
                    isMouseButtonPressed=true;
                }
                if(!isMouseButtonPressed && gameUI.recruitReturnButton.wasPressed(state.event, appWindow))
                {
                    gameUI.setPause(0);
                    gameUI.recruitSlider.setActive(false);
                    isMouseButtonPressed=true;
                }
                if(gameUI.recruitSlider.moveUpdate(state.event, appWindow))
                {
                    gameUI.recruitNrText.setString(my_to_string(gameUI.recruitSlider.getValue()));
                }
                if(state.event.type!=sf::Event::MouseButtonPressed)
                    isMouseButtonPressed=false;
            }
            mainMenu.gui.handleEvent(state.event);
        }
        //appWindowMutex.unlock();
		//mouse to edge moving
		if(!gameUI.getPause() || gameUI.getPause()==3)
        {
            mousePos=sf::Mouse().getPosition(appWindow);
            if((float)(mousePos.x)/(appWindowSize.x-gameUI.getUiWidth())<=1.0f &&
               (float)(mousePos.x)/(appWindowSize.x-gameUI.getUiWidth())>=0.0f &&
               (float)(mousePos.y)/appWindowSize.y<=1.0f &&
               (float)(mousePos.y)/appWindowSize.y>=0.0f)
            {
                if((float)(mousePos.x)/(appWindowSize.x-gameUI.getUiWidth())>0.9f || sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
                {
                    state.view.move(15.f*deltaTime*tileSize*std::sqrt(state.view.getSize().y/appWindowSize.y),.0f);
                }
                else if((float)(mousePos.x)/(appWindowSize.x-gameUI.getUiWidth())<0.1f || sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
                {
                    state.view.move(-15.f*deltaTime*tileSize*std::sqrt(state.view.getSize().y/appWindowSize.y),.0f);
                }
                if((float)(mousePos.y)/appWindowSize.y>0.9f || sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
                {
                    state.view.move(.0f,15.f*deltaTime*tileSize*std::sqrt(state.view.getSize().y/appWindowSize.y));
                }
                else if((float)(mousePos.y)/appWindowSize.y<0.1f || sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
                {
                    state.view.move(.0f,-15.f*deltaTime*tileSize*std::sqrt(state.view.getSize().y/appWindowSize.y));
                }
            }
        }
        deltaTime = clock.restart().asSeconds();
        scrollDelta+=deltaTime;
        if(state.drawState)
        {
            state.drawState=0;
        }
        AppDraw(state);
    }
}
void AppDraw(const State& state)
{
    appWindow.clear();
        appWindow.setView(state.view);
        mapa.draw(state, appWindow);
        if(gameUI.isGridVisible)
            appWindow.draw(gameUI.grid);
        for(std::size_t i=0u;i<cities.size();i++)
        {
            for(int j=0;j<cities[i].getBuildingNum();j++)
            {
                if(!state.isIso && !isDrawn(cities[i].getBuilding(j).getPosition(), state.view))
                    continue;
                if(state.drawAll || mapa.wasEverVisible(mapToTile(cities[i].getBuilding(j).getPosition()),state.actualPlayer))
                {
                    appWindow.draw(cities[i].getBuilding(j));
                }
            }
            if(!state.isIso && !isDrawn(cities[i].getPosition(), state.view))
                continue;
            //if(!isDrawn((my_vector2f)cities[i].getPosition(), state,state.view))
            //    continue;
            if(state.drawAll || mapa.wasEverVisible(mapToTile(cities[i].getPosition()),state.actualPlayer))
            {
                appWindow.draw(cities[i]);
            }
        }
        for(std::size_t i=0u;i<units.size();i++)
        {
            if(!state.isIso && !isDrawn(units[i].getPosition(), state.view))
                continue;
            if(state.drawAll || mapa.wasEverVisible(mapToTile(units[i].getPosition()),state.actualPlayer))
            {
                appWindow.draw(units[i]);
            }
        }
        mapa.draw(state,appWindow, true);
        if(state.drawState!=CITY_VIEW && state.drawState!=NO_UI)
        {
            appWindow.draw(path);
            appWindow.draw(gameUI.targetCircle);
        }
        appWindow.setView(state.uiView);
        if(state.drawState!=CITY_VIEW && state.drawState!=NO_UI)
        {
            gameUI.draw(state,appWindow);
            if(state.drawState==MAKE_SURE_STATE)
            {
                appWindow.draw(gameUI.makeSureBack);
                appWindow.draw(gameUI.makeSureText);
                appWindow.draw(gameUI.makeSureYesButton);
                appWindow.draw(gameUI.makeSureNoButton);
            }
            else if(state.drawState==INSERT_TEXT_STATE)
            {
                appWindow.draw(gameUI.makeSureBack);
                appWindow.draw(gameUI.makeSureText);
                appWindow.draw(gameUI.insertText);
            }
            appWindow.draw(mapa.miniMapSprite);
        }
        else
        {
            gameUI.draw(state,appWindow);
        }
        //appWindowMutex.lock();
        appWindow.display();
        //appWindowMutex.unlock();
}
bool isDrawn(const my_vector2f& position, const sf::View& view)
{
    my_vector2i tilePos = mapToTile(position);
    my_vector2i viewUpLeft = mapToTile(view.getCenter()-view.getSize()/2.f);
    my_vector2i viewBotRight = mapToTile(view.getCenter()+view.getSize()/2.f);
    if(tilePos.x>=viewUpLeft.x && tilePos.x<=viewBotRight.x && tilePos.y>=viewUpLeft.y && tilePos.y<=viewBotRight.y)
    //if(position.x<(myView.getCenter().x-myView.getSize().x/2.f)-mainMega.getTileSize() || position.x>myView.getCenter().x+myView.getSize().x/2.f ||
           //position.y<(myView.getCenter().y-myView.getSize().y/2.f)-mainMega.getTileSize() || position.y>myView.getCenter().y+myView.getSize().y/2.f)
           return true;
    return false;
}
void newTurn(State& state)
{
    if(state.actualPlayer+1==(int)players.size())
    {
        state.actualPlayer=0;
        for(std::size_t i=0u;i<units.size();i++)
            if(units[i].type!=nullptr) units[i].newTurn();
        //for(int i=0;i<buildings.size();i++)
        //    buildings[i].newTurn();
        for(std::size_t i=0u;i<cities.size();i++)
            cities[i].newTurn();
    }
    else
        state.actualPlayer++;
    mapa.actualizeMiniMap(state.actualPlayer);
    gameUI.actualizePlayer(state);
}

bool initGame(State& state, MainMenu& mainMenu, string fileName, short playersNum)
{
    if(fileName=="")
    {
        int tmpSizeX=mapa.getSize().x;
        int tmpSizeY=mapa.getSize().y;
        for(int i=0;i<playersNum;i++)
            players.push_back(Player());
        //try {players=new Player[playersNum];} catch(...) {return EXIT_FAILURE;}
        for(int i=0;i<playersNum;i++)
        {
            switch(i)
            {
                case 0: players[0].playerColor=sf::Color::Red;break;
                case 1: players[1].playerColor=sf::Color::Blue;break;
                case 2: players[2].playerColor=sf::Color(0,127,0);break;
                case 3: players[3].playerColor=sf::Color::Cyan;break;
                case 4: players[4].playerColor=sf::Color::Yellow;break;
                case 5: players[5].playerColor=sf::Color::Magenta;break;
                case 6: players[6].playerColor=sf::Color::White;break;
                case 7: players[7].playerColor=sf::Color::Black;break;
            }
            players[i].resourceQuantity[0]=5000;
            players[i].resourceQuantity[1]=100;
            players[i].resourceQuantity[2]=50;
            players[i].resourceQuantity[3]=0;
            players[i].resourceQuantity[4]=0;
            players[i].resourceQuantity[5]=0;
            players[i].isComp=i%2;
            players[i].name="Player_"+my_to_string(i);
        }
        state.actualPlayer=0;
        state.actualTurn=0;
        std::cout << "Beginning generation...\n";
        if(mapa.generate(tmpSizeX,tmpSizeY,state)) {logStr+="[ERROR]Map generating error\n"; saveLog();return EXIT_FAILURE;}
    }
    else
    {
        if(mapa.loadFromFile(state,fileName)) {logStr+="[ERROR]Map loading error\n"; return EXIT_FAILURE;}
    }
	logStr+="Map loaded successfully\n";
	//allocating memory for dynamic tables
	{
    try
	{
        mapFCost = new int*[mapa.getSize().x];
        mapGCost = new int*[mapa.getSize().x];
        mapClosed = new bool*[mapa.getSize().x];
        from = new signed char*[mapa.getSize().x];
	}
	catch(...)
	{
	    delete[] mapClosed; delete[] mapGCost;  delete[] mapFCost;
	    mapClosed=nullptr;  mapGCost=nullptr;   mapFCost=nullptr;
	    return EXIT_FAILURE;
	}
	for (int i = 0; i < mapa.getSize().x; i++)
	{
	    try
		{
            mapFCost[i] = new int[mapa.getSize().y];
            mapGCost[i] = new int[mapa.getSize().y];
            mapClosed[i] = new bool[mapa.getSize().y];
            from[i] = new signed char[mapa.getSize().y];
		}
		catch(...)
		{
            logStr+="[ERROR]Memory allocation error\n";
		    for(int j=0;j<=i;j++)
            {
                delete[] from[j]; delete[] mapClosed[j]; delete[] mapGCost[j]; delete[] mapFCost[j];
            }
            delete[] from; delete[] mapClosed; delete[] mapGCost; delete[] mapFCost;
            from=nullptr; mapClosed=nullptr; mapGCost=nullptr; mapFCost=nullptr;
            return EXIT_FAILURE;
		}
	}
	}
	logStr+="Memory allocated successfully\n";
	if (gameUI.init(state.uiView))
	{
	    logStr+="[ERROR]gameUI initialization error!\n";
		saveLog(); return EXIT_FAILURE;
	}
	logStr+="gameUI initialized successfully\n";
    return EXIT_SUCCESS;
}
void endGameLoop(my_vector2i winSize, MainMenu& mainMenu, State& state)
{
    state.view.setCenter(my_vector2f(winSize)/2.f);
    state.view.setSize(my_vector2f(winSize));
    appWindow.setView(state.view);
    units.clear();
    logStr+="Unit table cleared\n";
    //buildings.clear();
    //logStr+="Buildings table cleared\n";
    mapa.clear();
    players.clear();
    DeleteGameMemory();
    cities.clear();
    logStr+="Cities table cleared\n";
    mainMenu.setState(0);
}
/// ************************************* WYSZUKIWANIE SCIEZKI *****************************************************
///written by Michal Machnio
bool compare(const my_vector2i& node1, const my_vector2i& node2)
{
		return mapFCost[node1.x][node1.y] < mapFCost[node2.x][node2.y];
}

bool inBounds(my_vector2i node)
{
	if (node.x >= 0 && node.x < mapa.getSize().x && node.y >= 0 && node.y < mapa.getSize().y)
	{
        return mapa(node.x, node.y).tileValue(0) != INT_MAX / 2;
    }
	return false;
}

int heurestic(my_vector2i s, my_vector2i e)
{
	int dx = abs(s.x - e.x);
	int dy = abs(s.y - e.y);
	return 20 * (dx + dy) + (28 - (2 * 20)) * std::min(dx, dy);
}

void calculatePath(int startX, int startY)
{
	std::multiset<my_vector2i, bool(*)(const my_vector2i&, const my_vector2i&)> openList(compare);
	for (int i = 0; i < mapa.getSize().x; i++)
	{
		for (int j = 0; j < mapa.getSize().y; j++)
		{
			mapFCost[i][j] = INT_MAX / 2;
			mapGCost[i][j] = INT_MAX / 2;
			mapClosed[i][j] = 0;
			from[i][j]=127; //from went to the point i,j
		}
	}

	mapGCost[startX][startY] = 0;
	mapFCost[startX][startY] = 0;

	openList.insert(my_vector2i(startX, startY));

	while (!openList.empty())
	{
		my_vector2i current = *openList.begin();
		openList.erase(openList.begin());
		mapClosed[current.x][current.y] = 1;

		if (current == destinationTile)
		{
            if(mapFCost[destinationTile.x][destinationTile.y]<1000000)
                showPath();
            return; // found path
		}

		for (int i = -1; i <= 1; i++)
		{
			for (int j = -1; j <= 1; j++)
			{
				if (i == 0 && j == 0) continue;
				my_vector2i neighbor(current.x + i, current.y + j);
				if (!inBounds(neighbor)) continue;
				if (mapClosed[neighbor.x][neighbor.y]) continue;

				int candidateGCost = mapGCost[current.x][current.y] +
					mapa(neighbor.x,neighbor.y).tileValue(mapa(current.x,current.y).level,i+j==0 || abs(i+j)==2,isDiagCrosRiv(current.x,current.y,i,j),mapa(current.x,current.y).getRoadStage());
				if (openList.find(neighbor) == openList.end()) // not in openset
				{
					mapGCost[neighbor.x][neighbor.y] = candidateGCost;
					int hCost = heurestic(neighbor, destinationTile);
					mapFCost[neighbor.x][neighbor.y] = candidateGCost+hCost;
					from[neighbor.x][neighbor.y]=(i+1)*3+(j+1)%3;
					openList.insert(neighbor);
				}
				else
				{
					if (mapGCost[neighbor.x][neighbor.y] > candidateGCost)
					{
						mapGCost[neighbor.x][neighbor.y] = candidateGCost;
						int hCost = heurestic(neighbor, proposedMove);
						mapFCost[neighbor.x][neighbor.y] = candidateGCost + hCost;
                        from[neighbor.x][neighbor.y]=(i+1)*3+(j+1)%3;
						openList.insert(neighbor);
					}
				}
			}
		}
	}
}

bool isDiagCrosRiv(int fromX,int fromY,int dirX,int dirY)
{
    return fromX==-100 && fromY==-100 && dirX==-100 && dirY==-100;
     //return mapa(fromX+dirX,fromY).isRiver && mapa(fromX,fromY+dirY).isRiver;
}

/// ************************************* WYSZUKIWANIE SCIEZKI *****************************************************
bool isDiagCrosRiv(const my_vector2i& from, const my_vector2i& to)
{
     return isDiagCrosRiv(from.x,from.y,to.x-from.x,to.y-from.y);
}
short loadSettings(my_vector2i& winSize, MainMenu& mainMenu, State& state)
{
	std::ifstream settingsFile;
	settingsFile.open("settings.json");
	//sleep(seconds(1.f));
	if(!settingsFile.good())
    {
        winSize.x=800;
        winSize.y=600;
        state.isFullscreen=false;
        return 1;
    }
	Json::Value value;
	settingsFile >> value;
    settingsFile.close();
    state.isFullscreen=value.get("is_fullscreen",false).asBool();
    winSize.x=value.get("screen_x",800).asInt();
    winSize.y=value.get("screen_y",600).asInt();
    state.languageName=value.get("language","english").asString();
	/*if(*/loadLanguage("data/lang/"+state.languageName+".json",mainMenu,false)/*)*/;
        //return -1;
	return 0;
}
bool saveSettings(State& state)
{
	Json::Value saveValue;
	saveValue["is_fullscreen"]=state.isFullscreen;
    saveValue["screen_x"]=appWindow.getSize().x;
    saveValue["screen_y"]=appWindow.getSize().y;
    saveValue["language"]=state.languageName;
    std::ofstream settingsFile;
	settingsFile.open("settings.json");
    if(!settingsFile.good())
        return EXIT_FAILURE;
    settingsFile << saveValue << '\n';
    settingsFile.close();
    return EXIT_SUCCESS;
}

void ZoomIn(float& zoomOutInfo, State& state)
{
	if(zoomOutInfo>1.1f)
	{
		state.view.zoom(.7937005259871696f);
		zoomOutInfo*=.7937005259871696f;
	}
}
void ZoomOut(float& zoomOutInfo, State& state)
{
	if(zoomOutInfo<15.8f)
	{
		state.view.zoom(1.25992104989f);
		zoomOutInfo*=1.25992104989f;
	}
}
void showPath()
{
    path.clear();
    short actX=destinationTile.x, actY=destinationTile.y, i;
    for(i=0;i<MAX_PATH_LENGTH;i++)
    {
        switch(from[actX][actY])
        {
        case 0:
            actX++; actY++; break;
        case 1:
            actX++; break;
        case 2:
            actX++; actY--; break;
        case 3:
            actY++; break;
        case 5:
            actY--; break;
        case 6:
            actX--; actY++; break;
        case 7:
            actX--; break;
        case 8:
            actX--; actY--; break;
        default:
            path.setUnvisible(i-1);
            return;
        }
        path.setVisible(i);
        path.setIndPosition(i,tileToMap(actX,actY));
        //path.setIndPosition(i,actX*tileSize,actY*tileSize);
    }
    //path.setUnvisible(i);
}
void DeleteGameMemory()
{
    if(mapFCost!=nullptr)
    {
        for(int i=0;i<mapa.getSize().x;i++)
            delete[] mapFCost[i];
        delete[] mapFCost;
        mapFCost = nullptr;
    }
    logStr+="MapFCost cleared\n";
    if(mapGCost!=nullptr)
    {
        for(int i=0;i<mapa.getSize().x;i++)
            delete[] mapGCost[i];
        delete[] mapGCost;
        mapGCost = nullptr;
    }
    logStr+="MapGCost cleared\n";
    if(mapClosed!=nullptr)
    {
        for(int i=0;i<mapa.getSize().x;i++)
            delete[] mapClosed[i];
        delete[] mapClosed;
        mapClosed = nullptr;
    }
    logStr+="MapClosed cleared\n";
    if(from!=nullptr)
    {
        for(int i=0;i<mapa.getSize().x;i++)
            delete[] from[i];
        delete[] from;
        from = nullptr;
    }
    logStr+="From cleared\n";
}
void saveLog()
{
    std::ofstream logFile;
    logFile.open("log.txt");
    if(!logFile.good()) return;
    logFile << logStr;
    logFile.close();
}
int myRand(int minV, int maxV)
{
    int tmpUp = rand()%(maxV-minV+1);
    int tmpDwn = rand()%(maxV-minV+1);
    return (minV+maxV)/2+tmpUp-tmpDwn+rand()%(1+(maxV-minV)%2);
}
void saveText(std::ostream& out, const sf::String& text)
{
	const uint32_t* data = text.getData();
	for(std::size_t i=0u;i<std::min(text.getSize(),(std::size_t)256u);i++)
	{
	    write(out,(data[i]>>16));
	    write(out,(data[i]%0x10000));
	}
}
sf::String loadText(std::istream& in, uint16_t size)
{
	uint16_t tmp1, tmp2;
	uint32_t* data=new uint32_t[size];
	for(int i=0;i<size;i++)
	{
        tmp1=load(in);
        tmp2=load(in);
		//in >> /*std::hex >>*/ tmp1 >> tmp2;
		data[i]=uint32_t(tmp1)<<16;
		data[i]+=tmp2;
	}
	return sf::String::fromUtf32(data,data+size);
}
bool loadLanguage(const std::string& name, MainMenu& mainMenu, bool isActualizing)
{
    std::ifstream file(name);
    if(!file.good())
        return EXIT_FAILURE;
    Json::Value value;
    file >> value;
    lang.clear();
    for(int i=0;i<(int)value.size();i++)
    {
        std::string tmpVal=value[i]["value"].asString();
        lang.insert_or_assign(value[i]["key"].asString(), sf::String::fromUtf8(tmpVal.begin(),tmpVal.end()));
    }
    if(isActualizing)
    {
        mainMenu.actualizeNames();
    }
    std::cout << lang.size() << " " << value.size() << "\n";
    return EXIT_SUCCESS;
}

sf::String Lang(const std::string& val)
{
    sf::String tmp;
    try {tmp=lang.at(val);}
    catch(...) {return val;}
    return tmp;
}
my_vector2i mapToTile(const my_vector2f& v, bool isIso) ///ISO: 128x64 tile
{
    ///Non-iso
    if(!isIso)
        return my_vector2i(v)/tileSize;
    ///Iso
    else
        return my_vector2i(2*(int)v.y+(int)v.x,2*(int)v.y)/128;
    //int x = (int)(v.x/tileSize-v.y/(tileSize*2.f)-.5f);
    //int y = (int)(-v.x/tileSize-v.y/(tileSize*2.f)-.5f);
}
my_vector2f tileToMap(int x, int y, bool isIso) ///ISO: 128x64 tile
{
    ///Non-iso
    if(!isIso)
        return my_vector2f(x,y)*(float)tileSize;
    else
    {
        return my_vector2f((x-y)*64.f,(x+y-mapa(x,y).level)*32.f);
    }
    ///Iso
    //int x = (int)(v.x/tileSize-v.y/(tileSize*2.f)-.5f);
    //int y = (int)(-v.x/tileSize-v.y/(tileSize*2.f)-.5f);
}

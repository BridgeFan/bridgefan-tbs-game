#include "mainMenu.hpp"
#include "basic/constants.hpp"
#include "LoadingScreen.hpp"
#include "basic/megaTexture.hpp"
#include "Data.hpp"
#include <filesystem>
#include <iostream>
#include <fstream>
#include <algorithm>
#include <chrono>

namespace fs=std::filesystem;


MainMenu::MainMenu(const Data& data, const State& state, const sf::Window* window): data(data), list(window)
{
    //if(init(state))
    //    throw std::exception();
}
void MainMenu::init(const State& mainState)
{
	float uiScale = appWindow.getSize().y/1080.f;
	float rat = appWindow.getSize().x/appWindow.getSize().y;
	float y_size=appWindow.getSize().y;
    ls.actualizeSize();
    tgui::Layout2d defSize=tgui::Layout2d(0.4f*y_size,"10%");
	///NEW GAME BUTTON
	newGameButton=tgui::Button::create();
	newGameButton->setSize(defSize);
	newGameButton->setPosition("40%","5%");
	newGameButton->setInheritedFont(data.getFont());
    newGameButton->setTextSize(60.f*uiScale);
    gui.add(newGameButton,"newGame");
	///LOAD GAME BUTTON
    loadGameButton=tgui::Button::create();
    loadGameButton->setSize(defSize);
    loadGameButton->setPosition("40%","20%");
    loadGameButton->setInheritedFont(data.getFont());
    loadGameButton->setTextSize(60.f*uiScale);
    gui.add(loadGameButton,"loadGame");
	///SETTINGS BUTTON
    settingsButton=tgui::Button::create();
    settingsButton->setSize(defSize);
    settingsButton->setPosition("40%","35%");
    settingsButton->setInheritedFont(data.getFont());
    settingsButton->setTextSize(60.f*uiScale);
    gui.add(settingsButton,"settings");
    ///PLAYER NUM BUTTONS (NEW GAME)
    for(int i=0;i<8;i++)
    {
        playerNumButton[i]=tgui::Button::create();
        playerNumButton[i]->setSize(my_to_string(10.f/rat)+"%","10%");
        playerNumButton[i]->setPosition(my_to_string(10.f*(i+1)/rat)+"%","35%");
        playerNumButton[i]->setInheritedFont(data.getFont());
        playerNumButton[i]->setTextSize(60.f*uiScale);
        playerNumButton[i]->setText(my_to_string(i+1));
        gui.add(playerNumButton[i],"playerNum"+my_to_string(i+1));
    }
    ///MAP SIZE BUTTONS (NEW GAME)
    for(int i=0;i<8;i++) {
        mapSizeButton[i]=tgui::Button::create();
        mapSizeButton[i]->setSize(my_to_string(10.f/rat)+"%","10%");
        mapSizeButton[i]->setPosition(my_to_string(10.f*(i+1)/rat)+"%","50%");
        mapSizeButton[i]->setInheritedFont(data.getFont());
        mapSizeButton[i]->setTextSize(60.f*uiScale);
        mapSizeButton[i]->setText(my_to_string((i+1)*32));
        gui.add(mapSizeButton[i],"mapSize"+my_to_string((i+1)*32));
    }
    ///GENERATE MAP BUTTON (NEW GAME)
    generateMapButton=tgui::Button::create();
    generateMapButton->setSize(defSize);
    generateMapButton->setPosition("40%","65%");
    generateMapButton->setInheritedFont(data.getFont());
    generateMapButton->setTextSize(60.f*uiScale);
    gui.add(generateMapButton,"settings");
    ///ACTUALIZE NAMES
    actualizeNames();
	///ESCAPE BUTTON
    escapeButton.setSize(1080.f*.4f,1080.f*.1f);
    escapeButton.setPosition(appWindow.getSize().x/2.f-1080.f*.2f*uiScale,appWindow.getSize().y*.8f);
    escapeButton.unactiveColor=sf::Color::Red;
    escapeButton.activeColor=sf::Color(255,196,196);
    escapeButton.setOutlineColor(sf::Color::Black);
    escapeButton.setOutlineThickness(5.f);
    escapeButton.setScale(uiScale,uiScale);
    escapeButton.text.setFont(data.getFont());
    escapeButton.text.setFillColor(sf::Color::Black);
    escapeButton.setCharacterSize(60.f*uiScale);
    escapeButton.updateTextTransform();
    ///UP BUTTON
    //if(!upTexture.loadFromFile("data/up.png"))
	//	return EXIT_FAILURE;
    upButton.setSize(64.f,64.f);
    data.getUiMegaTexture().assignTexture(upButton,upID,upID+1);
    upButton.unactiveColor=sf::Color::White;
    upButton.activeColor=sf::Color(196,196,196);
    upButton.setPosition(appWindow.getSize().x*.8f,appWindow.getSize().y*.45f);
    upButton.setActive(false);
    upButton.setOutlineThickness(0.f);
    upButton.setScale(uiScale,uiScale);
    ///DOWN BUTTON
    downButton.setSize(64.f,64.f);
    data.getUiMegaTexture().assignTexture(downButton,downID,downID+1);
    downButton.unactiveColor=sf::Color(196,196,196);
    downButton.activeColor=sf::Color::White;
    downButton.setPosition(appWindow.getSize().x*.8f,appWindow.getSize().y*.55f);
    downButton.setActive(false);
    downButton.setOutlineThickness(0.f);
    downButton.setScale(uiScale,uiScale);
	///NUM OF PLAYERS TEXT
    numOfPlayers.setPosition(appWindow.getSize().x/2.f-1080.f*.2f*uiScale,appWindow.getSize().y*.05f);
    numOfPlayers.setCharacterSize(80.f*uiScale);
    numOfPlayers.setFillColor(sf::Color::White);
    numOfPlayers.setFont(data.getFont());
    ///GRAPHICS SETTING BUTTON
    graphicSettingsButton.setSize(1080.f*.4f,1080.f*.1f);
    graphicSettingsButton.setPosition(appWindow.getSize().x/2.f-1080.f*.2f*uiScale,appWindow.getSize().y*.2f);
    graphicSettingsButton.unactiveColor=sf::Color(196,196,196);
    graphicSettingsButton.activeColor=sf::Color::White;
    graphicSettingsButton.setTexture(bfUI::uiBackground);
    graphicSettingsButton.setOutlineColor(sf::Color::Black);
    graphicSettingsButton.setOutlineThickness(5.f);
    graphicSettingsButton.setScale(uiScale,uiScale);
    graphicSettingsButton.text.setFont(data.getFont());
    graphicSettingsButton.text.setFillColor(sf::Color::Black);
    graphicSettingsButton.setCharacterSize(60.f*uiScale);
    graphicSettingsButton.updateTextTransform();
    ///LANGUAGE SETTINGS BUTTON
    languageSettingsButton.setSize(1080.f*.4f,1080.f*.1f);
    languageSettingsButton.setPosition(appWindow.getSize().x/2.f-1080.f*.2f*uiScale,appWindow.getSize().y*.35f);
    languageSettingsButton.unactiveColor=sf::Color(196,196,196);
    languageSettingsButton.activeColor=sf::Color::White;
    languageSettingsButton.setTexture(bfUI::uiBackground);
    languageSettingsButton.setOutlineColor(sf::Color::Black);
    languageSettingsButton.setOutlineThickness(5.f);
    languageSettingsButton.setScale(uiScale,uiScale);
    languageSettingsButton.text.setFont(data.getFont());
    languageSettingsButton.text.setFillColor(sf::Color::Black);
    languageSettingsButton.setCharacterSize(60.f*uiScale);
    languageSettingsButton.updateTextTransform();
    ///FULLSCREEN CHECK BOX
    fullscreenCheckBox.setSize(64.f,64.f);
    data.getUiMegaTexture().assignTexture(fullscreenCheckBox,checkBoxOff,checkBoxOn);
    fullscreenCheckBox.setChecked(mainState.isFullscreen);
    fullscreenCheckBox.setPosition(appWindow.getSize().x/2.f+1080.f*.3f*uiScale,appWindow.getSize().y*.5f);
    fullscreenCheckBox.unactiveColor=sf::Color(196,196,196);
    fullscreenCheckBox.activeColor=sf::Color::White;
    ///FULLSCREEN TEXT
    fullscreenName.setPosition(appWindow.getSize().x/2.f-1080.f*.5f*uiScale,appWindow.getSize().y*.5f);
    fullscreenName.setCharacterSize(80.f*uiScale);
    fullscreenName.setFillColor(sf::Color::Black);
    fullscreenName.setOutlineColor(sf::Color::White);
    fullscreenName.setOutlineThickness(2.f);
    fullscreenName.setFont(data.getFont());
    ///SAVE SETTINGS BUTTON
    saveSettingsButton.setSize(1080.f*.4f,1080.f*.1f);
    saveSettingsButton.setPosition(appWindow.getSize().x/2.f-1080.f*.2f*uiScale,appWindow.getSize().y*.65f);
	saveSettingsButton.unactiveColor=sf::Color(196,196,196);
	saveSettingsButton.activeColor=sf::Color::White;
	saveSettingsButton.setTexture(bfUI::uiBackground);
    saveSettingsButton.setOutlineColor(sf::Color::Black);
    saveSettingsButton.setOutlineThickness(5.f);
    saveSettingsButton.setScale(uiScale,uiScale);
    saveSettingsButton.text.setFont(data.getFont());
    saveSettingsButton.text.setFillColor(sf::Color::Black);
    saveSettingsButton.setCharacterSize(60.f*uiScale);
    saveSettingsButton.updateTextTransform();
    ///REVERT SETTINGS BUTTON
    revertSettingsButton.setSize(1080.f*.4f,1080.f*.1f);
    revertSettingsButton.setPosition(appWindow.getSize().x/2.f-1080.f*.2f*uiScale,appWindow.getSize().y*.8f);
    revertSettingsButton.unactiveColor=sf::Color::Red;
    revertSettingsButton.activeColor=sf::Color(255,196,196);
    revertSettingsButton.setOutlineColor(sf::Color::Black);
    revertSettingsButton.setOutlineThickness(5.f);
    revertSettingsButton.setScale(uiScale,uiScale);
    revertSettingsButton.text.setFont(data.getFont());
    revertSettingsButton.text.setFillColor(sf::Color::Black);
    revertSettingsButton.setCharacterSize(60.f*uiScale);
    revertSettingsButton.updateTextTransform();
    setState(0);
}
void MainMenu::setState(int8_t a) {
    state=a;
    auto widgets=gui.getWidgets();
    for(size_t i=0;i<widgets.size();i++)
        widgets[i]->setVisible(false);
    switch(a) {
        case 0:
            newGameButton->setVisible(true);
            loadGameButton->setVisible(true);
            settingsButton->setVisible(true);
            break;
        case 1:
            for(int i=0;i<8;i++) {
                playerNumButton[i]->setVisible(true);
                mapSizeButton[i]->setVisible(true);
            }
            generateMapButton->setVisible(true);
            playerNumButton[1]->setEnabled(false);
            mapSizeButton[3]->setEnabled(false);
            break;
        default:
            break;
    }
}
void MainMenu::init_functions(short& playersNum, const Data& data, State& state, my_vector2i& winSize, bool& wasFullscreen, std::string& oldLanguageName, my_vector2i& oldWinSize) {
    newGameButton->connect("pressed", newGameFunction, std::ref(*this), std::ref(playersNum));
    loadGameButton->connect("pressed", loadGameFunction, std::ref(*this), std::ref(data), std::ref(state), winSize);
    settingsButton->connect("pressed", settingsFunction, std::ref(*this), std::ref(state), std::ref(wasFullscreen), std::ref(oldLanguageName), std::ref(winSize), std::ref(oldWinSize));
    generateMapButton->connect("pressed",generateMapFunction, std::ref(*this), std::ref(data), std::ref(state), std::ref(playersNum), std::ref(winSize));
    for(int i=0;i<8;i++) {
        playerNumButton[i]->connect("pressed", playerNumFunction, std::ref(*this), i, std::ref(playersNum));
        mapSizeButton[i]->connect("pressed", mapSizeFunction, std::ref(*this), i);
    }
}
void MainMenu::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    if(state==0) //main menu
    {
        target.draw(escapeButton, states);
    }
    else if(state==1) //new game menu
    {
        target.draw(numOfPlayers, states);
        target.draw(escapeButton, states);
    }
    else if(state==2 || state==5)
	{
        target.draw(escapeButton, states);
	}
    else if(state==3)
	{
        target.draw(revertSettingsButton, states);
        target.draw(fullscreenCheckBox, states);
        target.draw(fullscreenName, states);
        target.draw(saveSettingsButton, states);
        target.draw(graphicSettingsButton, states);
        target.draw(languageSettingsButton, states);
	}
	else if(state==4)
	{
        target.draw(escapeButton, states);
	}
}
std::string MainMenu::chooseSave(State& state, const std::string& path, const std::string& ext)
{
    std::vector<std::pair<std::string, std::filesystem::file_time_type> > names;
	//std::string path = "./saves/";
	//std::string ext = ".sav";
    for (const auto & entry : std::filesystem::directory_iterator(path))
		if(entry.path().extension()==ext)
		{
            names.push_back({entry.path().stem().string(),std::filesystem::last_write_time(entry)});
		}

    if(this->state==2)
        std::sort(names.begin(),names.end(),[](const auto& a, const auto& b)
			{if(a.second!=b.second) return a.second>b.second; return a.first<b.first;});
    else
        std::sort(names.begin(),names.end(),[](const auto& a, const auto& b)
			{return a.first<b.first;});

    std::vector<std::string> info;
	if(this->state==2)
	{
        std::fstream file;
        for(std::size_t i=0u;i<names.size();i++)
        {
            std::string fileName=path+names[i].first+ext;
            file.open(fileName, std::fstream::in | std::ifstream::binary);
            if(file.good())
            {
                info.push_back(my_to_string(load(file))+std::string("x")+my_to_string(load(file)));
            }
            else
            {
                names.erase(names.begin()+i);
                i--;
            }
            file.close();
        }
    }
	texts.clear();
	dateTexts.clear();
	int savesPage=0;
	//int savesPageSize=std::ceil(names.size()/14.f);
	float scale=appWindow.getSize().y/1080.f;
	for(int i=0;i<std::min(14,(int)names.size());i++)
	{
		texts.push_back(sf::Text(names[i].first,data.getFont(),60));
		texts[i].setFillColor(sf::Color::Black);
		texts[i].setOutlineThickness(6.f);
		texts[i].setScale(scale,scale);
		texts[i].setOutlineColor(sf::Color::Transparent);
		sf::FloatRect fr=texts[i].getLocalBounds();
		texts[i].setOrigin(fr.left + fr.width/2.0f, .0f);
		texts[i].setPosition(my_vector2f(appWindow.getSize().x*.4f,i*scale*60.f));
		if(this->state==2)
		{
            dateTexts.push_back(sf::Text(info[i],data.getFont(),60));
            dateTexts[i].setFillColor(sf::Color::Black);
            dateTexts[i].setOutlineThickness(6.f);
            dateTexts[i].setScale(scale,scale);
            dateTexts[i].setOutlineColor(sf::Color::Transparent);
            fr=dateTexts[i].getLocalBounds();
            dateTexts[i].setOrigin(fr.left + fr.width/2.0f, .0f);
            dateTexts[i].setPosition(my_vector2f(appWindow.getSize().x*.6f,i*scale*60.f));
        }
        else
            texts[i].move(appWindow.getSize().x*.1f,.0f);
	}
    upButton.setActive(true);
    downButton.setActive(true);
	bool isMouseButtonPressed=false;
	my_vector2f tmpMousePos;
	while(true)
	{
		tmpMousePos=appWindow.mapPixelToCoords(sf::Mouse::getPosition(appWindow));
		while (appWindow.pollEvent(state.event))
        {
            if(state.event.type==sf::Event::Closed)
			{
				isMouseButtonPressed=false;
				appWindow.close();
				setState(0);
				upButton.setActive(true);
				downButton.setActive(true);
				return std::string();
			}
            else if(state.event.type==sf::Event::MouseButtonReleased)
                isMouseButtonPressed=true;
        }
        for(std::size_t i=0u;i<texts.size();i++)
		{
            if(inBounds(tmpMousePos,texts[i].getGlobalBounds()))
			{
				texts[i].setOutlineColor(sf::Color::Yellow);
				if(isMouseButtonPressed && sf::Mouse::isButtonPressed(sf::Mouse::Left))
				{
				    setState(0);
					upButton.setActive(true);
					downButton.setActive(true);
					return path+texts[i].getString().toAnsiString()+ext;
				}
			}
			else
			{
				texts[i].setOutlineColor(sf::Color::Transparent);
			}
		}
        if(isMouseButtonPressed && escapeButton.wasPressed(state.event, appWindow))
		{
			isMouseButtonPressed=false;
			upButton.setActive(true);
			downButton.setActive(true);
			return std::string();
		}
        if(isMouseButtonPressed && upButton.wasPressed(state.event, appWindow))
		{
			isMouseButtonPressed=false;
			if(savesPage>0)
			{
				savesPage--;
				for(int i=0;i<14;i++)
					if(i+savesPage*14<(int)names.size())
					{
						texts[i].setString(names[i+savesPage*14].first);
						sf::FloatRect fr=texts[i].getLocalBounds();
						texts[i].setOrigin(fr.left + fr.width/2.0f, .0f);
						texts[i].setPosition(my_vector2f(appWindow.getSize().x*.4f,i*scale*60.f));
						if(this->state==2)
						{
                            dateTexts[i].setString(info[i+savesPage*14]);
                            fr=dateTexts[i].getLocalBounds();
                            dateTexts[i].setOrigin(fr.left + fr.width/2.0f, .0f);
                            dateTexts[i].setPosition(my_vector2f(appWindow.getSize().x*.6f,i*scale*60.f));
                        }
					}
					else
					{
						texts[i].setString("");
						if(this->state==2)
                            dateTexts[i].setString("");
					}
			}
		}
        if(isMouseButtonPressed && downButton.wasPressed(state.event, appWindow))
		{
			isMouseButtonPressed=false;
			if(savesPage<std::ceil(names.size()/14.f)-1)
			{
				savesPage++;
				for(int i=0;i<14;i++)
					if(i+savesPage*14<(int)names.size())
					{
						texts[i].setString(names[i+savesPage*14].first);
						sf::FloatRect fr=texts[i].getLocalBounds();
						texts[i].setOrigin(fr.left + fr.width/2.0f, .0f);
						texts[i].setPosition(my_vector2f(appWindow.getSize().x*.4f,i*scale*60.f));
						if(this->state==2)
						{
                            dateTexts[i].setString(info[i+savesPage*14]);
                            fr=dateTexts[i].getLocalBounds();
                            dateTexts[i].setOrigin(fr.left + fr.width/2.0f, .0f);
                            dateTexts[i].setPosition(my_vector2f(appWindow.getSize().x*.6f,i*scale*60.f));
                        }
					}
					else
					{
						texts[i].setString("");
						if(this->state==2)
                            dateTexts[i].setString("");
					}
			}
		}
		appWindow.clear();
		appWindow.draw(ls);
		appWindow.draw(*this);
		for(std::size_t i=0u;i<texts.size();i++)
			appWindow.draw(texts[i]);
        if(this->state==2)
		{
            for(std::size_t i=0u;i<dateTexts.size();i++)
                appWindow.draw(dateTexts[i]);
        }
		if(savesPage>0)
			appWindow.draw(upButton);
		if(savesPage<std::ceil(names.size()/14.f)-1)
			appWindow.draw(downButton);
		appWindow.display();
	}
}
sf::VideoMode MainMenu::chooseResolution(State& state, const std::vector<sf::VideoMode>& validModes)
{
    std::vector<sf::Text> texts;
	texts.clear();
	int savesPage=0;
	float scale=appWindow.getSize().y/1080.f;
	for(int i=0;i<std::min(14,(int)validModes.size());i++)
	{
		texts.push_back(sf::Text(VideoModeToString(validModes[i]),data.getFont(),60));
		texts[i].setFillColor(sf::Color::Black);
		texts[i].setOutlineThickness(6.f);
		texts[i].setScale(scale,scale);
		texts[i].setOutlineColor(sf::Color::Transparent);
		sf::FloatRect fr=texts[i].getLocalBounds();
		texts[i].setOrigin(fr.left + fr.width/2.0f, .0f);
		texts[i].setPosition(my_vector2f(appWindow.getSize().x*.5f,i*scale*60.f));
	}
    upButton.setActive(true);
    downButton.setActive(true);
	bool isMouseButtonPressed=false;
	my_vector2f tmpMousePos;
	while(true)
	{
		tmpMousePos=appWindow.mapPixelToCoords(sf::Mouse::getPosition(appWindow));
		while (appWindow.pollEvent(state.event))
        {
            if(state.event.type==sf::Event::Closed)
			{
				isMouseButtonPressed=false;
				appWindow.close();
				setState(0);
				upButton.setActive(true);
				downButton.setActive(true);
				return sf::VideoMode();
			}
            else if(state.event.type==sf::Event::MouseButtonReleased)
                isMouseButtonPressed=true;
        }
        for(std::size_t i=0u;i<texts.size();i++)
		{
            if(inBounds(tmpMousePos,texts[i].getGlobalBounds()))
			{
				texts[i].setOutlineColor(sf::Color::Yellow);
				if(isMouseButtonPressed && sf::Mouse::isButtonPressed(sf::Mouse::Left))
				{
				    setState(0);
					upButton.setActive(true);
					downButton.setActive(true);
					return validModes[i+savesPage*14];
				}
			}
			else
			{
				texts[i].setOutlineColor(sf::Color::Transparent);
			}
		}
        if(isMouseButtonPressed && escapeButton.wasPressed(state.event, appWindow))
		{
			isMouseButtonPressed=false;
			upButton.setActive(true);
			downButton.setActive(true);
			return sf::VideoMode();
		}
        if(isMouseButtonPressed && upButton.wasPressed(state.event, appWindow))
		{
			isMouseButtonPressed=false;
			if(savesPage>0)
			{
				savesPage--;
				for(int i=0;i<14;i++)
					if(i+savesPage*14<(int)validModes.size())
					{
						texts[i].setString(VideoModeToString(validModes[i+savesPage*14]));
						sf::FloatRect fr=texts[i].getLocalBounds();
						texts[i].setOrigin(fr.left + fr.width/2.0f, .0f);
						texts[i].setPosition(my_vector2f(appWindow.getSize().x*.5f,i*scale*60.f));
					}
					else
					{
						texts[i].setString("");
					}
			}
		}
        if(isMouseButtonPressed && downButton.wasPressed(state.event, appWindow))
		{
			isMouseButtonPressed=false;
			if(savesPage<std::ceil(validModes.size()/14.f)-1)
			{
				savesPage++;
				for(int i=0;i<14;i++)
					if(i+savesPage*14<(int)validModes.size())
					{
						texts[i].setString(VideoModeToString(validModes[i+savesPage*14]));
						sf::FloatRect fr=texts[i].getLocalBounds();
						texts[i].setOrigin(fr.left + fr.width/2.0f, .0f);
						texts[i].setPosition(my_vector2f(appWindow.getSize().x*.5f,i*scale*60.f));
					}
					else
					{
						texts[i].setString("");
					}
			}
		}
		appWindow.clear();
		appWindow.draw(ls);
		appWindow.draw(*this);
		for(std::size_t i=0u;i<texts.size();i++)
			appWindow.draw(texts[i]);
        if(this->state==2)
		{
            for(std::size_t i=0u;i<dateTexts.size();i++)
                appWindow.draw(dateTexts[i]);
        }
		if(savesPage>0)
			appWindow.draw(upButton);
		if(savesPage<std::ceil(validModes.size()/14.f)-1)
			appWindow.draw(downButton);
		appWindow.display();
	}
}
void MainMenu::actualizeNames()
{
    newGameButton->setText(Lang("MAIN_MENU_NEW_GAME"));
    loadGameButton->setText(Lang("MAIN_MENU_LOAD_GAME"));
    settingsButton->setText(Lang("MAIN_MENU_SETTINGS"));
    escapeButton.text.setString(Lang("MAIN_MENU_EXIT"));
    numOfPlayers.setString(Lang("MAIN_MENU_NUMBER_OF_PLAYERS"));
    generateMapButton->setText(Lang("MAIN_MENU_GENERATE"));
    fullscreenName.setString(Lang("MAIN_MENU_FULLSCREEN"));
    saveSettingsButton.text.setString(Lang("MAIN_MENU_SETTINGS_SAVE"));
    revertSettingsButton.text.setString(Lang("MAIN_MENU_SETTINGS_REVERT"));
    graphicSettingsButton.text.setString(Lang("MAIN_MENU_SETTINGS_GRAPHICS"));
    languageSettingsButton.text.setString(Lang("MAIN_MENU_SETTINGS_LANGUAGE"));
}
void newGameFunction(MainMenu& mainMenu, short& playersNum)
{
    mainMenu.setState(1);
    playersNum=2;
    mainMenu.mapSize=128;
}

void loadGameFunction(MainMenu& mainMenu, const Data& data, State& state, my_vector2i winSize)
{
    //Main function
    mainMenu.setState(2);
    std::string saveLocation=mainMenu.chooseSave(state, "./saves/",".sav");
    mainMenu.setState(0);
    if(!saveLocation.empty())
    {
        if(mainGameLoop(data, state, mainMenu, saveLocation, 0))
            appWindow.close();
        else
            endGameLoop(winSize,mainMenu,state);
    }
}
void playerNumFunction(MainMenu& mainMenu, int i, short& playersNum)
{
    mainMenu.playerNumButton[playersNum-1]->setEnabled(true);
    playersNum=i+1;
    mainMenu.playerNumButton[i]->setEnabled(false);
}

void mapSizeFunction(MainMenu& mainMenu, int i)
{
    mainMenu.mapSizeButton[mainMenu.mapSize/32-1]->setEnabled(true);
    mainMenu.mapSize=(i+1)*32;
    mainMenu.mapSizeButton[i]->setEnabled(false);
}
void revertSettingsFunction(std::vector<std::any> params) {
    //Loading parameters
    MainMenu* mainMenu=std::any_cast<MainMenu*>(params[0]);
    State* state=std::any_cast<State*>(params[1]);
    bool wasFullscreen=std::any_cast<bool>(params[2]);
    std::string* oldLanguageName=std::any_cast<std::string*>(params[3]);
    my_vector2i* winSize=std::any_cast<my_vector2i*>(params[4]);
    my_vector2i* oldWinSize=std::any_cast<my_vector2i*>(params[5]);
    //Main function
    if(state->isFullscreen!=wasFullscreen || winSize!=oldWinSize)
    {
        appWindow.close();
        *winSize=*oldWinSize;
        state->isFullscreen=wasFullscreen;
        createWindow(appWindow,*oldWinSize,wasFullscreen,*state,mainMenu->gui,sf::ContextSettings());
        mainMenu->init(*state);
        mainMenu->actualizeLoadingScreenSize();
    }
    if(state->languageName!=*oldLanguageName)
    {
        state->languageName=*oldLanguageName;
        loadLanguage(*oldLanguageName,*mainMenu,true);
    }
    mainMenu->setState(0);
}
void settingsFunction(MainMenu& mainMenu, State& state, bool& wasFullscreen, std::string& oldLanguageName, my_vector2i& winSize, my_vector2i& oldWinSize)
{
    oldLanguageName=state.languageName;
    oldWinSize=winSize;
    wasFullscreen=state.isFullscreen;
    mainMenu.setState(3);
}
void fullscreenCheckBoxFunction(std::vector<std::any> params)
{
    //Loading parameters
    MainMenu* mainMenu=std::any_cast<MainMenu*>(params[0]);
    State* state=std::any_cast<State*>(params[1]);
    std::string* oldLanguageName=std::any_cast<std::string*>(params[2]);
    //Main function
    sf::ContextSettings conSettings = appWindow.getSettings();
    my_vector2u oS = appWindow.getSize();
    appWindow.close();
    if(!mainMenu->fullscreenCheckBox.getChecked())
    {
        state->isFullscreen=true;
    }
    else
    {
        state->isFullscreen=false;
    }
    createWindow(appWindow,(my_vector2i)oS,state->isFullscreen,*state, mainMenu->gui, conSettings);
    if(state->languageName!=*oldLanguageName)
    {
        state->languageName=*oldLanguageName;
        loadLanguage(state->languageName,*mainMenu,true);
    }
    mainMenu->init(*state);
    mainMenu->setState(3);
}
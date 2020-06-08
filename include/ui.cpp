#include "ui.hpp"
#include "unit.hpp"
#include "building.hpp"
#include "player.hpp"
#include "map.hpp"
#include "city.hpp"
#include "basic/megaTexture.hpp"
#include "Data.hpp"
#include <typeinfo>
#include <limits>
#include <string>
#include <iostream>

bool UI::init(sf::View view)
{
    isPaused=false;
    isOfficer=false;
    if(loadUI()) return EXIT_FAILURE;
    nameText.setFont(data->getFont());
	nameText.setFillColor(sf::Color(0,0,127));
    nameText.setOutlineThickness(1.5f);
	nameText.setCharacterSize(30.f);
	for(int i=0;i<MAX_NUMBER_OF_PARAMETERS;i++)
	{
	    addText[i].setFont(data->getFont());
        addText[i].setFillColor(sf::Color::Black);
        addText[i].setCharacterSize(30.f);
	    addRText[i].setFont(data->getFont());
        addRText[i].setFillColor(sf::Color::Black);
        addRText[i].setCharacterSize(30.f);
	}
	buildPropos.setColor(sf::Color::Green);
    ///PAUSE BACK
    pauseBack=sf::RectangleShape(my_vector2f(1080.f*.6f,1080.f*.9f));
    pauseBack.setFillColor(sf::Color::White);
    pauseBack.setOutlineColor(sf::Color::Black);
    pauseBack.setOutlineThickness(10.f);
    escapeButton.setSize(1080.f*.4f,1080.f*.1f);
    escapeButton.unactiveColor=sf::Color::Red;
    escapeButton.activeColor=sf::Color(255,127,127);
    escapeButton.setOutlineColor(sf::Color::Black);
    escapeButton.setOutlineThickness(5.f);
    escapeButton.setText(Lang("GAME_UI_ESCAPE_TO_MENU"),data->getFont());
    saveGameButton.setSize(1080.f*.4f,1080.f*.1f);
    saveGameButton.unactiveColor=sf::Color(196,196,196);
    saveGameButton.activeColor=sf::Color::White;
    saveGameButton.setTexture(bfUI::uiBackground);
    saveGameButton.setOutlineColor(sf::Color::Black);
    saveGameButton.setOutlineThickness(5.f);
    saveGameButton.setText(Lang("GAME_UI_SAVE_GAME"),data->getFont());
    restartGameButton.setSize(1080.f*.4f,1080.f*.1f);
    restartGameButton.unactiveColor=sf::Color(196,196,196);
    restartGameButton.activeColor=sf::Color::White;
    restartGameButton.setTexture(bfUI::uiBackground);
    restartGameButton.setOutlineColor(sf::Color::Black);
    restartGameButton.setOutlineThickness(5.f);
    restartGameButton.setText(Lang("GAME_UI_RESTART_GAME"),data->getFont());
    returnToGameButton.setSize(1080.f*.4f,1080.f*.1f);
    returnToGameButton.unactiveColor=sf::Color(196,196,196);
    returnToGameButton.activeColor=sf::Color::White;
    returnToGameButton.setTexture(bfUI::uiBackground);
    returnToGameButton.setOutlineColor(sf::Color::Black);
    returnToGameButton.setOutlineThickness(5.f);
    returnToGameButton.setText(Lang("GAME_UI_RETURN_TO_GAME"),data->getFont());
    ///MAKE SURE BUTTONS
    makeSureBack=sf::RectangleShape(my_vector2f(1080.f*.6f,1080.f*.3f));
    makeSureBack.setFillColor(sf::Color::White);
    makeSureBack.setOutlineColor(sf::Color::Black);
    makeSureBack.setOutlineThickness(10.f);
    makeSureText.setFillColor(sf::Color::Black);
    makeSureText.setFont(data->getFont());
    makeSureText.setCharacterSize(60.f);
    makeSureYesButton.setSize(1080.f*.2f,1080.f*.1f);
    makeSureYesButton.unactiveColor=sf::Color::Green;
    makeSureYesButton.activeColor=sf::Color(127,255,127);
    makeSureYesButton.setOutlineColor(sf::Color::Black);
    makeSureYesButton.setOutlineThickness(5.f);
    makeSureYesButton.setText(Lang("GAME_UI_MAKE_SURE_YES"),data->getFont());
    makeSureNoButton.setSize(1080.f*.2f,1080.f*.1f);
    makeSureNoButton.unactiveColor=sf::Color::Red;
    makeSureNoButton.activeColor=sf::Color(255,127,127);
    makeSureNoButton.setOutlineColor(sf::Color::Black);
    makeSureNoButton.setOutlineThickness(5.f);
    makeSureNoButton.setText(Lang("GAME_UI_MAKE_SURE_NO"),data->getFont());
    ///INSERT TEXT
    insertText.setFont(data->getFont());
    insertText.setFillColor(sf::Color::Black);
    insertText.setCharacterSize(40);
    ///RECRUIT MENU BUTTONS
    recruitOkButton.setSize(1080.f*.2f,1080.f*.1f);
    recruitOkButton.unactiveColor=sf::Color::Green;
    recruitOkButton.activeColor=sf::Color(127,255,127);
    recruitOkButton.setOutlineColor(sf::Color::Black);
    recruitOkButton.setOutlineThickness(5.f);
    recruitOkButton.setText(Lang("GAME_UI_RECRUIT_ACCEPT"),data->getFont());
    recruitReturnButton.setSize(1080.f*.2f,1080.f*.1f);
    recruitReturnButton.unactiveColor=sf::Color::Red;
    recruitReturnButton.activeColor=sf::Color(255,127,127);
    recruitReturnButton.setOutlineColor(sf::Color::Black);
    recruitReturnButton.setOutlineThickness(5.f);
    recruitReturnButton.setText(Lang("GAME_UI_RECRUIT_DECLINE"),data->getFont());
    recruitSlider.initTextures();
    recruitNrText.setFont(data->getFont());
    recruitNrText.setFillColor(sf::Color(64,0,0));
    recruitNrText.setCharacterSize(40);
    ///COUNT TEXT
	countText.setFont(data->getFont());
	countText.setFillColor(sf::Color(127,0,0));
	countText.setCharacterSize(45.f);
    miniature.scale(128.f/256.f,128.f/256.f);//it's now 256x256 texture
    ///ACTUAL PLAYER TEXT
    actualPlayerText.setFont(data->getFont());
    actualPlayerText.setCharacterSize(actualPlayerText.getCharacterSize()*2.f);
	actualPlayerText.setFillColor(sf::Color::Red);
	actualPlayerText.setCharacterSize(60.f);
    actualPlayerText.setOutlineColor(sf::Color::Black);
    actualPlayerText.setOutlineThickness(5.f);
    menuBack=sf::RectangleShape(my_vector2f(1080.f*1.2f,1080.f*.9f));
    menuBack.setFillColor(sf::Color::White);
    menuBack.setOutlineColor(sf::Color::Black);
    menuBack.setOutlineThickness(10.f);
    ///UNIT/IMPROVEMENT INFO SWITCHING BUTTONS
    unitButton.setSize(128.f,128.f);
    unitButton.unactiveColor=sf::Color(127,127,127);
    unitButton.activeColor=sf::Color::White;
    unitButton.setOutlineColor(sf::Color::Red);
    unitButton.setOutlineThickness(2.f);
    unitButton.setUnactiveColor();
    buildButton.setSize(128.f,128.f);
    buildButton.unactiveColor=sf::Color(127,127,127);
    buildButton.activeColor=sf::Color::White;
    buildButton.setOutlineColor(sf::Color::Red);
    buildButton.setOutlineThickness(2.f);
    buildButton.setUnactiveColor();
    upgradeButton.setSize(64.f,64.f);
    upgradeButton.unactiveColor=sf::Color(127,127,127);
    upgradeButton.activeColor=sf::Color::White;
    upgradeButton.setOutlineColor(sf::Color::Red);
    upgradeButton.setOutlineThickness(1.f);
    upgradeButton.setUnactiveColor();
    data->getUiMegaTexture().assignTexture(upgradeButton,improveStar);
    /*recruitButton.icon=sf::RectangleShape(my_vector2f(64.f,64.f));
    recruitButton.unactiveColor=sf::Color(127,127,127);
    recruitButton.activeColor=sf::Color::White;
    recruitButton.icon.setOutlineColor(sf::Color::Red);
    recruitButton.icon.setOutlineThickness(1.f);
    recruitButton.icon.setFillColor(upgradeButton.unactiveColor);
    uiMega.assignTexture(recruitButton.icon,recruitID);*/
    destroyButton.setSize(64.f,64.f);
    destroyButton.unactiveColor=sf::Color(127,127,127);
    destroyButton.activeColor=sf::Color::White;
    destroyButton.setOutlineColor(sf::Color::Red);
    destroyButton.setOutlineThickness(1.f);
    destroyButton.setUnactiveColor();
    data->getUiMegaTexture().assignTexture(destroyButton,destroyID);
    openImprovMenuButton.setSize(64.f,64.f);
    openImprovMenuButton.unactiveColor=sf::Color(127,127,127);
    openImprovMenuButton.activeColor=sf::Color::White;
    openImprovMenuButton.setOutlineColor(sf::Color::Red);
    openImprovMenuButton.setOutlineThickness(1.f);
    openImprovMenuButton.setUnactiveColor();
    openImprovMenuButton.setText(Lang("GAME_UI_OPEN_BUILD_BUILDING_MENU"),data->getFont());
	openImprovMenuButton.setCharacterSize(40.f);
	///IMPROVEMENT MENU
	for(int i=0;i<std::min(50,(int)data->getBuildingTypesSize());i++)
    {
        improvementChooseButton[i].setSize(128.f,128.f);
        try{
        data->getBuildingType(i)->itMega->assignTexture(improvementChooseButton[i],data->getBuildingType(i)->iconIndFirst);
        }
        catch(...)
        {
            logStr+="Improvment types setting broke on "+my_to_string(i)+"\n";return EXIT_FAILURE;
        }
        //improvementChooseButton[i].icon.setTexture(&buildingTypes[i]->icon[0]);
        improvementChooseButton[i].unactiveColor=sf::Color(127,127,127);
        improvementChooseButton[i].activeColor=sf::Color::White;
        openImprovMenuButton.setOutlineColor(sf::Color::Red);
        openImprovMenuButton.setOutlineThickness(3.f);
    }
    improvementOkButton.setSize(1080.f*.2f,1080.f*.1f);
    improvementOkButton.unactiveColor=sf::Color::Green;
    improvementOkButton.activeColor=sf::Color(127,255,127);
    improvementOkButton.setOutlineColor(sf::Color::Black);
    improvementOkButton.setOutlineThickness(5.f);
    improvementOkButton.setText(Lang("GAME_UI_OK"),data->getFont());
    improvementOkButton.setCharacterSize(40.f);
    improvementReturnButton.setSize(1080.f*.2f,1080.f*.1f);
    improvementReturnButton.unactiveColor=sf::Color::Red;
    improvementReturnButton.activeColor=sf::Color(255,127,127);
    improvementReturnButton.setOutlineColor(sf::Color::Black);
    improvementReturnButton.setOutlineThickness(5.f);
    improvementReturnButton.setText(Lang("GAME_UI_RETURN"),data->getFont());
    improvementReturnButton.setCharacterSize(40.f);
    ///BUILD BUTTONS
    buildOk.setSize(128.f,128.f);
    buildOk.unactiveColor=sf::Color(0,127,0);
    buildOk.activeColor=sf::Color::Green;
    buildOk.setOutlineColor(sf::Color::Black);
    buildOk.setOutlineThickness(2.f);
    buildOk.setUnactiveColor();
    buildOk.setText(Lang("GAME_UI_OK"),data->getFont());
    buildOk.setCharacterSize(40.f);
    buildRet.setSize(128.f,128.f);
    buildRet.unactiveColor=sf::Color(127,0,0);
    buildRet.activeColor=sf::Color::Red;
    buildRet.setOutlineColor(sf::Color::Black);
    buildRet.setOutlineThickness(2.f);
    buildRet.setUnactiveColor();
    buildRet.setText(Lang("GAME_UI_RETURN"),data->getFont());
	///PLAYER RESOURCE INFO
	for(int i=0;i<RESOURCE_TYPES_NUMBER;i++)
	{
        data->getUiMegaTexture().assignTexture(resourceSprite[i],i+gold);
	    //resourceSprite[i].setTexture(gameUI.uiMega[i+gold]);
        resourceText[i].setFont(data->getFont());
        resourceText[i].setFillColor(sf::Color::Black);
        resourceText[i].setCharacterSize(30.f);
	}
    pauseBack=sf::RectangleShape(my_vector2f(1080.f*.6f,1080.f*.9f));
    pauseBack.setFillColor(sf::Color::White);
    pauseBack.setOutlineColor(sf::Color::Black);
    pauseBack.setOutlineThickness(10.f);
    pauseBack.setFillColor(sf::Color::White);
    pauseBack.setOutlineColor(sf::Color::Black);
    pauseBack.setOutlineThickness(10.f);
    ///BUILDING POPULATION BUTTONS
    workUp.setSize(64.f,64.f);
    //uiMega.assignTexture(workUp,clickedUpID,clickedUpID+1);
    workUp.activeColor=sf::Color::Green;
    workUp.unactiveColor=sf::Color(0,192,0);
    workUp.setActive(false);
    workDown.setSize(64.f,64.f);
    //uiMega.assignTexture(workDown,clickedDownID,clickedDownID+1);
    workDown.activeColor=sf::Color::Red;
    workDown.unactiveColor=sf::Color(192,0,0);
    workDown.setActive(false);
    uiReseting(view);
    return EXIT_SUCCESS;
}
void UI::uiReseting(sf::View myView)
{
	float uiScale = myView.getSize().y/1080.f;
	//140 0 - 134 6 - 140 70
	//mpText.setCharacterSize(40.f*uiScale);
	mapa.miniMapSprite.setPosition(myView.getCenter()-myView.getSize()*.5f);
	mapa.miniMapSprite.setScale(uiScale*.7f*384.f/mapa.getSize().x,uiScale*.7f*384.f/mapa.getSize().y);
	///PAUSE MENU
	pauseBack.setPosition(myView.getCenter()-pauseBack.getSize()*(uiScale/2.f));
	pauseBack.setScale(uiScale,uiScale);
	escapeButton.setPosition(myView.getCenter().x-escapeButton.getSize().x*(uiScale/2.f),myView.getCenter().y+myView.getSize().y*.05f);
	escapeButton.setScale(uiScale,uiScale);
	escapeButton.setCharacterSize(60.f*uiScale);
	escapeButton.updateTextTransform();
	saveGameButton.setPosition(myView.getCenter().x-escapeButton.getSize().x*(uiScale/2.f),myView.getCenter().y-myView.getSize().y*.25f);
	saveGameButton.setScale(uiScale,uiScale);
	saveGameButton.setCharacterSize(60.f*uiScale);
	saveGameButton.updateTextTransform();
	restartGameButton.setPosition(myView.getCenter().x-escapeButton.getSize().x*(uiScale/2.f),myView.getCenter().y-myView.getSize().y*.1f);
	restartGameButton.setScale(uiScale,uiScale);
	restartGameButton.setCharacterSize(60.f*uiScale);
	restartGameButton.updateTextTransform();
	returnToGameButton.setPosition(myView.getCenter().x-returnToGameButton.getSize().x*(uiScale/2.f),myView.getCenter().y-myView.getSize().y*.4f);
	returnToGameButton.setScale(uiScale,uiScale);
	returnToGameButton.setCharacterSize(60.f*uiScale);
	returnToGameButton.updateTextTransform();
	///MAKE SURE BUTTONS
    makeSureBack.setPosition(myView.getCenter()-makeSureBack.getSize()*(uiScale/2.f));
	makeSureBack.setScale(uiScale,uiScale);
	makeSureText.setPosition(makeSureBack.getPosition());
	makeSureText.setScale(uiScale,uiScale);
	makeSureYesButton.setPosition(myView.getCenter().x-myView.getSize().x*.15f,myView.getCenter().y);
	makeSureYesButton.setScale(uiScale,uiScale);
	makeSureYesButton.setCharacterSize(60.f*uiScale);
	makeSureYesButton.updateTextTransform();
	makeSureNoButton.setPosition(myView.getCenter().x+myView.getSize().x*.05f,myView.getCenter().y);
	makeSureNoButton.setScale(uiScale,uiScale);
	makeSureNoButton.setCharacterSize(60.f*uiScale);
	makeSureNoButton.updateTextTransform();
	///INSERT TEXT
    insertText.setPosition((my_vector2f)makeSureBack.getPosition()+my_vector2f(0.f,80.f)*uiScale);
    insertText.setScale(uiScale,uiScale);
	///RECRUIT MENU BUTTONS
	recruitOkButton.setPosition(myView.getCenter().x-myView.getSize().x*.15f,myView.getCenter().y);
	recruitOkButton.setScale(uiScale,uiScale);
	recruitOkButton.setCharacterSize(60.f*uiScale);
	recruitOkButton.updateTextTransform();
	recruitReturnButton.setPosition(myView.getCenter().x+myView.getSize().x*.05f,myView.getCenter().y);
	recruitReturnButton.setScale(uiScale,uiScale);
	recruitReturnButton.setCharacterSize(60.f*uiScale);
	recruitReturnButton.updateTextTransform();
	recruitSlider.setPosition(-240.f*uiScale+myView.getCenter().x,myView.getCenter().y-160.f*uiScale);
	recruitSlider.setScale(.35f*uiScale,uiScale);
	//recruitSlider.slider.setScale(uiScale,uiScale);
	recruitNrText.setPosition(myView.getCenter()-makeSureBack.getSize()*(uiScale/2.f));
	recruitNrText.setScale(uiScale,uiScale);
	///UNIT/IMPROVEMENT INFO SWITCHING BUTTONS
    unitButton.setPosition(myView.getSize().x/2.f-256.f*uiScale+myView.getCenter().x,740.f*uiScale-myView.getSize().y/2.f+myView.getCenter().y);
	unitButton.setScale(uiScale,uiScale);
    buildButton.setPosition(myView.getSize().x/2.f-128.f*uiScale+myView.getCenter().x,740.f*uiScale-myView.getSize().y/2.f+myView.getCenter().y);
	buildButton.setScale(uiScale,uiScale);
    //recruitButton.setPosition(myView.getSize().x/2.f-128.f*uiScale+myView.getCenter().x,675.f*uiScale-myView.getSize().y/2.f+myView.getCenter().y);
	//recruitButton.icon.setScale(uiScale,uiScale);
    upgradeButton.setPosition(myView.getSize().x/2.f-192.f*uiScale+myView.getCenter().x,675.f*uiScale-myView.getSize().y/2.f+myView.getCenter().y);
	upgradeButton.setScale(uiScale,uiScale);
    destroyButton.setPosition(myView.getSize().x/2.f-256.f*uiScale+myView.getCenter().x,675.f*uiScale-myView.getSize().y/2.f+myView.getCenter().y);
	destroyButton.setScale(uiScale,uiScale);
	///PLAYER UI BUTTONS
	openImprovMenuButton.setPosition(myView.getSize().x/2.f-64.f*uiScale+myView.getCenter().x,675.f*uiScale-myView.getSize().y/2.f+myView.getCenter().y);
	openImprovMenuButton.setScale(uiScale,uiScale);
	openImprovMenuButton.updateTextTransform();
	openImprovMenuButton.text.setScale(uiScale,uiScale);
	menuBack.setPosition(myView.getCenter()-menuBack.getSize()*(uiScale/2.f));
	menuBack.setScale(uiScale,uiScale);
	///IMPROVEMENT MENU
	for(int i=0;i<std::min(50,(int)data->getBuildingTypesSize());i++)
    {
        improvementChooseButton[i].setPosition(myView.getCenter().x+(i%10-5)*uiScale*132.f,myView.getCenter().y+((i+1)/11-3.5)*135.f*uiScale);
        improvementChooseButton[i].setScale(uiScale,uiScale);
    }
    improvementOkButton.setPosition(myView.getCenter().x-myView.getSize().x*.15f,myView.getCenter().y+myView.getSize().y*0.35f);
	improvementOkButton.setScale(uiScale,uiScale);
	improvementOkButton.setCharacterSize(60.f*uiScale);
	improvementOkButton.updateTextTransform();
	improvementReturnButton.setPosition(myView.getCenter().x+myView.getSize().x*.05f,myView.getCenter().y+myView.getSize().y*0.35f);
	improvementReturnButton.setScale(uiScale,uiScale);
	improvementReturnButton.setCharacterSize(60.f*uiScale);
	improvementReturnButton.updateTextTransform();
	///BUILD BUTTONS
	buildOk.setPosition(myView.getSize().x/2.f-256.f*uiScale+myView.getCenter().x,740.f*uiScale-myView.getSize().y/2.f+myView.getCenter().y);
	buildOk.setScale(uiScale,uiScale);
	buildOk.text.setScale(uiScale,uiScale);
	buildOk.updateTextTransform();
    buildRet.setPosition(myView.getSize().x/2.f-128.f*uiScale+myView.getCenter().x,740.f*uiScale-myView.getSize().y/2.f+myView.getCenter().y);
	buildRet.setScale(uiScale,uiScale);
	buildRet.text.setScale(uiScale,uiScale);
	buildRet.updateTextTransform();
	///GAME UI
	nameText.setPosition(myView.getSize().x/2.f-256.f*uiScale+myView.getCenter().x,256.f*uiScale-myView.getSize().y/2.f+myView.getCenter().y);
	nameText.setScale(uiScale,uiScale);
	//otherParamText.setPosition(myView.getSize().x/2.f-230.f*uiScale+myView.getCenter().x,220.f*uiScale-myView.getSize().y/2.f+myView.getCenter().y);
	otherParamText.setScale(uiScale,uiScale);
	for(int i=0;i<MAX_NUMBER_OF_PARAMETERS;i++)
	{
	    addSprite[i].setPosition(myView.getSize().x/2.f-256.f*uiScale+myView.getCenter().x,(290.f+i*30.f)*uiScale-myView.getSize().y/2.f+myView.getCenter().y);
        addText[i].setPosition(myView.getSize().x/2.f-230.f*uiScale+myView.getCenter().x,(290.f+i*30.f)*uiScale-myView.getSize().y/2.f+myView.getCenter().y);
        addSprite[i].setScale(uiScale*.5f,uiScale*.5f);
        addText[i].setScale(uiScale,uiScale);
        addRSprite[i].setPosition(myView.getSize().x/2.f-128.f*uiScale+myView.getCenter().x,(290.f+i*30.f)*uiScale-myView.getSize().y/2.f+myView.getCenter().y);
        addRText[i].setPosition(myView.getSize().x/2.f-100.f*uiScale+myView.getCenter().x,(290.f+i*30.f)*uiScale-myView.getSize().y/2.f+myView.getCenter().y);
        addRSprite[i].setScale(uiScale*.5f,uiScale*.5f);
        addRText[i].setScale(uiScale,uiScale);
	}
	///BUILDING POPULATION BUTTONS
        workDown.setPosition((my_vector2f)addText[3].getPosition()+my_vector2f(64.f*uiScale,0.f));
        workDown.setScale(uiScale*.5f,uiScale*.5f);
        workUp.setPosition((my_vector2f)addText[3].getPosition()+my_vector2f(96.f*uiScale,0.f));
        workUp.setScale(uiScale*.5f,uiScale*.5f);
	///end
	countText.setPosition(myView.getSize().x/2.f-50.f*uiScale+myView.getCenter().x,200.f*uiScale-myView.getSize().y/2.f+myView.getCenter().y);
	countText.setScale(uiScale,uiScale);
    miniature.setPosition(myView.getSize().x/2.f-256.f*uiScale+myView.getCenter().x,-myView.getSize().y/2.f+myView.getCenter().y);
    miniature.setScale(uiScale*2.f,uiScale*2.f);
    uiBack.setPosition(myView.getSize().x/2.f-256.f*uiScale+myView.getCenter().x,myView.getCenter().y-myView.getSize().y/2.f);
    uiBack.setScale(uiScale,uiScale);
    actualPlayerText.setPosition(300.f*uiScale-myView.getSize().x/2.f+myView.getCenter().x,myView.getCenter().y-myView.getSize().y/2.f+50.f*uiScale);
    actualPlayerText.setScale(uiScale,uiScale);
    //if(tileSize>0)
        //grid.setPosition((int)(myView.getCenter().x)/tileSize*tileSize-(int)(myView.getSize().x/2.f)/tileSize*tileSize-tileSize,(int)(myView.getCenter().y)/tileSize*tileSize-(int)(myView.getSize().y/2.f+.1f)/tileSize*tileSize-tileSize);
	for(int i=0;i<RESOURCE_TYPES_NUMBER;i++)
    {
        resourceSprite[i].setScale(uiScale*.5f,uiScale*.5f);
        resourceSprite[i].setPosition(myView.getSize().x/2.f-256.f*uiScale+myView.getCenter().x,(1080.f-(RESOURCE_TYPES_NUMBER-i)*30.f)*uiScale-myView.getSize().y/2.f+myView.getCenter().y);
        resourceText[i].setScale(uiScale,uiScale);
        resourceText[i].setPosition(myView.getSize().x/2.f-230.f*uiScale+myView.getCenter().x,(1080.f-(RESOURCE_TYPES_NUMBER-i)*30.f)*uiScale-myView.getSize().y/2.f+myView.getCenter().y);
    }
}
void UI::unitInfo(const State& state) //-1 - no
{
	if(state.pressedUnitIndex==-1)
	{
	    isOfficer=false;
	    nameText.setString("");
		countText.setString("");
		miniature.setTexture(data->getNullTexture());
		for(int i=0;i<MAX_NUMBER_OF_PARAMETERS;i++)
		{
		    addText[i].setString("");
		    addSprite[i].setTexture(data->getNullTexture());
		}
		return;
	}
	nameText.setString(units[state.pressedUnitIndex].type->name);
	nameText.setFillColor(players[units[state.pressedUnitIndex].owner].playerColor);
    nameText.setOutlineColor(sf::Color::Black);
	countText.setString("");
	addText[0].setString(my_to_string(units[state.pressedUnitIndex].mp)+"/"+my_to_string(units[state.pressedUnitIndex].type->maxMP));
	data->getUiMegaTexture().assignTexture(addSprite[0],mpID);
	//addSprite[0].setTexture(uiMega[mpID], true);
	if(typeid(*(units[state.pressedUnitIndex].type.get()))==typeid(officer))
	{
	    officer* tmpOff=dynamic_cast<officer*>(units[state.pressedUnitIndex].type.get());
        isOfficer=true;
        for(int i=1;i<=std::min(5,(int)units[state.pressedUnitIndex].commandedUnits.size());i++)
        {
            addText[2*i-1].setString("\t"+my_to_string(units[state.pressedUnitIndex].commandedUnits[i-1].second));
            //unitTypes[(units[state.pressedUnitIndex].commandedUnits[i-1]).first]->mega->assignTexture(addSprite[2*i-1], unitTypes[(units[state.pressedUnitIndex].commandedUnits[i-1]).first]->unitTextureInd);
            data->getUnitType(units[state.pressedUnitIndex].commandedUnits[i-1].first)->textureUP.get()->assignTexture(addSprite[2*i-1], 0);
            //addSprite[2*i-1].setTexture(unitTypes[(units[index].commandedUnits[i-1]).first]->getUnitTexture(),true);
        }
        for(int i=6;i<=std::min(10,(int)units[state.pressedUnitIndex].commandedUnits.size());i++)
        {
            addRText[2*i].setString("\t"+my_to_string(units[state.pressedUnitIndex].commandedUnits[i-1].second));
            //unitTypes[(units[state.pressedUnitIndex].commandedUnits[i-1]).first]->mega->assignTexture(addRSprite[2*i-11], unitTypes[(units[state.pressedUnitIndex].commandedUnits[i-1]).first]->unitTextureInd);
            data->getUnitType(units[state.pressedUnitIndex].commandedUnits[i-1].first)->textureUP.get()->assignTexture(addRSprite[2*i-11], 0);
            //addRSprite[2*i-11].setTexture(unitTypes[(units[index].commandedUnits[i-1]).first]->getUnitTexture(),true);
        }
        for(int i=tmpOff->maxSize+1;i<11;i++)
        {
            if(i<6)
                data->getMainMegaTexture().assignTexture(addSprite[2*i-1],locked);
                //addSprite[2*i-1].setTexture(data->getMainMegaTexture()[locked],true);
            else
                data->getMainMegaTexture().assignTexture(addRSprite[2*i-11],locked);
                //addRSprite[2*i-11].setTexture(data->getMainMegaTexture()[locked],true);
        }
        //uiReseting(state.view);
	}
	else
	{
	    addText[1].setString(my_to_string(units[state.pressedUnitIndex].hp)+"/"+my_to_string(units[state.pressedUnitIndex].type->maxHP));
	    data->getUiMegaTexture().assignTexture(addSprite[1],hpID);
        //addSprite[1].setTexture(uiMega[hpID], true);
        if(typeid(*(units[state.pressedUnitIndex].type.get()))==typeid(melee))
        {
            addText[2].setString(my_to_string(units[state.pressedUnitIndex].type->str));
            data->getUiMegaTexture().assignTexture(addSprite[2],str);
            //addSprite[2].setTexture(gameUI.uiMega[str], true);
        }
        else if(typeid(*(units[state.pressedUnitIndex].type.get()))==typeid(ranged))
        {
            addText[2].setString(my_to_string(units[state.pressedUnitIndex].type->str));
            data->getUiMegaTexture().assignTexture(addSprite[2],str);
            ranged* tmp57045=dynamic_cast<ranged*>((units[state.pressedUnitIndex].type.get()));
            addText[3].setString(my_to_string(tmp57045->rangedStr));
            data->getUiMegaTexture().assignTexture(addSprite[3],ranStr);
            addText[4].setString(my_to_string(tmp57045->range));
            data->getUiMegaTexture().assignTexture(addSprite[4],rang);
            addText[5].setString(my_to_string(units[state.pressedUnitIndex].numOfArrows)+"/"+my_to_string(tmp57045->capacity));
            data->getUiMegaTexture().assignTexture(addSprite[5],capac);
        }
	}
    units[state.pressedUnitIndex].type->textureUP.get()->assignTexture(miniature,0);
	//miniature.setTexture(units[index].type->getUnitTexture());
}
void UI::cityInfo(int index)
{
    if(index==-1)
	{
		countText.setString("");
		nameText.setString("");
		miniature.setTexture(data->getNullTexture());
		for(int i=0;i<MAX_NUMBER_OF_PARAMETERS;i++)
		{
		    addSprite[i].setTexture(data->getNullTexture());
		    addText[i].setString("");
		    addText[i].setFillColor(sf::Color::Black);
		}
		return;
	}
    countText.setString(my_to_string((short)cities[index].getStage()));
	nameText.setString(cities[index].getName());
	data->getMainMegaTexture().assignTexture(miniature,cityTex+cities[index].getStage());
	data->getUiMegaTexture().assignTexture(addSprite[0],house);
	addText[0].setString(my_to_string(cities[index].getPopulation())+std::string("/")+my_to_string(cities[index].getMaxPop()));
	addText[1].setString(my_to_string(cities[index].freePop));
}
void UI::improInfo(const State& state) //-1 - no
{
	if(state.pressedImproIndex==-1)
	{
        workUp.setActive(false);
        workDown.setActive(false);
		countText.setString("");
		nameText.setString("");
		miniature.setTexture(data->getNullTexture());
		for(int i=0;i<MAX_NUMBER_OF_PARAMETERS;i++)
		{
		    addSprite[i].setTexture(data->getNullTexture());
		    addText[i].setString("");
		    addText[i].setFillColor(sf::Color::Black);
		}
		return;
	}
	else if(state.pressedImproIndex==-2) //special index for showing type information
    {
        workUp.setActive(false);
        workDown.setActive(false);
        nameText.setString(data->getBuildingType(pressedType)->name);
        nameText.setFillColor(players[state.actualPlayer].playerColor);
        nameText.setOutlineColor(sf::Color::Black);
        if(typeid(*data->getBuildingType(pressedType))==typeid(ResourceBuilding))
        {
            ResourceBuilding* tmpImpr=dynamic_cast<ResourceBuilding*>(data->getBuildingType(pressedType).get());
            addText[0].setString(my_to_string(tmpImpr->givenResourceNumber[0])+std::string("")+Lang("GAME_UI_PER_WORKER"));
            data->getUiMegaTexture().assignTexture(addSprite[0],tmpImpr->givenResourceID);
            //addSprite[0].setTexture(uiMega[tmpImpr->givenResourceID],true);
        }
        if(typeid(*data->getBuildingType(pressedType))==typeid(Farm))
        {
            Farm* tmpImpr=dynamic_cast<Farm*>(data->getBuildingType(pressedType).get());
            addText[0].setString(my_to_string(tmpImpr->givenFood[0])+std::string("")+Lang("GAME_UI_PER_WORKER"));
            data->getUiMegaTexture().assignTexture(addSprite[0],food);
            //addSprite[0].setTexture(uiMega[tmpImpr->givenResourceID],true);
        }
        else if(typeid(*data->getBuildingType(pressedType))==typeid(RecruitBuilding))
        {
            RecruitBuilding* tmpImpr=dynamic_cast<RecruitBuilding*>(data->getBuildingType(pressedType).get());
            addText[0].setString(tmpImpr->desc1);
            addText[1].setString(tmpImpr->desc2);
        }
        else if(typeid(*data->getBuildingType(pressedType))==typeid(HouseBuilding))
        {
            HouseBuilding* tmpImpr=dynamic_cast<HouseBuilding*>(data->getBuildingType(pressedType).get());
            if(tmpImpr->heldPopulation[0]==1)
                addText[0].setString(my_to_string(tmpImpr->heldPopulation[0])+" "+Lang("GAME_UI_HOUSE_SINGULAR"));
            else
                addText[0].setString(my_to_string(tmpImpr->heldPopulation[0])+" "+Lang("GAME_UI_HOUSE_PLURAL"));
        }
        data->getBuildingType(pressedType)->itMega->assignTexture(miniature,data->getBuildingType(pressedType)->iconIndFirst);
        //miniature.setTexture(buildingTypes[pressedType]->icon[0]);
        //auto tmp618 = state.pressedImproIndex;
        //state.pressedImproIndex=-2;
        showUpgradeCost(state);
        //state.pressedImproIndex=tmp618;
        return;
    }
    workUp.setActive(true);
    workDown.setActive(true);
    data->getBuildingType(pressedType)->itMega->assignTexture(buildButton,data->getBuildingType(pressedType)->iconIndFirst+cities[state.pressedImproCityIndex].getBuilding(state.pressedImproIndex).stage);
	//buildButton.icon.setTexture(&(buildings[index].type->icon[buildings[index].stage]), true);
	countText.setString(my_to_string(cities[state.pressedImproCityIndex].getBuilding(state.pressedImproIndex).stage));
	nameText.setString(cities[state.pressedImproCityIndex].getBuilding(state.pressedImproIndex).type->name);
	nameText.setFillColor(players[cities[state.pressedImproCityIndex].owner].playerColor);
    nameText.setOutlineColor(sf::Color::Black);
    addText[3].setString(my_to_string(cities[state.pressedImproCityIndex].getBuilding(state.pressedImproIndex).employed)+std::string("/")+my_to_string(cities[state.pressedImproCityIndex].getBuilding(state.pressedImproIndex).type->maxEmployedNr[cities[state.pressedImproCityIndex].getBuilding(state.pressedImproIndex).stage]));
    data->getUiMegaTexture().assignTexture(addSprite[3],popIconId);
    if(typeid(*(cities[state.pressedImproCityIndex].getBuilding(state.pressedImproIndex).type.get()))==typeid(ResourceBuilding))
    {
        ResourceBuilding* tmpImpr=dynamic_cast<ResourceBuilding*>(cities[state.pressedImproCityIndex].getBuilding(state.pressedImproIndex).type.get());
        addText[0].setString(my_to_string(tmpImpr->givenResourceNumber[cities[state.pressedImproCityIndex].getBuilding(state.pressedImproIndex).stage])+" "+Lang("GAME_UI_PER_TURN"));
        data->getUiMegaTexture().assignTexture(addSprite[0],tmpImpr->givenResourceID);
    }
    else if(typeid(*(cities[state.pressedImproCityIndex].getBuilding(state.pressedImproIndex).type.get()))==typeid(RecruitBuilding))
    {
        RecruitBuilding* tmpImpr=dynamic_cast<RecruitBuilding*>(cities[state.pressedImproCityIndex].getBuilding(state.pressedImproIndex).type.get());
        addText[0].setString(tmpImpr->desc1);
        addText[1].setString(tmpImpr->desc2);
        addText[2].setString(my_to_string(cities[state.pressedImproCityIndex].getBuilding(state.pressedImproIndex).tmp1));
        //showing recruit cost
        int16_t tmp614[RESOURCE_TYPES_NUMBER]; int16_t tmpSize=0;
        for(int i=0;i<=RESOURCE_TYPES_NUMBER;i++)
        {
            if(cities[state.pressedImproCityIndex].getBuilding(state.pressedImproIndex).type->buildingCost[cities[state.pressedImproCityIndex].getBuilding(state.pressedImproIndex).stage][i]>0)
            {
                tmp614[tmpSize]=i; tmpSize++;
            }
        }
        data->getUiMegaTexture().assignTexture(addSprite[0],improveStar);
        //addSprite[3].setTexture(gameUI.uiMega[improveStar], true);
        if(tmpSize==0)
            addText[4].setString(Lang("GAME_UI_FREE_RECRUIT"));
        else
        {
            addText[4].setString(Lang("GAME_UI_"));
            data->getUiMegaTexture().assignTexture(addSprite[4],popIconId);
            //addSprite[0].setTexture(uiMega[popIconId],true);
            for(int j=0;j<tmpSize;j++)
            {
                addText[j+4].setString(my_to_string(tmpImpr->recruitCost[tmp614[j]]));
                if(tmpImpr->recruitCost[tmp614[j]]>players[state.actualPlayer].resourceQuantity[tmp614[j]])
                    addText[j+4].setFillColor(sf::Color::Red);
                data->getUiMegaTexture().assignTexture(addSprite[j+4],gold+tmp614[j]);
            }
        }
    }
    else if(typeid(*(cities[state.pressedImproCityIndex].getBuilding(state.pressedImproIndex).type.get()))==typeid(HouseBuilding))
    {
        HouseBuilding* tmpImpr=dynamic_cast<HouseBuilding*>(cities[state.pressedImproCityIndex].getBuilding(state.pressedImproIndex).type.get());
        if(tmpImpr->heldPopulation[0]==1)
            addText[0].setString(my_to_string(tmpImpr->heldPopulation[0])+Lang("GAME_UI_HOUSE_SINGULAR"));
        else
            addText[0].setString(my_to_string(tmpImpr->heldPopulation[0])+Lang("GAME_UI_HOUSE_PLURAL"));
    }
    if(cities[state.pressedImproCityIndex].getBuilding(state.pressedImproIndex).lastManCost>0)
    {
        addText[10].setString(Lang("GAME_UI_UNDER_CONSTRUCTION"));
        data->getUiMegaTexture().assignTexture(addSprite[11],popIconId);
        addText[11].setString(my_to_string(cities[state.pressedImproCityIndex].getBuilding(state.pressedImproIndex).lastManCost)+" "+Lang("GAME_UI_MORE_NEEDED"));
    }
    ///upgrading cost text
    //showUpgradeCost(state);
    data->getBuildingType(pressedType)->itMega->assignTexture(miniature,data->getBuildingType(pressedType)->iconIndFirst);
	//miniature.setTexture(buildings[index].type->icon[buildings[index].stage]);
}
void UI::showUpgradeCost(const State& state)
{
    if(state.pressedImproIndex==-2)
    {
        int16_t tmp614[RESOURCE_TYPES_NUMBER]; int16_t tmpSize=0;
        for(int i=0;i<=RESOURCE_TYPES_NUMBER;i++)
        {
            if(data->getBuildingType(pressedType)->buildingCost[0][i]>0)
            {
                tmp614[tmpSize]=i; tmpSize++;
            }
        }
        addText[2].setString(Lang("GAME_UI_BUILD_COST"));
        for(int j=0;j<tmpSize;j++)
        {
            addText[j+3].setString(my_to_string(data->getBuildingType(pressedType)->buildingCost[0][tmp614[j]]));
            if(data->getBuildingType(pressedType)->buildingCost[0][tmp614[j]]>players[state.actualPlayer].resourceQuantity[tmp614[j]])
                addText[j+3].setFillColor(sf::Color::Red);
            if(tmp614[j]!=RESOURCE_TYPES_NUMBER) data->getUiMegaTexture().assignTexture(addSprite[j+3],gold+tmp614[j]);
            else data->getUiMegaTexture().assignTexture(addSprite[j+3],popIconId);
            //addSprite[j+3].setTexture(uiMega[mpID], true);
        }
        return;
    }
    if(cities[state.pressedImproCityIndex].getBuilding(state.pressedImproIndex).stage<cities[state.pressedImproCityIndex].getBuilding(state.pressedImproIndex).type->maxStage-1) //can be upgraded
    {
        int16_t tmp614[RESOURCE_TYPES_NUMBER]; int16_t tmpSize=0;
        for(int i=0;i<=RESOURCE_TYPES_NUMBER;i++)
        {
            if(cities[state.pressedImproCityIndex].getBuilding(state.pressedImproIndex).type->buildingCost[cities[state.pressedImproCityIndex].getBuilding(state.pressedImproIndex).stage][i]>0)
            {
                tmp614[tmpSize]=i; tmpSize++;
            }
        }
        data->getUiMegaTexture().assignTexture(addSprite[2],improveStar);
        //addSprite[2].setTexture(gameUI.uiMega[improveStar], true);
        if(tmpSize==0)
            addText[2].setString(Lang("GAME_UI_FREE_UPGRADE"));
        else
        {
            addText[2].setString(Lang("GAME_UI_UPGRADE_COST"));
            for(int j=0;j<tmpSize;j++)
            {
                addText[j+3].setString(my_to_string(cities[state.pressedImproCityIndex].getBuilding(state.pressedImproIndex).type->buildingCost[cities[state.pressedImproCityIndex].getBuilding(state.pressedImproIndex).stage][tmp614[j]]));
                if(cities[state.pressedImproCityIndex].getBuilding(state.pressedImproIndex).type->buildingCost[cities[state.pressedImproCityIndex].getBuilding(state.pressedImproIndex).stage][tmp614[j]]>players[state.actualPlayer].resourceQuantity[tmp614[j]])
                    addText[j+3].setFillColor(sf::Color::Red);
                if(tmp614[j]!=RESOURCE_TYPES_NUMBER) data->getUiMegaTexture().assignTexture(addSprite[j+3],gold+tmp614[j]);
                else data->getUiMegaTexture().assignTexture(addSprite[j+3],popIconId);
            }
        }
    }
}
void UI::setTargetPosition(const State& state,int x, int y, int dis)
{
    targetCircle.setPosition(x,y);
    targetCircle.setOutlineColor(sf::Color::Red);
    //targetSprite.setTexture(targetTexture,true);
    if(dis>1000000)
        addText[0].setString(my_to_string(units[state.pressedUnitIndex].mp)+"/"+my_to_string(units[state.pressedUnitIndex].type->maxMP)+"(INF)"); //zmienilem bo mi nie wyswietlalo
    else
        addText[0].setString(my_to_string(units[state.pressedUnitIndex].mp)+"/"+my_to_string(units[state.pressedUnitIndex].type->maxMP)+"("+my_to_string(dis)+")");
}
void UI::setTargetPosition(const State& state, my_vector2i pos, int dis, short attType)
{
    targetCircle.setPosition((my_vector2f)pos);
    targetCircle.setOutlineColor(sf::Color::Red);
    //targetSprite.setTexture(targetTexture,true);
    addText[0].setString(my_to_string(units[state.pressedUnitIndex].mp)+"/"+my_to_string(units[state.pressedUnitIndex].type->maxMP)+" ("+my_to_string(dis)+")");
    int dealtDamage; short killedUnits;
    addText[7].setString(Lang("GAME_UI_KILLED_UNITS"));
    if(attType==AttackTypes::meleeAttack)
    {
        dealtDamage=units[state.pressedUnitIndex].kount*units[state.pressedUnitIndex].type->str;
        killedUnits=dealtDamage/units[state.targetUnitIndex].type->maxHP;
        if(dealtDamage%units[state.targetUnitIndex].type->maxHP>=units[state.targetUnitIndex].hp)
            killedUnits++;
        if(killedUnits>=units[state.targetUnitIndex].kount) {addText[8].setString(my_to_string(units[state.targetUnitIndex].kount)+"/"+ my_to_string(units[state.targetUnitIndex].kount)+" "+Lang("GAME_UI_KILLED"));return;}
        else otherParamText.setString(my_to_string(killedUnits)+"/"+ my_to_string(units[state.targetUnitIndex].kount)+" "+Lang("GAME_UI_KILLED")+", "+Lang("HP")+my_to_string(units[state.targetUnitIndex].hp-dealtDamage+killedUnits*units[state.targetUnitIndex].type->maxHP));
        int takenDamage=(units[state.targetUnitIndex].kount-killedUnits)*units[state.targetUnitIndex].type->str;
        short lostUnits=takenDamage/units[state.pressedUnitIndex].type->maxHP;
        if(dealtDamage%units[state.pressedUnitIndex].type->maxHP>=units[state.pressedUnitIndex].hp)
            lostUnits++;
        if(lostUnits>=units[state.targetUnitIndex].kount) {addText[1].setString(my_to_string(units[state.pressedUnitIndex].hp)+"/"+my_to_string(units[state.pressedUnitIndex].type->maxHP)+" (RIP)");return;}
        else addText[1].setString(my_to_string(units[state.pressedUnitIndex].hp)+"/"+my_to_string(units[state.pressedUnitIndex].type->maxHP)+"p. (-"+my_to_string(lostUnits)+";"+my_to_string(units[state.pressedUnitIndex].hp-dealtDamage+lostUnits*units[state.pressedUnitIndex].type->maxHP)+"hp)");
    }
    else if(attType==AttackTypes::rangedAttack)
    {
        dealtDamage=units[state.pressedUnitIndex].kount*dynamic_cast<ranged*>(units[state.pressedUnitIndex].type.get())->rangedStr;
        killedUnits=dealtDamage/units[state.targetUnitIndex].type->maxHP;
        if(dealtDamage%units[state.targetUnitIndex].type->maxHP>=units[state.targetUnitIndex].hp)
            killedUnits++;
        if(killedUnits>=units[state.targetUnitIndex].kount) {addText[8].setString(my_to_string(units[state.targetUnitIndex].kount)+"/"+ my_to_string(units[state.targetUnitIndex].kount)+" killed");return;}
        else addText[8].setString(my_to_string(killedUnits)+"/"+ my_to_string(units[state.targetUnitIndex].kount)+" "+Lang("GAME_UI_KILLED")+", "+Lang("HP")+my_to_string(units[state.targetUnitIndex].hp-dealtDamage+killedUnits*units[state.targetUnitIndex].type->maxHP));
    }
}
void UI::setTargetPosition(const State& state, my_vector2i pos, int dis)
{
    setTargetPosition(state, pos.x,pos.y,dis);
}
void UI::unsetTarget()
{
    targetCircle.setFillColor(sf::Color::Transparent);
    //targetSprite.setTexture(data->getNullTexture());
    otherParamText.setString("");
}
void UI::actualizeTileInfo(const State& state)
{
    if(state.pressedImproIndex<0 && state.pressedUnitIndex<0)
    {
        unsetTarget();
        pressedType=0;
        buildButton.setTexture(data->getNullTexture());
        unitButton.setTexture(data->getNullTexture());
        unsetTarget();
        improInfo(state); unitInfo(state);
        return;
    }
    else if(pressedType==0)
    {
        pressedType=1;
        if(state.pressedImproIndex>=0 && cities[state.pressedImproCityIndex].getBuilding(state.pressedImproIndex).stage==cities[state.pressedImproCityIndex].getBuilding(state.pressedImproIndex).type->maxStage)
            cities[state.pressedImproCityIndex].getBuilding(state.pressedImproIndex).type->itMega->assignTexture(buildButton,data->getBuildingType(pressedType)->iconIndFirst+cities[state.pressedImproCityIndex].getBuilding(state.pressedImproIndex).stage);
            //buildButton.icon.setTexture(&(buildings[state.pressedImproIndex].type->icon[buildings[state.pressedImproIndex].stage]), true);
        if(state.pressedUnitIndex>=0)
        {
            units[state.pressedUnitIndex].type->textureUP.get()->assignTexture(unitButton,0);
            //unitButton.icon.setTexture(&(units[state.pressedUnitIndex].type->getUnitTexture()), true);
            unitInfo(state);
        }
        else
            {improInfo(state); pressedType=2;}
    }
    else if(pressedType==1)
    {
        //auto tmp618 = state.pressedImproIndex;
        //state.pressedImproIndex=-1;
        improInfo(state);
        //state.pressedImproIndex=tmp618;
        unitInfo(state);
    }
    else if(pressedType==2)
    {
        unsetTarget();
        //auto tmp618 = state.pressedUnitIndex;
        //state.pressedUnitIndex=-1;
        unitInfo(state);
        //state.pressedImproIndex=tmp618;
        improInfo(state);
    }
}
void UI::actualizePlayer(const State& state)
{
    actualPlayerText.setString(players[state.actualPlayer].name);
    actualPlayerText.setFillColor(players[state.actualPlayer].playerColor);
    for(int i=0;i<RESOURCE_TYPES_NUMBER;i++)
        resourceText[i].setString(my_to_string(players[state.actualPlayer].resourceQuantity[i]));
}
void UI::draw(const State& state, sf::RenderTarget& target) const
{
	target.draw(uiBack);
    target.draw(nameText);
    target.draw(openImprovMenuButton);
    if(state.pressedUnitIndex+state.pressedImproIndex>-2)
    {
        target.draw(unitButton);
        target.draw(buildButton);
        if((pressedType==1 && units[state.pressedUnitIndex].owner==state.actualPlayer) || (pressedType==2 && cities[state.pressedImproCityIndex].getBuilding(state.pressedImproIndex).owner==state.actualPlayer))
            target.draw(destroyButton);
        if(pressedType==2 && cities[state.pressedImproCityIndex].getBuilding(state.pressedImproIndex).owner==state.actualPlayer)
        {
            if(cities[state.pressedImproCityIndex].getBuilding(state.pressedImproIndex).stage<cities[state.pressedImproCityIndex].getBuilding(state.pressedImproIndex).type->maxStage-1)
                target.draw(upgradeButton);
            if(cities[state.pressedImproCityIndex].freePop>0)
                target.draw(workUp);
            if(cities[state.pressedImproCityIndex].getBuilding(state.pressedImproIndex).employed>0)
                target.draw(workDown);
        }
    }
	if(state.pressedUnitIndex>=0 || state.pressedImproIndex>=0 || state.drawState==CITY_VIEW)
	{
        target.draw(otherParamText);
        for(int i=0;i<MAX_NUMBER_OF_PARAMETERS;i++)
        {
            target.draw(addSprite[i]);
            target.draw(addText[i]);
            if(isOfficer)
            {
                target.draw(addRSprite[i]);
                target.draw(addRText[i]);
            }
        }
	}
	target.draw(actualPlayerText);
	target.draw(miniature);
	target.draw(countText);
	for(int i=0;i<RESOURCE_TYPES_NUMBER;i++)
	{
	    target.draw(resourceSprite[i]);
        target.draw(resourceText[i]);
	}
	if(isPaused==1)
    {
        target.draw(pauseBack);
        target.draw(returnToGameButton);
        target.draw(saveGameButton);
        target.draw(restartGameButton);
        target.draw(escapeButton);
    }
    else if(isPaused==2)
    {
        target.draw(menuBack);
        for(int i=0;i<std::min(50,(int)data->getBuildingTypesSize());i++)
            target.draw(improvementChooseButton[i]);
        if(pressedType<255)
            target.draw(improvementOkButton);
        target.draw(improvementReturnButton);
        for(int i=0;i<MAX_NUMBER_OF_PARAMETERS;i++)
        {
            target.draw(addSprite[i]);
            target.draw(addText[i]);
        }
    }
    else if(isPaused==3)
    {
        if(canBeBuilt)
        {
            target.draw(buildOk);
        }
        target.draw(buildRet);
        for(int i=0;i<MAX_NUMBER_OF_PARAMETERS;i++)
        {
            target.draw(addSprite[i]);
            target.draw(addText[i]);
        }
        target.setView(state.view);
        target.draw(buildPropos);
        target.setView(state.uiView);
    }
    else if(isPaused==4)
    {
        target.draw(makeSureBack);
        target.draw(recruitOkButton);
        target.draw(recruitReturnButton);
        target.draw(recruitSlider);
        target.draw(recruitNrText);
    }
}
bool UI::makeSure(State& state, string text)
{
    makeSureText.setString(text);
    bool isMouseButtonPressed=true;
    uint8_t prevDrawState=state.drawState;
    state.drawState=MAKE_SURE_STATE;
    while(true)
    {
        while (appWindow.pollEvent(state.event))
        {
            if(!isMouseButtonPressed && makeSureYesButton.wasPressed(state.event, appWindow))
            {
                state.drawState=prevDrawState;
                return true;
            }
            if(!isMouseButtonPressed && makeSureNoButton.wasPressed(state.event, appWindow))
            {
                state.drawState=prevDrawState;
                return false;
            }
            if(state.event.type!=sf::Event::MouseButtonPressed) isMouseButtonPressed=false;
        }
    }
}
sf::String UI::insertTxt(State& state, string text)
{
    makeSureText.setString(text);
    uint8_t prevDrawState=state.drawState;
    state.drawState=INSERT_TEXT_STATE;
    sf::String name;
    insertText.setString(name);
    bool wasPressed=true;
    while(true)
    {
        while (appWindow.pollEvent(state.event))
        {
            if (!wasPressed && state.event.type == sf::Event::KeyPressed && state.event.key.code==sf::Keyboard::Escape)
            {
                bfUI::clickSound.play();
                name="";
                state.drawState=prevDrawState;
                return name;
            }
            #if SFML_VERSION_MAJOR==2 && SFML_VERSION_MINOR>=5
            else if (!wasPressed && state.event.type == sf::Event::KeyPressed && state.event.key.code==sf::Keyboard::Enter)
            #else
            if (state.event.type == sf::Event::KeyPressed && state.event.key.code==sf::Keyboard::Return)
            #endif
            {
                bfUI::clickSound.play();
                if(name.getSize()>0)
                {
                    state.drawState=prevDrawState;
                    return name;
                }
            }
            #if SFML_VERSION_MAJOR==2 && SFML_VERSION_MINOR>=5
            else if (!wasPressed && state.event.type == sf::Event::KeyPressed &&
                    (state.event.key.code==sf::Keyboard::Delete || state.event.key.code==sf::Keyboard::Backspace))
            #else
            else if (state.event.type == sf::Event::KeyPressed &&
                    (state.event.key.code==sf::Keyboard::Delete || state.event.key.code==sf::Keyboard::BackSpace))
            #endif
            {
                bfUI::clickSound.play();
                wasPressed=true;
                if(name.getSize()>0)
                {
                    name.erase(name.getSize()-1,1);
                    insertText.setString(name);
                }
            }
            else if (state.event.type == sf::Event::TextEntered)
            {
                if(state.event.text.unicode >= 32)
                {
                    bfUI::clickSound.play();
                    if(state.event.text.unicode == '_')
                        name += ' ';
                    else
                        name += state.event.text.unicode;
                    insertText.setString(name);
                }
            }
            if (state.event.type != sf::Event::KeyPressed)
                wasPressed=false;
        }
    }
}
bool UI::loadUI()
{
    targetCircle.setRadius(56.f);
    targetCircle.setOutlineThickness(16.f);
    targetCircle.setOrigin(-8.f,-8.f);
    targetCircle.setFillColor(sf::Color::Transparent);
    if(!UIBack.loadFromFile("data/ui-back.png"))
		return EXIT_FAILURE;
	UIBack.setSmooth(true);
	uiBack.setTexture(UIBack);
	grid.setTexture(data->getGridTexture());
	grid.setTextureRect(sf::IntRect(0,0,mapa.getSize().x*tileSize,mapa.getSize().y*tileSize));
	grid.setPosition(0,0);
	return EXIT_SUCCESS;
}
void UI::checkIfCanBeBuilt(const State& state)
{
    //if(mapa(my_vector2i(buildPropos.getPosition())/tileSize).level<0)
    if(mapa(mapToTile((my_vector2f)buildPropos.getPosition())).level<0)
        {canBeBuilt=false;buildPropos.setColor(sf::Color::Red);return;}
    if(mapa(mapToTile((my_vector2f)buildPropos.getPosition())).getOwner()<0
       || cities[mapa(mapToTile((my_vector2f)buildPropos.getPosition())).getOwner()].owner!=state.actualPlayer)
        {canBeBuilt=false;buildPropos.setColor(sf::Color::Red);return;}
    /*for(int i=0;i<buildings.size();i++)
    {
        if(buildings[i].getPosition()==buildPropos.getPosition())
            {canBeBuilt=false;buildPropos.setColor(sf::Color::Red);return;}
    }*/
    for(std::size_t i=0u;i<cities.size();i++)
    {
        if(cities[i].getPosition()==(my_vector2f)buildPropos.getPosition())
            {canBeBuilt=false;buildPropos.setColor(sf::Color::Red);return;}
        for(int j=0;j<cities[i].getBuildingNum();j++)
        {
            if(cities[i].getBuilding(j).getPosition()==(my_vector2f)buildPropos.getPosition())
                {canBeBuilt=false;buildPropos.setColor(sf::Color::Red);return;}
        }
    }
    if(typeid(*(data->getBuildingType(pressedType)))==typeid(ResourceBuilding))
    {
        if(dynamic_cast<ResourceBuilding*>(data->getBuildingType(pressedType).get())->givenResourceID!=(short)mapa(mapToTile((my_vector2f)buildPropos.getPosition())).resource)
            {canBeBuilt=false;buildPropos.setColor(sf::Color::Red);return;}
    }
    else if(typeid(*data->getBuildingType(pressedType))==typeid(Farm))
    {
        if((short)mapa(mapToTile((my_vector2f)buildPropos.getPosition())).resource>0 || mapa(mapToTile((my_vector2f)buildPropos.getPosition())).terrainType==sand)
            {canBeBuilt=false;buildPropos.setColor(sf::Color::Red);return;}
    }
    else if(typeid(*data->getBuildingType(pressedType))!=typeid(RecruitBuilding) && typeid(*data->getBuildingType(pressedType))!=typeid(RoadBuilding))
    {
        ///check if that tile is city tile
        if(mapa(mapToTile((my_vector2f)buildPropos.getPosition())).cityField<0)
            {canBeBuilt=false;buildPropos.setColor(sf::Color::Red);return;}
    }
    canBeBuilt=true;
    buildPropos.setColor(sf::Color::Green);
}

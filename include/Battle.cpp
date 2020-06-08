#include "Battle.h"
#include "basic/constants.hpp"
#include "basic/megaTexture.hpp"
#include "map.hpp"
#include "unit.hpp"
#include "player.hpp"
#include "Data.hpp"
#include <typeinfo>
#include <list>
#include <cmath>
#include <iostream>

#define RIVER_SAND 13
Battle::Battle(const Data& data): data(data)
{
    float scale=appWindow.getSize().y/1080.f;
    isForcedMelee=false;
    ///ESCAPE BUTTON
    escButton.unactiveColor=sf::Color(127,127,127);
    escButton.activeColor=sf::Color::White;
    escButton.setOutlineColor(sf::Color::Red);
    escButton.setOutlineThickness(1.f);
    escButton.setUnactiveColor();
    escButton.setPosition((appWindow.getSize().x-appWindow.getSize().y)*.5f+(60.f-64.f)*scale,124.f*scale);
	escButton.setScale(scale,scale);
    //escButton.icon.setTexture(&gameUI.uiMega[batExit],true);
    battleView.setCenter(my_vector2f(appWindow.getSize().x/2.f, appWindow.getSize().y/2.f));
	battleView.setSize(my_vector2f(appWindow.getSize().x, appWindow.getSize().y));
	battleGrid.setTexture(data.getGridTexture());
    nameText.setFont(data.getFont());
	nameText.setFillColor(sf::Color(0,0,127));
    nameText.setOutlineThickness(1.5f);
	nameText.setCharacterSize(30);
    nameText.setPosition(appWindow.getSize().x-256.f*scale,256.f*scale);
	nameText.setScale(scale,scale);
	for(int i=0;i<MAX_NUMBER_OF_PARAMETERS;i++)
	{
	    addSprite[i].setPosition(appWindow.getSize().x-256.f*scale,(290.f+i*30.f)*scale);
        addSprite[i].setScale(scale*.5f,scale*.5f);
        addText[i].setPosition(appWindow.getSize().x-166.f*scale,(290.f+i*30.f)*scale);
        addText[i].setFont(data.getFont());
        addText[i].setFillColor(sf::Color::Black);
        addText[i].setCharacterSize(30);
        addText[i].setScale(scale,scale);
        abText[i].setPosition(appWindow.getSize().x-256.f*scale,(290.f+i*30.f)*scale);
        abText[i].setFont(data.getFont());
        abText[i].setFillColor(sf::Color::Black);
        abText[i].setCharacterSize(30);
        abText[i].setScale(scale,scale);
	}
    for(int i=0;i<21;i++)
    {
        for(int j=0;j<21;j++)
        {
            movableTo[i][j].setRadius(16.f);
            movableTo[i][j].setFillColor(sf::Color(255,255,255,63));
            movableTo[i][j].setOutlineColor(sf::Color(0,0,0,63));
            movableTo[i][j].setOutlineThickness(2.f);
            movableTo[i][j].setScale(scale,scale);
        }
    }
	damageText.setPosition(appWindow.getSize().x-260.f*scale,650.f*scale);
	damageText.setScale(scale,scale);
	damageText.setFont(data.getFont());
	damageText.setFillColor(sf::Color::Black);
	damageText.setCharacterSize(30);
	counterDamText.setPosition(appWindow.getSize().x-260.f*scale,710.f*scale);
	counterDamText.setScale(scale,scale);
	counterDamText.setFont(data.getFont());
	counterDamText.setFillColor(sf::Color::Black);
	counterDamText.setCharacterSize(30);
	countText.setPosition(appWindow.getSize().x-50.f*scale,200.f*scale);
	countText.setScale(scale,scale);
	countText.setFont(data.getFont());
	countText.setFillColor(sf::Color(127,0,0));
	countText.setCharacterSize(45.f);
    miniature.setPosition(appWindow.getSize().x-260.f*scale,4.f*scale);
    miniature.setScale(scale*2.f,scale*2.f);
	uiBack.setTexture(gameUI.UIBack);
    uiBack.setPosition(appWindow.getSize().x-260.f*scale,0.f);
    uiBack.setScale(scale,scale);
    activeUnitCircle.setOutlineThickness(4.f);
    activeUnitCircle.setOutlineColor(sf::Color::Red);
    activeUnitCircle.setFillColor(sf::Color::Transparent);
    activeUnitCircle.setOrigin(-2.f,-2.f);
    activeUnitCircle.setScale(scale,scale);
}
void Battle::draw()
{
    appWindow.clear();
    for(int i=0;i<15+6*isFort;i++)
        for(int j=0;j<15+6*isFort;j++)
            appWindow.draw(battleTiles[i][j]);
    if(!moveOrder.empty())
    {
        for(int i=0;i<15+6*isFort;i++)
            for(int j=0;j<15+6*isFort;j++)
            {
                //if(moveCost[i][j]<=battleUnits[moveOrder.back()].mp*5+2)
                //{
                    appWindow.draw(movableTo[i][j]);
                //}
            }
    }
	appWindow.draw(uiBack);
    appWindow.draw(nameText);
    appWindow.draw(miniature);
	appWindow.draw(countText);
	appWindow.draw(damageText);
	appWindow.draw(counterDamText);
	for(int i=0;i<MAX_NUMBER_OF_PARAMETERS;i++)
	{
	    appWindow.draw(addSprite[i]);
        appWindow.draw(addText[i]);
        appWindow.draw(abText[i]);
	}
    appWindow.draw(battleGrid);
    for(std::size_t i=0u;i<battleUnits.size();i++)
    {
        //appWindow.draw(battleUnits[i]);
        battleUnits[i].battleDraw(appWindow);
        appWindow.draw(battleCountText[i]);
    }
    appWindow.draw(activeUnitCircle);
    appWindow.draw(escButton);
    appWindow.display();
}
bool Battle::beginBattle(State& state, Unit& attacker, Unit& defender, uint8_t dir, bool isFort)
{
	///no fort - 60 + 64-tileSize
	///is fort - 36 + 48-tileSize
    float scale=appWindow.getSize().y/1080.f;
    roundNr=0;
	this->isFort=isFort;
    my_vector2f tileScale;
    if(!isFort)
		tileScale = my_vector2f(scale*.5f,scale*.5f);
	else
		tileScale = my_vector2f(scale*.375f,scale*.375f);
    ///former in constructor (dependent on isFort)
    if(isFort)
		battleGrid.setTextureRect(sf::IntRect(0,0,21*tileSize,21*tileSize));
    else
		battleGrid.setTextureRect(sf::IntRect(0,0,15*tileSize,15*tileSize));
    battleGrid.setScale(tileScale);
    float uiBackHalfWidth=uiBack.getTexture()->getSize().x*.5f;
    battleGrid.setPosition((appWindow.getSize().x-appWindow.getSize().y-uiBackHalfWidth)*.5f+(60.f-24.f*isFort)*scale,(60.f-24.f*isFort)*scale);
    bctPos=my_vector2f((40.f-16.f*isFort)*scale,(24.f-12.f*isFort)*scale);
    for(int i=0;i<21;i++)
        for(int j=0;j<21;j++)
		{
			battleTiles[i][j].setScale(tileScale);
			if(isFort)
			{
				battleTiles[i][j].setPosition((appWindow.getSize().x-appWindow.getSize().y-uiBackHalfWidth)*.5f+(36.f+i*48.f)*scale,(36.f+j*48.f)*scale);
				movableTo[i][j].setPosition((appWindow.getSize().x-appWindow.getSize().y-uiBackHalfWidth)*.5f+(44.f+i*48.f)*scale,(44.f+j*48.f)*scale);
			}
			else
			{
				battleTiles[i][j].setPosition((appWindow.getSize().x-appWindow.getSize().y-uiBackHalfWidth)*.5f+(60.f+i*64.f)*scale,(60.f+j*64.f)*scale);
				movableTo[i][j].setPosition((appWindow.getSize().x-appWindow.getSize().y-uiBackHalfWidth)*.5f+(76.f+i*64.f)*scale,(76.f+j*64.f)*scale);
			}
		}
    attackerOwner=attacker.owner;
    if(isFort)
		activeUnitCircle.setRadius(22.f);
	else
		activeUnitCircle.setRadius(30.f);
    appWindow.setView(battleView);
    my_vector2i pos;
    pos.x=std::min(std::max(3.f+isFort,defender.getPosition().x/128.f),mapa.getSize().x-3.f-isFort);
	pos.y=std::min(std::max(3.f+isFort,defender.getPosition().y/128.f),mapa.getSize().y-3.f-isFort);
	for(int i=15;i<15+6*isFort;i++)
		for(int j=15;j<15+6*isFort;j++)
			type[i][j]=grass;
    for(int i=0;i<5+2*isFort;i++)
        for(int j=0;j<5+2*isFort;j++)
        {
            for(int k=0;k<9;k++)
			{
				if(isFort && 3*i+k%3>=7 && 3*i+k%3<=13 && 3*j+k/3>=7 && 3*j+k/3<=13)
				{
					if((3*i+k%3==7 || 3*i+k%3==13 || 3*j+k/3==7 || 3*j+k/3==13))
					{
						if(3*i+k%3!=10 && 3*j+k/3!=10)
                        {
                        	data.getMainMegaTexture().assignTexture(battleTiles[3*i+k%3][3*j+k/3],wall);
							type[3*i+k%3][3*j+k/3]=wall;
                        }
                        else
						{
                        	data.getMainMegaTexture().assignTexture(battleTiles[3*i+k%3][3*j+k/3],gate);
							type[3*i+k%3][3*j+k/3]=gate;
						}
					}
					else
					{
                        data.getMainMegaTexture().assignTexture(battleTiles[3*i+k%3][3*j+k/3],grass);
						type[3*i+k%3][3*j+k/3]=grass;
					}
					continue;
				}
                if(mapa(pos.x+i-2-isFort,pos.y+j-2-isFort).level>=0)
                {
                    bool wasSet=false;
                    if(!mapa(pos.x+i-2-isFort,pos.y+j-2-isFort).isRiver)
                    {
                    	switch(k)
                    	{
						case 0:
                            if(pos.x+i>=3+isFort && pos.y+j>=3+isFort && !mapa(pos.x+i-3-isFort,pos.y+j-3-isFort).isRiver
							&& mapa(pos.x+i-3-isFort,pos.y+j-2-isFort).isRiver
							&& mapa(pos.x+i-2-isFort,pos.y+j-3-isFort).isRiver)
							{
								data.getMainMegaTexture().assignTexture(battleTiles[3*i+k%3][3*j+k/3],sand);
								type[3*i+k%3][3*j+k/3]=RIVER_SAND;
								wasSet=true;
							}
							break;
						case 2: ///up right
							if(pos.x+i<mapa.getSize().x && pos.y+j>=3+isFort
							&& !mapa(pos.x+i-1-isFort,pos.y+j-3-isFort).isRiver
							&& mapa(pos.x+i-1-isFort,pos.y+j-2-isFort).isRiver
							&& mapa(pos.x+i-2-isFort,pos.y+j-3-isFort).isRiver)
							{
								data.getMainMegaTexture().assignTexture(battleTiles[3*i+k%3][3*j+k/3],sand);
								type[3*i+k%3][3*j+k/3]=RIVER_SAND;
								wasSet=true;
							}
							break;
						case 6:
							if(pos.x+i>=3+isFort && pos.y+j<mapa.getSize().y
							&& !mapa(pos.x+i-3-isFort,pos.y+j-1-isFort).isRiver
							&& mapa(pos.x+i-3-isFort,pos.y+j-2-isFort).isRiver
							&& mapa(pos.x+i-2-isFort,pos.y+j-1-isFort).isRiver)
							{
								data.getMainMegaTexture().assignTexture(battleTiles[3*i+k%3][3*j+k/3],sand);
								type[3*i+k%3][3*j+k/3]=RIVER_SAND;
								wasSet=true;
							}
							break;
						case 8: ///down right
							if(pos.x+i<mapa.getSize().x && pos.y+j<mapa.getSize().y
							&& !mapa(pos.x+i-1-isFort,pos.y+j-1-isFort).isRiver
							&& mapa(pos.x+i-1-isFort,pos.y+j-2-isFort).isRiver
							&& mapa(pos.x+i-2-isFort,pos.y+j-1-isFort).isRiver)
							{
								data.getMainMegaTexture().assignTexture(battleTiles[3*i+k%3][3*j+k/3],sand);
								type[3*i+k%3][3*j+k/3]=RIVER_SAND;
								wasSet=true;
							}
							break;
						default:
							break;
                    	}
                    	if(!wasSet)
                        {
                        	data.getMainMegaTexture().assignTexture(battleTiles[3*i+k%3][3*j+k/3],mapa(pos.x+i-2-isFort,pos.y+j-2-isFort).terrainType);
							type[3*i+k%3][3*j+k/3]=mapa(pos.x+i-2,pos.y+j-2).terrainType;
                        }
                        continue;
                    }
                    switch(k)
                    {
                    case 0: ///up left
                        if(pos.x+i>=3+isFort && pos.y+j>=3+isFort
						&& mapa(pos.x+i-3-isFort,pos.y+j-3-isFort).isRiver
						&& !mapa(pos.x+i-3-isFort,pos.y+j-2-isFort).isRiver
						&& !mapa(pos.x+i-2-isFort,pos.y+j-3-isFort).isRiver)
                        {
                            data.getMainMegaTexture().assignTexture(battleTiles[3*i+k%3][3*j+k/3],bay);
                            type[3*i+k%3][3*j+k/3]=bay;
                            wasSet=true;
                        }
                        else if(pos.x+i<3+isFort || pos.y+j<3+isFort ||
						(!mapa(pos.x+i-3-isFort,pos.y+j-3-isFort).isRiver
						&& !mapa(pos.x+i-3-isFort,pos.y+j-2-isFort).isRiver
						&& !mapa(pos.x+i-2-isFort,pos.y+j-3-isFort).isRiver))
                        {
                            data.getMainMegaTexture().assignTexture(battleTiles[3*i+k%3][3*j+k/3],mapa(pos.x+i-2-isFort,pos.y+j-2-isFort).terrainType);
                            type[3*i+k%3][3*j+k/3]=mapa(pos.x+i-2-isFort,pos.y+j-2-isFort).terrainType;
                            wasSet=true;
                        }
                        break;
                    case 1: ///up
                        if(pos.y+j>=3+isFort && mapa(pos.x+i-2-isFort,pos.y+j-3-isFort).isRiver)
                        {
                            data.getMainMegaTexture().assignTexture(battleTiles[3*i+k%3][3*j+k/3],bay);
                            type[3*i+k%3][3*j+k/3]=bay;
                            wasSet=true;
                        }
                        break;
                    case 2: ///up right
                        if(pos.x+i<mapa.getSize().x && pos.y+j>=3+isFort &&
						mapa(pos.x+i-1-isFort,pos.y+j-3-isFort).isRiver
						&& !mapa(pos.x+i-1-isFort,pos.y+j-2-isFort).isRiver
						&& !mapa(pos.x+i-2-isFort,pos.y+j-3-isFort).isRiver)
                        {
                            data.getMainMegaTexture().assignTexture(battleTiles[3*i+k%3][3*j+k/3],bay);
                            type[3*i+k%3][3*j+k/3]=bay;
                            wasSet=true;
                        }
                        else if(pos.x+i>=mapa.getSize().x || pos.y+j<3+isFort ||
						(!mapa(pos.x+i-1-isFort,pos.y+j-3-isFort).isRiver
						&& !mapa(pos.x+i-1-isFort,pos.y+j-2-isFort).isRiver
						&& !mapa(pos.x+i-2,pos.y+j-3).isRiver))
                        {
                            data.getMainMegaTexture().assignTexture(battleTiles[3*i+k%3][3*j+k/3],mapa(pos.x+i-2-isFort,pos.y+j-2-isFort).terrainType);
                            type[3*i+k%3][3*j+k/3]=mapa(pos.x+i-2,pos.y+j-2).terrainType;
                            wasSet=true;
                        }
                        break;
                    case 3: ///left
                        if(pos.x+i>=3+isFort && mapa(pos.x+i-3-isFort,pos.y+j-2-isFort).isRiver)
                        {
                            data.getMainMegaTexture().assignTexture(battleTiles[3*i+k%3][3*j+k/3],bay);
                            type[3*i+k%3][3*j+k/3]=bay;
                            wasSet=true;
                        }
                        break;
                    case 4: ///centre
                        data.getMainMegaTexture().assignTexture(battleTiles[3*i+k%3][3*j+k/3],bay);
                        type[3*i+k%3][3*j+k/3]=bay;
                        wasSet=true;
                        break;

                    case 5: ///right
                        if(pos.x+i<mapa.getSize().x && mapa(pos.x+i-1-isFort,pos.y+j-2-isFort).isRiver)
                        {
                            data.getMainMegaTexture().assignTexture(battleTiles[3*i+k%3][3*j+k/3],bay);
                            type[3*i+k%3][3*j+k/3]=bay;
                            wasSet=true;
                        }
                        break;
                    case 6: ///down left
                        if(pos.x+i>=3+isFort && pos.y+j<mapa.getSize().y
						&& mapa(pos.x+i-3-isFort,pos.y+j-1-isFort).isRiver
						&& !mapa(pos.x+i-3-isFort,pos.y+j-2-isFort).isRiver
						&& !mapa(pos.x+i-2-isFort,pos.y+j-1-isFort).isRiver)
                        {
                            data.getMainMegaTexture().assignTexture(battleTiles[3*i+k%3][3*j+k/3],bay);
                            type[3*i+k%3][3*j+k/3]=bay;
                            wasSet=true;
                        }
                        else if((pos.x+i<3+isFort && pos.y+j>=mapa.getSize().y) ||
						(!mapa(pos.x+i-3-isFort,pos.y+j-1-isFort).isRiver
						&& !mapa(pos.x+i-3-isFort,pos.y+j-2-isFort).isRiver
						&& !mapa(pos.x+i-2-isFort,pos.y+j-1-isFort).isRiver))
                        {
                            data.getMainMegaTexture().assignTexture(battleTiles[3*i+k%3][3*j+k/3],mapa(pos.x+i-2-isFort,pos.y+j-2-isFort).terrainType);
                            type[3*i+k%3][3*j+k/3]=mapa(pos.x+i-2,pos.y+j-2).terrainType;
                            wasSet=true;
                        }
                        break;
                    case 7: ///down
                        if(pos.y+j<mapa.getSize().y && mapa(pos.x+i-2-isFort,pos.y+j-1-isFort).isRiver)
                        {
                            data.getMainMegaTexture().assignTexture(battleTiles[3*i+k%3][3*j+k/3],bay);
                            type[3*i+k%3][3*j+k/3]=bay;
                            wasSet=true;
                        }
                        break;
                    case 8: ///down right
                        if(pos.x+i<mapa.getSize().x
						&& pos.y+j<mapa.getSize().y
						&& mapa(pos.x+i-1-isFort,pos.y+j-1-isFort).isRiver
						&& !mapa(pos.x+i-1-isFort,pos.y+j-2-isFort).isRiver
						&& !mapa(pos.x+i-2-isFort,pos.y+j-1-isFort).isRiver)
                        {
                            data.getMainMegaTexture().assignTexture(battleTiles[3*i+k%3][3*j+k/3],bay);
                            type[3*i+k%3][3*j+k/3]=bay;
                            wasSet=true;
                        }
                        else if((pos.x+i>=mapa.getSize().x && pos.y+j>=mapa.getSize().y) ||
						(!mapa(pos.x+i-1-isFort,pos.y+j-1-isFort).isRiver
						&& !mapa(pos.x+i-1-isFort,pos.y+j-2-isFort).isRiver
						&& !mapa(pos.x+i-2-isFort,pos.y+j-1-isFort).isRiver))
                        {
                            data.getMainMegaTexture().assignTexture(battleTiles[3*i+k%3][3*j+k/3],mapa(pos.x+i-2-isFort,pos.y+j-2-isFort).terrainType);
                            type[3*i+k%3][3*j+k/3]=mapa(pos.x+i-2,pos.y+j-2).terrainType;
                            wasSet=true;
                        }
                        break;
                    }
                    if(!wasSet)
                    {
                        data.getMainMegaTexture().assignTexture(battleTiles[3*i+k%3][3*j+k/3],sand);
                        type[3*i+k%3][3*j+k/3]=RIVER_SAND;
                    }
                }
                else
                {
                    data.getMainMegaTexture().assignTexture(battleTiles[3*i+k%3][3*j+k/3],sea);
                    type[3*i+k%3][3*j+k/3]=sea;
                }
                //battleTiles[3*i+k%3][3*j+k/3].setTexture(*(mapa(pos.x+i-2,pos.y+j-2).getMainTexture()),true);
			}
        }
    if(typeid(*defender.type.get())!=typeid(officer))
    {
        battleUnits.push_back(Unit(defender.type));
        if(typeid(*(defender.type.get()))==typeid(ranged))
			battleUnits.back().numOfArrows=dynamic_cast<ranged*>(battleUnits.back().type.get())->capacity;
        battleUnits.back().setOwner(defender.owner);
        battleUnits.back().kount=defender.kount;
        battleUnits.back().setScale(tileScale);
        battleCountText.push_back(sf::Text(my_to_string(battleUnits.back().kount),data.getFont(),32));
        battleCountText.back().setScale(scale,scale);
        battleCountText.back().setFillColor(players[battleUnits.back().owner].playerColor);
        battleCountText.back().setOutlineColor(sf::Color::White);
        battleCountText.back().setOutlineThickness(1.f);
    }
    else
    {
        auto defT=dynamic_cast<officer*>(defender.type.get());
        int defSize=defT->maxSize;
        for(std::size_t i=0u;i<defender.commandedUnits.size();i++)
        {
            battleUnits.push_back(Unit(data.getUnitType(defender.commandedUnits[i].first)));
            if(typeid(*data.getUnitType(defender.commandedUnits[i].first))==typeid(ranged))
            {
                battleUnits.back().numOfArrows=dynamic_cast<ranged*>(data.getUnitType(defender.commandedUnits[i].first).get())->capacity;
            }
            battleUnits.back().setOwner(defender.owner);
            battleUnits.back().kount=defender.commandedUnits[i].second;
            battleUnits.back().setScale(tileScale);
            battleCountText.push_back(sf::Text(my_to_string(battleUnits.back().kount),data.getFont(),32));
            battleCountText.back().setScale(scale,scale);
            battleCountText.back().setFillColor(players[battleUnits.back().owner].playerColor);
            battleCountText.back().setOutlineColor(sf::Color::White);
            battleCountText.back().setOutlineThickness(1.f);
            int tmpPos=round(11.f*(i+1.f)/(defSize+1.f));
            //battleUnits.back().setPosition((appWindow.getSize().x-appWindow.getSize().y)*.5f+956.f*scale,(60.f+(15*(i+1)/(defSize+1))*64.f)*scale);
            if(!isFort)
            {
            	switch(dir)
				{
				case 0:
					battleUnits.back().setPosition(tileToBattle(14-tmpPos/2,9+tmpPos%2+tmpPos/2));
					//battleUnits.back().setPosition((appWindow.getSize().x-appWindow.getSize().y)*.5f+(60.f+(14-tmpPos/2)*64.f)*scale,(60.f+(9+tmpPos%2+tmpPos/2)*64.f)*scale);
					break;
				case 1:
					battleUnits.back().setPosition(tileToBattle(round(15.f*(i+1.f)/(defSize+1.f)-.5f),14));
					//battleUnits.back().setPosition((appWindow.getSize().x-appWindow.getSize().y)*.5f+(60.f+round(15.f*(i+1.f)/(defSize+1.f)-.5f)*64.f)*scale,956.f*scale);
					break;
				case 2:
					battleUnits.back().setPosition(tileToBattle(5-tmpPos%2-tmpPos/2,14-tmpPos/2));
					//battleUnits.back().setPosition((appWindow.getSize().x-appWindow.getSize().y)*.5f+(60.f+(5-tmpPos%2-tmpPos/2)*64.f)*scale,(60.f+(14-tmpPos/2)*64.f)*scale);
					break;
				case 3:
					battleUnits.back().setPosition(tileToBattle(14,round(14.5f-15.f*(i+1.f)/(defSize+1.f))));
					//battleUnits.back().setPosition((appWindow.getSize().x-appWindow.getSize().y)*.5f+956.f*scale,(60.f+round(15.f-15.f*(i+1.f)/(defSize+1.f)-.5f)*64.f)*scale);
					break;
				case 5:
					battleUnits.back().setPosition((appWindow.getSize().x-appWindow.getSize().y)*.5f+60.f*scale,(60.f+round(15.f*(i+1.f)/(defSize+1.f)-.5f)*64.f)*scale);
					break;
				case 6:
					battleUnits.back().setPosition((appWindow.getSize().x-appWindow.getSize().y)*.5f+(60.f+(9+tmpPos%2+tmpPos/2)*64.f)*scale,(60.f+(tmpPos/2)*64.f)*scale);
					break;
				case 7:
					battleUnits.back().setPosition((appWindow.getSize().x-appWindow.getSize().y)*.5f+(60.f+round(15.f*(i+1.f)/(defSize+1.f)-.5f)*64.f)*scale,60.f*scale);
					break;
				case 8:
					battleUnits.back().setPosition((appWindow.getSize().x-appWindow.getSize().y)*.5f+(60.f+(tmpPos/2)*64.f)*scale,(60.f+(5-tmpPos%2-tmpPos/2)*64.f)*scale);
					break;
				default:
					battleUnits.back().setPosition((appWindow.getSize().x-appWindow.getSize().y)*.5f+956.f*scale,round(15-60.f+(15*(i+1)/(defSize+1))*64.f)*scale);
					break;
				}
            }
            else
			{
				battleUnits.back().setPosition((appWindow.getSize().x-appWindow.getSize().y)*.5f+(36.f+(9+i%5)*48.f)*scale,(36.f+(9+i/5)*48.f)*scale);
			}
            battleCountText.back().setPosition(battleUnits.back().getPosition()+bctPos);
        }
    }
    auto attT=dynamic_cast<officer*>(defender.type.get());
    int attSize=attT->maxSize;
    ///setting attacker units - only for
    for(std::size_t i=0u;i<attacker.commandedUnits.size();i++)
    {
        battleUnits.push_back(Unit(data.getUnitType(attacker.commandedUnits[i].first)));
        battleUnits.back().setOwner(attacker.owner);
        battleUnits.back().kount=attacker.commandedUnits[i].second;
        battleUnits.back().setScale(tileScale);
        battleCountText.push_back(sf::Text(my_to_string(battleUnits.back().kount),data.getFont(),32));
        battleCountText.back().setScale(scale,scale);
        battleCountText.back().setFillColor(players[battleUnits.back().owner].playerColor);
        battleCountText.back().setOutlineColor(sf::Color::White);
        battleCountText.back().setOutlineThickness(1.f);
        int tmpPos=round(11.f*(i+1.f)/(attSize+1.f)); //index of place of unit (used if diagonal)
        switch(dir)
        {
        case 0:
            battleUnits.back().setPosition((appWindow.getSize().x-appWindow.getSize().y)*.5f+((60.f-24.f*isFort)+(tmpPos/2)*(64.f-16.f*isFort))*scale,((60.f-24.f*isFort)+(5-tmpPos%2-tmpPos/2)*(64.f-16.f*isFort))*scale);
            break;
        case 1:
            battleUnits.back().setPosition((appWindow.getSize().x-appWindow.getSize().y)*.5f+((60.f-24.f*isFort)+round((15.f+6.f*isFort)*(i+1.f)/(attSize+1.f)-.5f)*(64.f-16.f*isFort))*scale,(60.f-24.f*isFort)*scale);
            break;
        case 2:
            battleUnits.back().setPosition((appWindow.getSize().x-appWindow.getSize().y)*.5f+((60.f-24.f*isFort)+(9+6*isFort+tmpPos%2+tmpPos/2)*(64.f-16.f*isFort))*scale,(60.f-24.f*isFort+(tmpPos/2)*(64.f-16.f*isFort))*scale);
            break;
        case 3:
            battleUnits.back().setPosition((appWindow.getSize().x-appWindow.getSize().y)*.5f+(60.f-24.f*isFort)*scale,((60.f-24.f*isFort)+round((15.f+6.f*isFort)*(i+1.f)/(attSize+1.f)-.5f)*(64.f-16.f*isFort))*scale);
            break;
        case 5:
            battleUnits.back().setPosition((appWindow.getSize().x-appWindow.getSize().y)*.5f+(956.f+40.f*isFort)*scale,((60.f-24.f*isFort)+round((15.f+6.f*isFort)*(i+1.f)/(attSize+1.f)-.5f)*(64.f-16.f*isFort))*scale);
            break;
        case 6:
            battleUnits.back().setPosition((appWindow.getSize().x-appWindow.getSize().y)*.5f+((60.f-24.f*isFort)+(5-tmpPos%2-tmpPos/2)*(64.f-16.f*isFort))*scale,((60.f-24.f*isFort)+(14+6*isFort-tmpPos/2)*(64.f-16.f*isFort))*scale);
            break;
        case 7:
            battleUnits.back().setPosition((appWindow.getSize().x-appWindow.getSize().y)*.5f+((60.f-24.f*isFort)+round((15.f+6.f*isFort)-(15.f+6.f*isFort)*(i+1.f)/(attSize+1.f)-.5f)*(64.f-16.f*isFort))*scale,(956.f+40.f*isFort)*scale);
            break;
        case 8:
            battleUnits.back().setPosition((appWindow.getSize().x-appWindow.getSize().y)*.5f+((60.f-24.f*isFort)+(14+6*isFort-tmpPos/2)*(64.f-16.f*isFort))*scale,((60.f-24.f*isFort)+(9+6*isFort+tmpPos%2+tmpPos/2)*(64.f-16.f*isFort))*scale);
            break;
        default:
            battleUnits.back().setPosition((appWindow.getSize().x-appWindow.getSize().y)*.5f+((60.f-24.f*isFort)-24.f*isFort)*scale,((60.f-24.f*isFort)+round(15*(i+1)/(attSize+1))*(64.f-16.f*isFort))*scale);
            break;
        }
        battleCountText.back().setPosition(battleUnits.back().getPosition()+bctPos);
    }
    ///begin of the main loop
    std::cout << "Battle began\n";
    draw();
    if(battleLoop(state))
    {
        endBattle();
        return true; ///if window was closed
    }
    attacker.commandedUnits.clear();
    defender.commandedUnits.clear();
    bool isDefOff = (typeid(*defender.type.get())==typeid(officer));
    for(std::size_t i=0u;i<battleUnits.size();i++)
    {
        std::size_t j=0u;
        for(;j<data.getUnitTypeSize();j++)
        {
            if(battleUnits[i].type==data.getUnitType(j))
                break;
        }
        if(battleUnits[i].owner==attacker.owner)
            attacker.commandedUnits.push_back(std::make_pair(j,battleUnits[i].kount));
        else if(isDefOff)
            defender.commandedUnits.push_back(std::make_pair(j,battleUnits[i].kount));
        else
            defender=battleUnits[i];
    }
    endBattle();
    return false;
}
bool Battle::battleLoop(State& state)
{
    float scale=appWindow.getSize().y/1080.f;
    uint8_t isMouseButtonPressed=(sf::Mouse::isButtonPressed(sf::Mouse::Left));
    nextTurn();
    while(true)
    {
        while (appWindow.pollEvent(state.event))
        {
            if (state.event.type == sf::Event::Closed)
                return true;
            if (state.event.type == sf::Event::KeyPressed)
            {
				switch(state.event.key.code)
				{
				case sf::Keyboard::Escape:
                    return false;
				case sf::Keyboard::W:
					Wait();
					if(isBattleFinished()) return false;
					nextUnit();
					break;
                case sf::Keyboard::D: ///defend
                	Defend();
					if(isBattleFinished()) return false;
					nextUnit();
                    break;
				case sf::Keyboard::T: ///set forcing melee
					isForcedMelee=true;
					calculate(true);
					break;
				case sf::Keyboard::Y: ///unset forcing melee
					isForcedMelee=false;
					calculate(true);
					break;
                default:
                    break;
				}
			}
			if(players[battleUnits[moveOrder.back()].owner].isComp)
			{
				AiMove(state);
				if(isBattleFinished())
					return false;
				nextUnit();
			}
			else
			{
            my_vector2i mapPressPosition = (my_vector2i)appWindow.mapPixelToCoords(sf::Mouse::getPosition(appWindow));
            my_vector2i tile=mapPressPosition-(my_vector2i)battleTiles[0][0].getPosition();
            tile=my_vector2i(tile.x/((64.f-16.f*isFort)*scale),tile.y/((64.f-16.f*isFort)*scale));
            if(isMouseButtonPressed && escButton.wasPressed(state.event, appWindow))
            {
                return false;
            }
            if(sf::Mouse::isButtonPressed(sf::Mouse::Right) && !isMouseButtonPressed)
            {
                bool wasFound=false;
                isMouseButtonPressed = true;
                for(std::size_t i=0u;i<battleUnits.size();i++)
                {
                    if (battleUnits[i].getPosition().x-battleTiles[tile.x][tile.y].getPosition().x > (-63.f+16.f*isFort)*scale &&
                        battleUnits[i].getPosition().x-battleTiles[tile.x][tile.y].getPosition().x <= 0.f &&
                        battleUnits[i].getPosition().y-battleTiles[tile.x][tile.y].getPosition().y > (-63.f+16.f*isFort)*scale &&
                        battleUnits[i].getPosition().y-battleTiles[tile.x][tile.y].getPosition().y <= 0.f )
                    {
                        unitInfo(i);
                        wasFound=true;
                        break;
                    }
                }
                if(!wasFound)
                    unitInfo(-1);
            }
            else if(sf::Mouse::isButtonPressed(sf::Mouse::Left) && !isMouseButtonPressed)
            {
                isMouseButtonPressed = true;
                if(tile.x>=0 && tile.x<15+6*isFort && tile.y>=0 && tile.y<15+6*isFort)
                {
                    short isUnit=0; ///0-not, 1-your, 2-enemy's
                    short unitIndex;
                    for(std::size_t i=0u; i<battleUnits.size(); i++)
                    {
                        if (battleUnits[i].getPosition().x-battleTiles[tile.x][tile.y].getPosition().x > (-64.f+16.f*isFort)*scale &&
                            battleUnits[i].getPosition().x-battleTiles[tile.x][tile.y].getPosition().x <= 0.f &&
                            battleUnits[i].getPosition().y-battleTiles[tile.x][tile.y].getPosition().y > (-64.f+16.f*isFort)*scale &&
                            battleUnits[i].getPosition().y-battleTiles[tile.x][tile.y].getPosition().y <= 0.f )
                        {
                            if(int(battleUnits[moveOrder.back()].owner)==int(battleUnits[i].owner))
                                isUnit=1;
                            else
                                isUnit=2;
                            unitIndex=i;
                            break;
                        }
                    }
                    if(isUnit==0)
                    {
                    	if(!MoveTo(tile))
						{
							if(isBattleFinished()) return false;
							nextUnit();
						}
                    }
                    else if(isUnit==1)
                    {
                    }
                    else if(isUnit==2)
                    {
                        if(!isForcedMelee && typeid(*battleUnits[moveOrder.back()].type.get())==typeid(ranged))
                        {
							auto tmpR = dynamic_cast<ranged*>(battleUnits[moveOrder.back()].type.get());
							if(moveCost[tile.x][tile.y]<=10*tmpR->range+2) ///radius lower than doubled normal radius
							{
								if(moveCost[tile.x][tile.y]<=5*tmpR->range+2)
								{
									attack(state,moveOrder.back(),unitIndex,rangedAttack);
								}
								else ///doubled radius
								{
									attack(state,moveOrder.back(),unitIndex,halvedRangedAttack);
								}
								//battleCountText[unitIndex].setString(my_to_string(battleUnits[unitIndex].kount));
								isUnit=0;
							}
                        }
                        else
                        {
                        ///to melee attack
                            if(moveCost[tile.x][tile.y]<=5*battleUnits[moveOrder.back()].mp+30)
                            {
                                int tarX=int(battleUnits[unitIndex].getPosition().x-battleTiles[0][0].getPosition().x)/int((64.f-16.f*isFort)*scale);
                                int tarY=int(battleUnits[unitIndex].getPosition().y-battleTiles[0][0].getPosition().y)/int((64.f-16.f*isFort)*scale);
                                int minInd=0;
                                for(int i=0;i<9;i++)
                                {
                                    if(tarX==0 && i<3) continue;
                                    else if(tarX==15+6*isFort && i>5) continue;
                                    else if(tarY==0 && i%3==0) continue;
                                    else if(tarY==15+6*isFort && i%3==2) continue;
                                    else if(i==4) continue;
                                    if(moveCost[tarX+i/3][tarY+i%3]+2*i%2<moveCost[tarX-1+minInd/3][tarY-1+minInd%3]+2*minInd%2)
                                        minInd=i;
                                }
                                if(moveCost[tarX+minInd/3][tarY+minInd%3]<=5*battleUnits[moveOrder.back()].mp+2)
                                {
									if(!MoveTo(my_vector2i(tarX+minInd/3,tarY+minInd%3)))
									{
										attack(state,moveOrder.back(),unitIndex,meleeAttack);
										isUnit=0;
									}
                                }
                            }
                        }
						/*if(removeIfDestroyed(unitIndex))
							return false;*/
                        if(isUnit==0) ///unit did an action
						{
							if(isBattleFinished())
								return false;
							nextUnit();
						}
                    }
                }
            }
			}
			if(!sf::Mouse::isButtonPressed(sf::Mouse::Left) && !sf::Mouse::isButtonPressed(sf::Mouse::Right))
				isMouseButtonPressed=false;
        }
        draw();
    }
    appWindow.setView(state.view);
}
bool Battle::isBattleFinished()
{
    for(std::size_t i=0u;i<battleUnits.size();i++)
	{
        if(battleUnits[0].owner!=battleUnits[i].owner)
			return false;
	}
	return true;
}
bool Battle::removeIfDestroyed(int index)
{
	if(battleUnits[index].kount==0)
	{
		battleUnits.erase(battleUnits.begin()+index);
		uint8_t tmpOwn = battleUnits.begin()->owner;
		bool wasOther = false;
		for(std::size_t i=1u;i<battleUnits.size();i++)
		{
			if(battleUnits[i].owner!=tmpOwn)
			{
				wasOther = true;
				break;
			}
		}
		if(!wasOther)
			return true;
		battleCountText.erase(battleCountText.begin()+index);
		for(std::size_t i=0u;i<moveOrder.size();i++)
		{
			if(moveOrder[i]>index)
				moveOrder[i]--;
			else if(moveOrder[i]==index)
			{
				moveOrder.erase(moveOrder.begin()+i);
				i--;
			}
		}
	}
	return false;
}
void Battle::beginWaitPhase()
{
	std::cout << "Wait phase began\n";
	std::cout << waitOrder.size() << " units waiting\n";
	moveOrder = std::vector<int>(waitOrder.begin(), waitOrder.end());
    isWaitPhase=false;
    waitOrder.clear();
}
void Battle::nextUnit()
{
    moveOrder.pop_back();
    if(moveOrder.empty())
    {
        if(isWaitPhase && !waitOrder.empty())
            beginWaitPhase();
        else
            nextTurn();
    }
    battleUnits[moveOrder.back()].isDef=false;
    activeUnitCircle.setPosition(battleUnits[moveOrder.back()].getPosition());
    calculate();
}
void Battle::nextTurn()
{
	roundNr++;
	std::cout << roundNr << " turn begins\n";
    isWaitPhase=true;
    moveOrder.clear();
    short attUnits=0, defUnits=0;
    for(std::size_t i=0u;i<battleUnits.size();i++)
    {
        battleUnits[i].counterstroke=false;
        //battleUnits[i].isDef=false;
        moveOrder.push_back(i);
        if(battleUnits[i].owner==attackerOwner)
            attUnits++;
        else
            defUnits++;
    }
    ///move order rules (from most important to least)
    ///1. Movement points
    ///2. From up to down for units owned by the same player
    ///3. If at particular MP any player has more units than other, it has first move, then alternately (now: ownership)
    std::sort(moveOrder.begin(),moveOrder.end(),[&](int a, int b){
        if(battleUnits[a].mp!=battleUnits[b].mp)
            return battleUnits[a].mp<battleUnits[b].mp;
        return a<b;
    });
    activeUnitCircle.setPosition(battleUnits[moveOrder.back()].getPosition());
    /* short k=0;
    for(int i=1;i<=moveOrder.size();i++)
    {
        if(i==moveOrder.size() || battleUnits[moveOrder[i]].mp<battleUnits[moveOrder[k]].mp)
        {
            if(k+1<i) ///if there are more than 1 stacks of units with the same MP
            {
                std::vector<int> changeOrderTmp;
                short attAdv=0;
                for(int j=k;j<i;j++)
                    if(battleUnits[moveOrder[j]].owner==attackerOwner)
                        attAdv++;
                    else
                        attAdv--;
                if(attAdv<0) ///if defender has priority
                {

                }
            }
            else
            {
            if(battleUnits[moveOrder[k]].owner==attackerOwner)
                attUnits--;
            else
                defUnits--;
            }
            k=i;
        }
    }*/
    calculate();
}

void Battle::calculate(bool changeMelee)
{
	if(!changeMelee)
    {
		///calculating move costs
		//bool wasSet[21][21];
		for(int i=0;i<15+6*isFort;i++)
			for(int j=0;j<15+6*isFort;j++)
			{
				moveCost[i][j]=SHRT_MAX;
				//wasSet[i][j]=false;
			}
		std::list<my_vector2i> visiting;
		my_vector2i tile=battleToTile(activeUnitCircle.getPosition());
		//my_vector2i tile=my_vector2i(activeUnitCircle.getPosition()/scale-battleTiles[0][0].getPosition()/scale)/(64-16*isFort)+my_vector2i(1,1);
		moveCost[tile.x][tile.y]=0;
		visiting.push_back(tile);
		while(!visiting.empty())
		{
			short x=visiting.begin()->x; short y=visiting.begin()->y;
			for(int j=0;j<9;j++)
			{
				if(j==4) continue;
				else if(j%3==0 && x==0) continue;
				else if(j<3 && y==0) continue;
				else if(j%3==2 && x>=14+6*isFort) continue;
				else if(j>5 && y>=14+6*isFort) continue;
				else if(moveCost[x-1+j%3][y-1+j/3]>moveCost[x][y]+1)
				{
				    int cost = 7;
				    if(type[x-1+j%3][y-1+j/3]==sea)
                    {
                        continue;
                    }
				    if(type[x-1+j%3][y-1+j/3]==bay)
                        cost=21;
                    else if(type[x-1+j%3][y-1+j/3]==RIVER_SAND)
                        cost=14;
                    else if(type[x-1+j%3][y-1+j/3]==wall)
                        cost=1000;
                    else if(type[x-1+j%3][y-1+j/3]==gate && battleUnits[moveOrder.back()].owner!=battleUnits[0].owner)
						cost=1000;
					if(moveCost[x-1+j%3][y-1+j/3]>moveCost[x][y]+cost-int(cost/3.499)*(j%2))
					{
						moveCost[x-1+j%3][y-1+j/3]=moveCost[x][y]+cost-int(cost/3.499)*(j%2);
						visiting.push_back(my_vector2i(x-1+j%3,y-1+j/3));
					}
				}
			}
			visiting.pop_front();
		}
		///calculating range attack costs

		/*bool isInFort = isFort
		&& battleUnits[moveOrder.back()].getPosition().x>=battleTiles[7][7].getPosition().x
		&& battleUnits[moveOrder.back()].getPosition().y>=battleTiles[7][7].getPosition().y
		&& battleUnits[moveOrder.back()].getPosition().x<=battleTiles[13][13].getPosition().x
		&& battleUnits[moveOrder.back()].getPosition().y<=battleTiles[13][13].getPosition().y;*/
		rangeCost[tile.x][tile.y]=0;
		for(int i=0;i<15+6*isFort;i++)
			for(int j=0;j<15+6*isFort;j++)
			{
				if(tile.x==i && tile.y==j) continue;
				int tmpRan=0;
				bool isX = std::abs(tile.x-i)-std::abs(tile.y-j)>=0.f;
				bool isPosit;
				if(isX)
					isPosit=tile.x<=i;
				else
					isPosit=tile.y<=j;
				for(int k=1;k<=std::max(std::abs(tile.x-i),std::abs(tile.y-j));k++)
				{
					int tmpOth, prevTmpOth;
					//uint8_t tmpTile;
					short cost=5;
					if(isX)
					{
						tmpOth=std::round((float)k/abs(tile.x-i)*(tile.y-j));
						prevTmpOth=std::round((k-1.f)/abs(tile.x-i)*(tile.y-j));
						if(tile.x+(2*isPosit-1)*k<0 || tile.x+(2*isPosit-1)*k>=15+6*isFort)
						{
							std::cout << "XH " << i << " " << j << " " << k << "\n";
							//std::cout << tile.x+tmpOth << " " << tile.y+k*(2*isPosit-1) << "\n";
							//cost=15;
						}
						if(isFort)
						{
							if(type[tile.x+(2*isPosit-1)*k][tile.y-tmpOth]==wall || type[tile.x+(2*isPosit-1)*k][tile.y-tmpOth]==gate)
							{
								//std::cout << "XW\n";
								if(tile.x>7 && tile.x<13 && tile.y>7 && tile.y<13)
									cost=0;
								else
									cost=15;
							}
						}
						if(tmpOth!=prevTmpOth)
                            tmpRan+=cost*1.4f;
						else
							tmpRan+=cost;
					}
					else
					{
						int tmpOth=std::round((float)k/abs(tile.y-j)*(tile.x-i));
						int prevTmpOth=std::round((k-1.f)/abs(tile.y-j)*(tile.x-i));
						if(tile.y+(2*isPosit-1)*k<0 || tile.y+(2*isPosit-1)*k>=15+6*isFort)
						{
							std::cout << "YV " << i << " " << j << " " << k << "\n";
							//std::cout << tile.x-tmpOth << " " << tile.y-k*(2*isPosit-1) << "\n";
						}
						if(isFort)
						{
							if(type[tile.x-tmpOth][tile.y+k*(2*isPosit-1)]==wall || type[tile.x-tmpOth][tile.y+k*(2*isPosit-1)]==gate)
							{
								//std::cout << "YW\n";
								if(tile.x>7 && tile.x<13 && tile.y>7 && tile.y<13)
									cost=0;
								else
									cost=15;
							}
						}
						if(tmpOth!=prevTmpOth)
                            tmpRan+=cost*1.4f;
						else
							tmpRan+=cost;
					}
				}
				rangeCost[i][j]=tmpRan;
				//wasSet[i][j]=false;
			}
		/*rangeCost[tile.x][tile.y]=0;
		visiting.push_back(tile);
		while(!visiting.empty())
		{
			short x=visiting.begin()->x; short y=visiting.begin()->y;
			for(int j=0;j<9;j++)
			{
				if(j==4) continue;
				else if(j%3==0 && x==0) continue;
				else if(j<3 && y==0) continue;
				else if(j%3==2 && x==14+6*isFort) continue;
				else if(j>5 && y==14+6*isFort) continue;
				else if(rangeCost[x-1+j%3][y-1+j/3]>rangeCost[x][y])
				{
					int cost=5;
					if(type[x-1+j%3][y-1+j/3]==wall || type[x-1+j%3][y-1+j/3]==gate)
					{
						if(isInFort) cost=0;
						else cost=20;
					}
					if(rangeCost[x-1+j%3][y-1+j/3]>rangeCost[x][y]+7-int(cost/2.5f)*(j%2))
					{
						rangeCost[x-1+j%3][y-1+j/3]=rangeCost[x][y]+7-2*(j%2);
						visiting.push_back(my_vector2i(x-1+j%3,y-1+j/3));
					}
				}
			}
			visiting.pop_front();
		}*/
    }
    ///dark blue - both move and full range (5)
    ///green - only full range (4)
    ///white - move and half range (3)
    ///red - half range (2)
    ///gray - only move (1)
    for(int i=0;i<15+6*isFort;i++)
		for(int j=0;j<15+6*isFort;j++)
		{
			if(!isForcedMelee && typeid(*(battleUnits[moveOrder.back()].type.get()))==typeid(ranged))
			{
				auto tmpR = dynamic_cast<ranged*>(battleUnits[moveOrder.back()].type.get());
				uint8_t colCase = '\0'; //
				if (moveCost[i][j]<=5*battleUnits[moveOrder.back()].mp+2) colCase++;
				if (rangeCost[i][j]<=5*tmpR->range+2) colCase+=4;
				else if (rangeCost[i][j]<=10*tmpR->range+2) colCase+=2;
				switch(colCase)
				{
				case 1:
					movableTo[i][j].setFillColor(sf::Color(127,127,127,95));
					movableTo[i][j].setOutlineColor(sf::Color(0,0,0,95));
					break;
				case 2:
					movableTo[i][j].setFillColor(sf::Color(255,0,0,95));
					movableTo[i][j].setOutlineColor(sf::Color(0,0,0,95));
					break;
				case 3:
					movableTo[i][j].setFillColor(sf::Color(255,255,255,95));
					movableTo[i][j].setOutlineColor(sf::Color(0,0,0,95));
					break;
				case 4:
					movableTo[i][j].setFillColor(sf::Color(0,255,0,95));
					movableTo[i][j].setOutlineColor(sf::Color(0,0,0,95));
					break;
				case 5:
					movableTo[i][j].setFillColor(sf::Color(0,0,127,95));
					movableTo[i][j].setOutlineColor(sf::Color(0,0,0,95));
					break;
				default:
					movableTo[i][j].setFillColor(sf::Color::Transparent);
					movableTo[i][j].setOutlineColor(sf::Color::Transparent);
				}
			}
			else
			{
				if(moveCost[i][j]<=5*battleUnits[moveOrder.back()].mp+2)
				{
					movableTo[i][j].setFillColor(sf::Color(255,255,255,95));
					movableTo[i][j].setOutlineColor(sf::Color(0,0,0,95));
				}
				else
				{
					movableTo[i][j].setFillColor(sf::Color::Transparent);
					movableTo[i][j].setOutlineColor(sf::Color::Transparent);
				}
			}
		}
}
void Battle::unitInfo(int index) //-1 - no
{
	if(index==-1)
	{
	    nameText.setString("");
		countText.setString("");
		miniature.setTexture(data.getNullTexture());
		damageText.setString("");
		counterDamText.setString("");
		for(int i=0;i<MAX_NUMBER_OF_PARAMETERS;i++)
		{
		    addText[i].setString("");
		    abText[i].setString("");
			addText[i].setFillColor(sf::Color::Black);
		    addSprite[i].setTexture(data.getNullTexture());
		}
		return;
	}
	nameText.setString(battleUnits[index].type->name);
    nameText.setFillColor(players[battleUnits[index].owner].playerColor);
    nameText.setOutlineColor(sf::Color::Black);
	countText.setString(my_to_string(battleUnits[index].kount));
	addText[0].setString(my_to_string(battleUnits[index].type->maxMP));
	data.getUiMegaTexture().assignTexture(addSprite[0],mpID);
	//addSprite[0].setTexture(uiMega[mpID], true);
	addText[1].setString(my_to_string(battleUnits[index].hp)+"/"+my_to_string(battleUnits[index].type->maxHP));
    data.getUiMegaTexture().assignTexture(addSprite[1],hpID);
	addText[2].setString(my_to_string(battleUnits[index].type->att));
    data.getUiMegaTexture().assignTexture(addSprite[2],attID);
    if(battleUnits[index].isDef)
		addText[3].setFillColor(sf::Color(0,127,0));
    else
		addText[3].setFillColor(sf::Color::Black);
	addText[3].setString(my_to_string(battleUnits[index].type->def*(1+battleUnits[index].isDef)));
    data.getUiMegaTexture().assignTexture(addSprite[3],defID);
	addText[4].setString(my_to_string(battleUnits[index].type->str));
    data.getUiMegaTexture().assignTexture(addSprite[4],str);
	for(int i=0;i<5;i++)
		abText[i].setString("");
    //addSprite[1].setTexture(uiMega[hpID], true);
	std::vector<std::string> tmpAbis = battleUnits[index].type->abilities.getNames();
    if(typeid(*(battleUnits[index].type.get()))==typeid(melee))
    {
        for(int i=0;i<std::min((int)MAX_NUMBER_OF_PARAMETERS-5,(int)(tmpAbis.size()));i++)
		{
			abText[i+5].setString(tmpAbis[i]);
            addText[i+5].setString("");
            addSprite[i+5].setTexture(data.getNullTexture());
		}
        for(int i=5+tmpAbis.size();i<MAX_NUMBER_OF_PARAMETERS;i++)
        {
			abText[i].setString("");
            addText[i].setString("");
            addSprite[i].setTexture(data.getNullTexture());
        }
        //addSprite[2].setTexture(gameUI.uiMega[str], true);
    }
    else if(typeid(*(battleUnits[index].type.get()))==typeid(ranged))
    {
		for(int i=5;i<8;i++)
			abText[i].setString("");
        ranged* tmp57045=dynamic_cast<ranged*>((battleUnits[index].type.get()));
        addText[5].setString(my_to_string(tmp57045->rangedStr));
        data.getUiMegaTexture().assignTexture(addSprite[5],ranStr);
        addText[6].setString(my_to_string(tmp57045->range));
        data.getUiMegaTexture().assignTexture(addSprite[6],rang);
        addText[7].setString(my_to_string(battleUnits[index].numOfArrows)+"/"+my_to_string(tmp57045->capacity));
        data.getUiMegaTexture().assignTexture(addSprite[7],capac);
        for(int i=0;i<std::min((int)MAX_NUMBER_OF_PARAMETERS-8,(int)(tmpAbis.size()));i++)
		{
			abText[i+8].setString(tmpAbis[i]);
			addText[i+8].setString("");
		}
        for(int i=8+tmpAbis.size();i<MAX_NUMBER_OF_PARAMETERS;i++)
        {
            addText[i].setString("");
            addSprite[i].setTexture(data.getNullTexture());
        }
        //addSprite[5].setTexture(gameUI.uiMega[capac], true);
    }
    battleUnits[index].type->textureUP.get()->assignTexture(miniature,0);
    //battleUnits[index].type->mega->assignTexture(miniature,battleUnits[index].type->unitTextureInd);
    if(battleUnits[index].owner==battleUnits[moveOrder.back()].owner)
    {
		damageText.setString("");
		counterDamText.setString("");
		return;
    }
    std::pair<int,int> dealtDamage = std::make_pair(0,0);
    std::pair<int,int> dying = std::make_pair(0,0);
    my_vector2i indexTile=(my_vector2i)(battleUnits[index].getPosition()-(my_vector2f)battleTiles[0][0].getPosition());
    float scale = appWindow.getSize().y/1080.f;
	indexTile=my_vector2i(indexTile.x/((64.f-16.f*isFort)*scale),indexTile.y/((64.f-16.f*isFort)*scale));
	bool isRangeAttack=!isForcedMelee && typeid(*(battleUnits[moveOrder.back()].type.get()))==typeid(ranged);
    if(isRangeAttack)
    {
		ranged* tmpRan = dynamic_cast<ranged*>(battleUnits[moveOrder.back()].type.get());
		if(rangeCost[indexTile.x][indexTile.y]<=5*tmpRan->range+2)
			dealtDamage = battleUnits[moveOrder.back()].calculateDamage(battleUnits[index],rangedAttack);
		else if(rangeCost[indexTile.x][indexTile.y]<=10*tmpRan->range+2)
			dealtDamage = battleUnits[moveOrder.back()].calculateDamage(battleUnits[index],halvedRangedAttack);
    }
    else
    {
		bool canMove=false;
		if(moveCost[indexTile.x][indexTile.y]>5*battleUnits[moveOrder.back()].type->maxMP+2)
		{
			for(int i=0;i<9;i++)
			{
				if((i<3 && indexTile.x==0) || (i>5 && indexTile.x==14+6*isFort) || i==4 || (i%3 && indexTile.y==0) || (i%3==2 && indexTile.y==14+6*isFort)) continue;
				if(moveCost[indexTile.x+i/3][indexTile.y+i%3]<=5*battleUnits[moveOrder.back()].type->maxMP+2)
				{
					canMove=true;
					break;
				}
			}
		}
		else canMove=true;
		if(canMove)
			dealtDamage = battleUnits[moveOrder.back()].calculateDamage(battleUnits[index],meleeAttack);
    }
    if(dealtDamage.second==0)
	{
		damageText.setString("");
		counterDamText.setString("");
		return;
	}
	dying.first = std::min(dealtDamage.first/battleUnits[index].type->maxHP,battleUnits[index].kount)+((dealtDamage.first%battleUnits[index].type->maxHP)>=battleUnits[index].hp);
	dying.second = std::min(dealtDamage.second/battleUnits[index].type->maxHP,battleUnits[index].kount)+((dealtDamage.second%battleUnits[index].type->maxHP)>=battleUnits[index].hp);
	if(dying.first!=dying.second)
		damageText.setString("Deals "+MToStr(dealtDamage.first)+"-"+MToStr(dealtDamage.second)+" dam.\n("+MToStr(dying.first)+"-"+MToStr(dying.second)+" dies)");
	else
		damageText.setString("Deals "+MToStr(dealtDamage.first)+"-"+MToStr(dealtDamage.second)+" dam.\n("+MToStr(dying.first)+" dies)");
    if(isRangeAttack || battleUnits[index].counterstroke)
    {
    	//if(battleUnits[moveOrder.back()])
		counterDamText.setString("");
		return;
	}
	dealtDamage.first = battleUnits[index].calculateDamage(battleUnits[moveOrder.back()],meleeAttack,dying.second).first;
	dealtDamage.second = battleUnits[index].calculateDamage(battleUnits[moveOrder.back()],meleeAttack,dying.first).second;
    dying.first = std::min(dealtDamage.first/battleUnits[moveOrder.back()].type->maxHP,battleUnits[moveOrder.back()].kount)+((dealtDamage.first%battleUnits[moveOrder.back()].type->maxHP)>=battleUnits[moveOrder.back()].hp);
	dying.second = std::min(dealtDamage.second/battleUnits[moveOrder.back()].type->maxHP,battleUnits[moveOrder.back()].kount)+((dealtDamage.second%battleUnits[moveOrder.back()].type->maxHP)>=battleUnits[moveOrder.back()].hp);
	if(dying.first!=dying.second)
		counterDamText.setString("Takes "+MToStr(dealtDamage.first)+"-"+MToStr(dealtDamage.second)+"\n("+MToStr(dying.first)+"-"+MToStr(dying.second)+" killed)");
	else
		counterDamText.setString("Takes "+MToStr(dealtDamage.first)+"-"+MToStr(dealtDamage.second)+"\n("+MToStr(dying.first)+" killed)");
	//miniature.setTexture(units[index].type->getUnitTexture());
}
void Battle::endBattle()
{
    battleUnits.clear();
    battleCountText.clear();
    moveOrder.clear();
}
void Battle::Wait()
{
	if(!isWaitPhase)
		return;
	std::cout << "Unit " << moveOrder.back() << " waits\n";
	waitOrder.push_back(moveOrder.back());
}
void Battle::Defend()
{
	std::cout << "Unit " << moveOrder.back() << " defends\n";
	battleUnits[moveOrder.back()].isDef=true;
}
void Battle::AiMove(State& state)
{
	bool isOffensive = false;
	int dam;
	int maxDamSum=0;
    if(typeid(ranged)==typeid(*(battleUnits[moveOrder.back()].type.get())))
    {
    	auto tmpR = dynamic_cast<ranged*>(battleUnits[moveOrder.back()].type.get());
    	///1. Shooting
    	int bestToShot=-1;
    	auto attackType=static_cast<AttackTypes>(-1);
    	for(std::size_t i=0u;i<battleUnits.size();i++)
		{
			if(battleUnits[i].owner==battleUnits[moveOrder.back()].owner) continue;
            my_vector2i indTile = battleToTile(battleUnits[i].getPosition());
            bool isFull=false;
            if(rangeCost[indTile.x][indTile.y]>10*tmpR->range+2) continue; ///out of range
            if(rangeCost[indTile.x][indTile.y]<=5*tmpR->range+2)
			{
				dam=realDamage(battleUnits[moveOrder.back()].calculateDamage(battleUnits[i],rangedAttack)
					,(battleUnits[i].kount-1)*battleUnits[i].type->maxHP+battleUnits[i].hp);
				isFull=true;
			}
			else
			{
				dam=realDamage(battleUnits[moveOrder.back()].calculateDamage(battleUnits[i],halvedRangedAttack)
					,(battleUnits[i].kount-1)*battleUnits[i].type->maxHP+battleUnits[i].hp);
				isFull=false;
			}
			if(maxDamSum<dam)
			{
				maxDamSum=dam;
				bestToShot=i;
				if(isFull)
					attackType=rangedAttack;
				else
					attackType=halvedRangedAttack;
			}
		}
		if(bestToShot>=0) ///decides to shot
		{
			attack(state,moveOrder.back(),bestToShot,attackType);
            //battleUnits[moveOrder.back()].attack(state,battleUnits[bestToShot],attackType,true);
			return;
		}
		if(isOffensive)
		{
			///2. Move
		}
		else
		{
			///2. Wait/Defend
			if(isWaitPhase)
				Wait();
			else
				Defend();
			return;
		}
    }
    else ///melee unit
	{
		int unitIndex=-1;
		///1. Attack enemy unit in range
		my_vector2i moveToTile;
		bool isTargetRanged=false;
		for(std::size_t i=0u;i<battleUnits.size();i++)
		{
			if(battleUnits[i].owner==battleUnits[moveOrder.back()].owner) continue;
            my_vector2i indTile = battleToTile(battleUnits[i].getPosition());
            int mJ=-1; ///direction from which unit should be attacked
            for(int j=0;j<9;j++)
			{
				if(j<3 && indTile.x==0) continue;
				if(j>5 && indTile.x==14+6*isFort) continue;
				if(j%3 && indTile.y==0) continue;
				if(j%3==2 && indTile.y==14+6*isFort) continue;
				if(j==4) continue;
				if(moveCost[indTile.x-1+j%3][indTile.y-1+j/3]<=5*battleUnits[moveOrder.back()].mp)
				{
					if(mJ<0 || moveCost[indTile.x-1+j%3][indTile.y-1+j/3]<moveCost[indTile.x-1+mJ%3][indTile.y-1+mJ/3])
						mJ=j;
				}
			}
			if(mJ<0) continue; ///unit too far from moving unit
            bool isRanged=(typeid(*(battleUnits[i].type.get()))==typeid(ranged));
			indTile+=my_vector2i(mJ%3-1,mJ/3-1);
            dam=realDamage(battleUnits[moveOrder.back()].calculateDamage(battleUnits[i],rangedAttack)
					,(battleUnits[i].kount-1)*battleUnits[i].type->maxHP+battleUnits[i].hp);
			if((isTargetRanged==isRanged && dam>maxDamSum) || (!isTargetRanged && isRanged))
			{
				isTargetRanged=isRanged;
				unitIndex=i;
				moveToTile=indTile;
			}
		}
		if(unitIndex>-1)
		{
			MoveTo(moveToTile);
            battleUnits[moveOrder.back()].attack(state,battleUnits[unitIndex],meleeAttack,true);
			return;
		}
		if(isOffensive)
		{

		}
		else
		{
			///3.Wait/Defend
			if(isWaitPhase)
				Wait();
			else
				Defend();
			return;
		}
	}
}
bool Battle::MoveTo(const my_vector2i& tile)
{
	if(moveCost[tile.x][tile.y]<=5*battleUnits[moveOrder.back()].mp+2)
	{
		std::cout << "Unit " << moveOrder.back() << " moves to tile " << tile.x << "," << tile.y << "\n";
		///calculating path to tile
		my_vector2i fromTile = battleToTile(battleUnits[moveOrder.back()].getPosition());
		std::vector<my_vector2i> tmpTile;
		tmpTile.push_back(tile);
		while(tmpTile.back()!=fromTile)
		{
			int mJ=0;
			if(tmpTile.back().y==0)
			{
				mJ=3;
				if(tmpTile.back().x==0)
					mJ=5;
			}
            for(int j=1;j<9;j++)
			{
				if(tmpTile.back()+my_vector2i(j%3-1,j/3-1)==fromTile)
				{
					mJ=j;
					break;
				}
				if(j==4) continue;
				if(j<3 && tmpTile.back().y==0) continue;
				if(j>5 && tmpTile.back().y==14+6*isFort) continue;
				if(j%3==0 && tmpTile.back().x==0) continue;
				if(j%3==2 && tmpTile.back().x==14+6*isFort) continue;
				if(moveCost[tile.x-1+j%3][tile.y-1+j/3]<moveCost[tile.x-1+mJ%3][tile.y-1+mJ/3])
					mJ=j;
			}
			tmpTile.push_back(tmpTile.back()+my_vector2i(mJ%3-1,mJ/3-1));
			std::cout << tmpTile.back().x << "," << tmpTile.back().y << " ";
		}
		std::cout << "\n";
		battleUnits[moveOrder.back()].setPosition((my_vector2f)battleTiles[tile.x][tile.y].getPosition());
		battleCountText[moveOrder.back()].setPosition((my_vector2f)battleTiles[tile.x][tile.y].getPosition()+bctPos);
		return EXIT_SUCCESS;
	}
	return EXIT_FAILURE;
}
bool Battle::attack(State& state, int attackerID, int defenderID, AttackTypes attackType)
{
    switch(attackType)
    {
	case meleeAttack:
		std::cout << "Unit " << attackerID << " meleely attacks unit" << defenderID <<"\n";
		battleUnits[attackerID].attack(state,battleUnits[defenderID],meleeAttack,true);
		battleCountText[attackerID].setString(my_to_string(battleUnits[attackerID].kount));
		if(!battleUnits[defenderID].findAbility(UnlimitedCounterstrike).has_value())
			battleUnits[defenderID].counterstroke=true;
		if(removeIfDestroyed(attackerID))
			return EXIT_SUCCESS;
		break;
	case rangedAttack:
		if(typeid(*battleUnits[attackerID].type.get())!=typeid(ranged)) return EXIT_FAILURE;
		{
			//auto tmpR = dynamic_cast<ranged*>(battleUnits[attackerID].type);
			std::cout << "Unit " << attackerID << " full-rangedly attacks unit" << defenderID <<"\n";
			battleUnits[attackerID].attack(state, battleUnits[defenderID],rangedAttack,true);
			if(battleUnits[attackerID].findAbility(DoubleAttack).has_value())
			{
				std::size_t tmpLength = battleUnits.size();
				if(removeIfDestroyed(defenderID))
					return EXIT_SUCCESS;
				if(tmpLength==battleUnits.size()) ///unit wasn't destroyed
					battleUnits[attackerID].attack(state, battleUnits[defenderID],rangedAttack,true);
			}
		}
		break;
	case halvedRangedAttack:
		if(typeid(*battleUnits[attackerID].type.get())!=typeid(ranged)) return EXIT_FAILURE;
		{
			//auto tmpR = dynamic_cast<ranged*>(battleUnits[attackerID].type);
			std::cout << "Unit " << attackerID << " half-rangedly attacks unit" << defenderID <<"\n";
			battleUnits[attackerID].attack(state, battleUnits[defenderID],halvedRangedAttack,true);
			if(battleUnits[attackerID].findAbility(DoubleAttack).has_value())
			{
				std::size_t tmpLength = battleUnits.size();
				if(removeIfDestroyed(defenderID))
					return EXIT_SUCCESS;
				if(tmpLength==battleUnits.size()) ///unit wasn't destroyed
					battleUnits[attackerID].attack(state, battleUnits[defenderID],halvedRangedAttack,true);
			}
		}
    }
    battleCountText[defenderID].setString(my_to_string(battleUnits[defenderID].kount));
    removeIfDestroyed(defenderID);
	return EXIT_SUCCESS;
}

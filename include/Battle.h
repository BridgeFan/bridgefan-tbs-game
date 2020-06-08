#pragma once
#ifndef BATTLE_H
#define BATTLE_H
#include <SFML/Graphics.hpp>
#include "basic/constants.hpp"
#include "ui.hpp"
extern sf::RenderWindow appWindow;
extern UI gameUI;
class Map; extern Map mapa;
class Unit;
class Data;
class Player;
class State;

class Battle
{
    public:

        Battle(const Data& data);
        bool beginBattle(State& state, Unit& attacker, Unit& defender, uint8_t dir=0,bool isFort=false);
        ///dir (from which the attacker attacks)
        ///012
        ///3X5
        ///678
    private:
        const Data& data;
        sf::Sprite battleTiles[21][21]; ///adjusted for bigger battle
        sf::Sprite battleGrid;
        sf::CircleShape activeUnitCircle;
        sf::Sprite uiBack, miniature;
        sf::Text nameText, countText;
        sf::Sprite addSprite[MAX_NUMBER_OF_PARAMETERS];
        sf::Text addText[MAX_NUMBER_OF_PARAMETERS];
        sf::Text abText[MAX_NUMBER_OF_PARAMETERS];
        sf::Text damageText, counterDamText;
        sf::CircleShape movableTo[21][21];
        sf::View battleView;
        bfUI::Button escButton;
        my_vector2f bctPos;
        bool isForcedMelee;
        bool isFort;
        int roundNr;
        std::vector<Unit> battleUnits;
        std::vector<sf::Text> battleCountText;
        std::vector<int> moveOrder; ///reversed
        std::vector<int> waitOrder;
        bool isWaitPhase;
        void nextUnit();
        uint8_t type[21][21];
        short moveCost[21][21];
        short rangeCost[21][21]; ///for ranged units
        short movingUnitIndex(bool isWaiting);
        void draw();
        bool battleLoop(State& state);
        void nextTurn();
        short attackerOwner;
        void calculate(bool changeMelee=false);
        void unitInfo(int index);
        void endBattle();
        void beginWaitPhase();
        bool removeIfDestroyed(int index);
        void Wait();
        void Defend();
        void AiMove(State& state);
        bool isBattleFinished();
        int realDamage(std::pair<int,int> dam, int maxHP)
        {
                ///returns doubled damage
                return std::max(dam.first+dam.second,2*maxHP);
        }
        bool MoveTo(const my_vector2i& tile); ///was move not successfull
        bool attack(State& state, int attackerID, int defenderID, AttackTypes attackType);
        inline my_vector2i battleToTile(const my_vector2f& pos)
        {
                float scale = appWindow.getSize().y/1080.f;
                return my_vector2i((pos-(my_vector2f)battleTiles[0][0].getPosition())/(scale*(64.f-16.f*isFort)));
        }
        my_vector2f tileToBattle(const my_vector2i& tile)
        {
                float scale = appWindow.getSize().y/1080.f;
                if(tile.x>=0 && tile.y>=0 && tile.x<15+6*isFort && tile.y<15+6*isFort)
                        return battleTiles[tile.x][tile.y].getPosition();
                else
                        return (my_vector2f)battleTiles[0][0].getPosition()+(my_vector2f)tile*(scale*(64.f-16.f*isFort));
        }
        inline my_vector2f tileToBattle(int x, int y)
        {
                return tileToBattle({x,y});
        }
        /*inline my_vector2i unitToTile(const my_vector2f& pos)
        {
                float scale = appWindow.getSize().y/1080.f;
                return my_vector2i((pos-battleTiles[0][0].getPosition())/(scale*(64.f-16.f*isFort)));
        }*/
};

#endif // BATTLE_H

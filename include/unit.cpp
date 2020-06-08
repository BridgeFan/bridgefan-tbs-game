#include "unit.hpp"
#include "player.hpp"
#include "ui.hpp"
#include "basic/megaTexture.hpp"
#include "path.hpp"
#include "map.hpp"
#include "Data.hpp"
#include <SFML/Graphics.hpp>
#include <typeinfo>
#include <cmath>
#include <iostream>
float Unit::unitScale=1.f;
State* UnitKind::state=nullptr;
const Data* Unit::data=nullptr;

UnitKind::UnitKind(string n, short h,short m,short s, short a, short d)
{
	maxHP=h;
	maxMP=m;
	str=s;
	name=n;
	att=a;
	def=d;
}
Unit::Unit(std::shared_ptr<UnitKind> t): type(t),  mp(t->maxMP), owner(0), hp(t->maxHP), kount(1), numOfArrows(0)
{
	counterstroke=false;
	isDef=false;
	if(t!=nullptr)
	{
		//std::copy(t->abilities.aList.begin(),t->abilities.aList.end(),std::back_inserter(abilities));
		actualizeSpriteTextures();
		icon.setScale(unitScale,unitScale);
	}
}
std::optional<Ability> Unit::findAbility(AbilityIndex id)
{
	auto ret = type->abilities.findAbility(id);
	if(ret.has_value())
		return ret;
	return ownAbilities.findAbility(id);
}
std::vector<std::string> Unit::getAbilityNames()
{
	std::vector<std::string> ret = type->abilities.getNames();
	std::vector<std::string> ret2 = ownAbilities.getNames();
	ret.insert( ret.end(), std::make_move_iterator(ret2.begin()), std::make_move_iterator(ret2.end()));
	return ret;
}
/*void UnitKind::setTexture(const sf::Texture& tex, const sf::Texture& unitOwnerTex)
{
    unitTexture=tex;
    unitOwnerTexture=unitOwnerTex;
}*/
void UnitKind::setOwnerTexture(MegaTexture& ownerMega, int uomInd)
{
    this->ownerMega=&ownerMega;
    unitOwnerTextureInd=uomInd;
    //mega.assignTexture(unitTexture,megaInd);
    //unitOwnerMega.assignTexture(unitOwnerTexture,uomInd);
}
void Unit::newTurn()
{
    hp=std::min(hp+type->maxHP/10,(int)type->maxHP);
    mp=type->maxMP;
}
void Unit::setPosition(float x, float y, uint8_t isAnimation, const State& state)
{
    if(isAnimation==1)
	{
        float tilePerSec = 10.f;
		sf::Clock clock;
		float time;
        for(int i=MAX_PATH_LENGTH-2;i>=-1;i--)
		{
			if(i>-1 && !path.getVisible(i)) continue;
			my_vector2i actPos,prevPos;
			if(i!=-1)
				actPos=mapToTile(path.getIndPosition(i));
			else
				actPos=mapToTile(my_vector2f(x,y));
			prevPos=mapToTile(path.getIndPosition(i+1));
			int cost=mapa(actPos).tileValue(mapa(prevPos).level,
						tileDir(path.getIndPosition(i),path.getIndPosition(i+1))%2==0,
						isDiagCrosRiv(prevPos,actPos),
						mapa(prevPos).getRoadStage());
			if(cost>mp)
			{
				icon.setPosition(tileToMap(prevPos));
				unitOwnerSprite.setPosition(tileToMap(prevPos));
				mapa.actualizeVisibility(owner, state);
				return;
			}
			mp-=cost;
			gameUI.unitInfo(*(type->state));
			path.setUnvisible(i+1);
			time=.0f;
			clock.restart();
			mapa.actualizeVisibility(owner, state);
			while(time<1.f/tilePerSec)
			{
				//icon.setPosition(path.getIndPosition(i+1)*(1.f-time*tilePerSec)+path.getIndPosition(i)*(time*tilePerSec));
				//icon.setPosition(tileToMap(prevPos)*(1.f-time*tilePerSec)+tileToMap(actPos)*(time*tilePerSec));
				//unitOwnerSprite.setPosition(icon.getPosition());
				time+=clock.restart().asSeconds();
			}
			icon.setPosition(tileToMap(actPos));
            unitOwnerSprite.setPosition(icon.getPosition());
		}
	}
	icon.setPosition(x,y);
	unitOwnerSprite.setPosition(x,y);
	mapa.actualizeVisibility(owner, state);
}
void Unit::setPosition(const my_vector2f& pos, bool isAnimation, const State& state)
{
    setPosition(pos.x,pos.y,isAnimation, state);
}
void Unit::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    if(type==nullptr) return;
    target.draw(icon, states);
    target.draw(unitOwnerSprite, states);
}
void Unit::battleDraw(sf::RenderTarget& target) const
{
    if(type==nullptr) return;
    target.draw(icon);
}
void Unit::setOwner(unsigned char num)
{
    owner=num;
    unitOwnerSprite.setColor(players[num].playerColor);
}

bool Unit::FillQuiver(const State& state)
{
    if(mp<20) return EXIT_FAILURE;
    short& capac=dynamic_cast<ranged*>(type.get())->capacity;
    if(numOfArrows==capac) return EXIT_FAILURE;
    numOfArrows=capac;
    mp-=20;
    gameUI.unitInfo(state);
    return EXIT_SUCCESS;
}
void Unit::TakeDamage(int amount)
{
    kount-=amount/type->maxHP;
    hp-=amount%type->maxHP;
    if(hp<0) {kount--; hp+=type->maxHP;}
    if(kount<=0) DestroyUnit();
}
void Unit::actualizeSpriteTextures()
{
    type->textureUP.get()->assignTexture(icon,0);
    //type->mega->assignTexture(icon,type->unitTextureInd);
    type->ownerMega->assignTexture(unitOwnerSprite,type->unitOwnerTextureInd);
    unitOwnerSprite.setScale(.75f,.75f);
}
short Unit::getDefence()
{
    return type->def*(1+isDef);
}
bool Unit::attack(State& state, Unit& target, unsigned char type, bool isInternal)
{
    if(type==AttackTypes::meleeAttack && (mp>=15 || isInternal))
    {
        if(typeid(*(this->type.get()))==typeid(officer)) //officers have different mechanic
        {
            float thisStr=(((float)rand()) / (float) RAND_MAX + .7f)*.3f; //random number between 0.7 and 1.3
            float otherStr=(((float)rand()) / (float) RAND_MAX + .7f)*.3f; //random number between 0.7 and 1.3
            thisStr*=this->getOfficerAiValue();
            if(typeid(*target.type.get())==typeid(officer))
                otherStr*=target.getOfficerAiValue();
            else
                otherStr*=target.type->getAiValue();
            //proportional to square of strength
            float result=(thisStr/otherStr)*(thisStr/otherStr);
            if(result>=1.f) //your officer wins
                {
                    if((players[owner].isComp && players[target.owner].isComp) || gameUI.makeSure(state,"UNIT_BATTLE_WIN_1"+my_to_string(int(100.f/result))+"UNIT_BATTLE_WIN_2"))
                    {
                        killPartRandomly(1.f/result);
                        target.TakeDamage(100.f);
                        return false;

                    }
                    else return true;
                }
            else //your officer dies
                if(gameUI.makeSure(state, Lang("UNIT_BATTLE_LOSE_1")+my_to_string(int(100.f*result))+Lang("UNIT_BATTLE_LOSE_2")))
                {
                    target.killPartRandomly(result);
                    TakeDamage(100.f);
                    return false;
                }
                else return true;
        }
        else ///if other unit than officer attacks (possible only in a battle)
        {
            /*if(!isInternal)
                mp-=15;*/
			std::pair<int,int> damage;
			if(target.type!=nullptr && !target.counterstroke && target.findAbility(FirstStrike).has_value() && !findAbility(FirstStrike).has_value())
			{
				damage = target.calculateDamage(*this,meleeAttack);
				TakeDamage(myRand(damage.first,damage.second));
				damage = calculateDamage(target,meleeAttack);
				target.TakeDamage(myRand(damage.first,damage.second));
			}
			else
			{
				damage = calculateDamage(target,meleeAttack);
				target.TakeDamage(myRand(damage.first,damage.second));
				if(target.type!=nullptr && !target.counterstroke)
				{
					damage = target.calculateDamage(*this,meleeAttack);
					TakeDamage(myRand(damage.first,damage.second));
				}
			}
			if(findAbility(DoubleAttack).has_value() && target.kount>0)
			{
				damage = calculateDamage(target,meleeAttack);
                target.TakeDamage(myRand(damage.first,damage.second));
			}
        }
        return false;
    }
    else if(typeid(*(this->type.get()))==typeid(ranged) && (type==AttackTypes::rangedAttack || type==AttackTypes::halvedRangedAttack)) ///used only in a battle
    {
        //ranged* rType=dynamic_cast<ranged*>(this->type);
        numOfArrows--;
		std::pair<int,int> damage = calculateDamage(target,type);
        //if(type==AttackTypes::halvedRangedAttack)
		target.TakeDamage(myRand(damage.first,damage.second));
        //else
			//target.TakeDamage(myRand(damage.first,damage.second));
    }
    return false;
}
int Unit::getOfficerAiValue() const
{
    return std::accumulate(commandedUnits.begin(),commandedUnits.end(),0,[](int acc, const std::pair<int,int>& s){return acc+=s.second*data->getUnitType(s.first)->getAiValue();});
}
std::pair<int,int> Unit::calculateDamage(Unit& target, unsigned char type, int killedUnits)
{
	if(type==AttackTypes::meleeAttack)
	{
		int dam=(kount-killedUnits)*this->type->str*std::max(std::min(std::sqrt(static_cast<float>(this->type->att)/target.getDefence()),4.f),.25f);
		return std::make_pair(std::floor(dam*0.75f),std::ceil(dam*1.25f));
	}
	else if(typeid(*(this->type.get()))==typeid(ranged) && (type==AttackTypes::rangedAttack || type==AttackTypes::halvedRangedAttack))
	{
		ranged* rType=dynamic_cast<ranged*>(this->type.get());
		int dam = (kount-killedUnits)*rType->rangedStr;
		if(type==AttackTypes::halvedRangedAttack)
			return std::make_pair(std::floor(dam*.375f),std::ceil(dam*.625f));
		else
			return std::make_pair(std::floor(dam*.75),std::ceil(dam*1.25));
	}
	else return std::make_pair(0,0);
}
void Unit::killPartRandomly(float pr)
{
    for(auto it=commandedUnits.begin();it<commandedUnits.end();++it)
    {
        it->second=std::max(1.,round((1.f-pr)*it->second));
    }
}

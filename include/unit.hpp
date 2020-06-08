#pragma once
#ifndef UNIT_H_INCLUDED
#define UNIT_H_INCLUDED
#include <SFML/Graphics.hpp>
#include "basic/constants.hpp"
#include "ability.hpp"
#include <numeric>
class Player;
class unit;
class UI;
class MegaTexture;
class AnimationInfo;
class Path;
class Map;
class Data;
extern Map mapa;
extern Path path;
extern std::vector<Player> players;
extern UI gameUI;
extern bool isDiagCrosRiv(const my_vector2i&,const my_vector2i&);

class UnitKind
{
public:
    static State* state;
    short unitTextureInd, unitOwnerTextureInd;
    std::unique_ptr<MegaTexture> textureUP;
    MegaTexture* ownerMega;
    std::vector<AnimationInfo> animationInfo;
    virtual ~UnitKind() {}
    short maxHP,maxMP, str, att, def;
    std::string name;
    virtual int getAiValue()=0;
    UnitKind(std::string n="", short h=1,short m=100,short s=0, short a=0, short d=0);
    void setOwnerTexture(MegaTexture& ownerMega,int uomInd);
    AbilityList abilities;
    friend class Unit;
};
class ranged: public UnitKind
{
public:
    short rangedStr;
    short range;
    short capacity;
    virtual int getAiValue() {return maxHP*(rangedStr*range*.1f+str*.4f);}
    ranged(std::string n="", short h=1,short m=100,short s=0, short rs=0, short r=5, short c=5): UnitKind(n,h,m,s), rangedStr(rs),range(r),capacity(c) {}

};
class melee: public UnitKind
{
public:
    melee(std::string n="", short h=1,short m=100,short s=0): UnitKind(n,h,m,s) {}
    virtual int getAiValue() {return maxHP*str;}
};
class officer: public UnitKind
{
public:
    int maxSize, totalNrCommandedUnits;
    officer(std::string n="", short m=100): UnitKind(n,1,m,0) {}
    virtual int getAiValue() {return 0;};
};
class settler: public UnitKind
{
public:
    settler(std::string n="", short h=1,short m=100): UnitKind(n,h,m,0) {}
    virtual int getAiValue() {return 0;}
};
class Unit: public sf::Drawable
{
private:
    sf::Sprite icon, unitOwnerSprite;
    static float unitScale;
    static const Data* data;
    my_vector2i pos;
public:
    static void setData(const Data& dat) {data=&dat;}
    std::shared_ptr<UnitKind> type;
    AbilityList ownAbilities; ///only abilities not included by his parent
    std::vector<std::pair<int,int> > commandedUnits; //type of unit, number of pieces, used only if unit is an officer
    std::optional<Ability> findAbility(AbilityIndex id);
    std::vector<std::string> getAbilityNames();
    short mp;
    uint8_t owner;
    ///used only in battle:
    short hp;
    int kount;
    bool counterstroke;
    bool isDef;
    short numOfArrows;
    ///methods
    void setOwner(unsigned char num=255);
    Unit(std::shared_ptr<UnitKind> t=std::shared_ptr<UnitKind>());
    void newTurn();
    void setPosition(float x, float y, uint8_t isAnimation=0, const State& state=State());
    void setPosition(const my_vector2f& pos, bool isAnimation=false, const State& state=State());
    void setScale(float x, float y) {icon.setScale(x,y);unitOwnerSprite.setScale(x,y);};
    void setScale(const my_vector2f &factors) {setScale(factors.x,factors.y);}
    void setColor(const sf::Color& color) {icon.setColor(color);}
    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
    void battleDraw(sf::RenderTarget& target) const;
    my_vector2f getPosition() const {return icon.getPosition();}
    void actualizeSpriteTextures();
    bool FillQuiver(const State& state); //0 if success, 1 if fail
    void TakeDamage(int amount);
    short getDefence();
    bool attack(State& state, Unit& target, unsigned char type, bool isInternal=false); //is battle beginning
    void DestroyUnit() {type=nullptr; hp=0; mp=0; kount=0; owner=255; numOfArrows=0; icon.setTexture(sf::Texture());unitOwnerSprite.setTexture(sf::Texture());}
    int getOfficerAiValue() const;
    void killPartRandomly(float pr);
    std::pair<int,int> calculateDamage(Unit& target, unsigned char type, int killedUnits=0); ///min, max damage (killed units is used only for information in battle
};
#endif // UNIT_H_INCLUDED

#pragma once
#ifndef CONSTANTS_H_INCLUDED
#define CONSTANTS_H_INCLUDED
#include <string>
#include <vector>
#include <sstream>
#include <cmath>
#include <bitset>
#include <memory>
#include <SFML/Graphics.hpp>
#include <SelbaWard/Line.hpp>
#include <SelbaWard/NinePatch.hpp>
#include <SelbaWard/ProgressBar.hpp>
#include "my_vector.hpp"
#ifndef INT_MAX
#define INT_MAX 2147483647
#endif
#ifndef SHRT_MAX
#define SHRT_MAX 32767
#endif
#ifndef INT_MIN
#define INT_MIN -INT_MAX-1
#endif
#ifndef __USE_MINGW_ANSI_STDIO
#define __USE_MINGW_ANSI_STDIO 0
#endif
#define AUTOSAVE_LOCATION "saves/save.sav"
constexpr short RESOURCE_TYPES_NUMBER=6, BUILDING_STAGES_NUMBER=10, MAX_NUMBER_OF_PARAMETERS=12, MAX_LOADING_PTS=75, MAX_NUMBER_OF_PLAYERS=64;
constexpr uint8_t MAKE_SURE_STATE=1, INSERT_TEXT_STATE=2, NO_UI=129, CITY_VIEW=130;
constexpr int tileSize=128;
constexpr uint32_t BFUISIG_KILL_DRAW_THREAD=1u;
//constexpr string AUTOSAVE_LOCATION="saves/save.txt";
struct State
{
    short pressedUnitIndex;
    short pressedImproIndex;
    short pressedImproCityIndex;
    short targetUnitIndex;
    short actualPlayer, actualTurn;
    uint8_t drawState;
    sf::Event event;
    sf::View view;
    sf::View uiView;
    bool isIso;
    bool drawAll;
    std::string languageName;
    bool isFullscreen;
    uint32_t signal;
    State(): pressedUnitIndex(-1), pressedImproIndex(-1), targetUnitIndex(-1), actualPlayer(0), actualTurn(0), drawState(0), event(), view(), uiView(), isIso(false), drawAll(false), languageName(), isFullscreen(false), signal(0){}
};
enum uiMegaID
{
    mpID=0, hpID=1, gold=2, wood=3, stone=4, iron=5, horses=6, jewels=7,    circleBack=15,
    str=16, ranStr=17, rang=18, capac=19, improveStar=20, destroyID=21, recruitID=22, popIconId=23,
    batExit=24, attID=25, defID=26, house=27, food=28,
    upID=32, clickedUpID=33, downID=34, clickedDownID=35, checkBoxOff=36, checkBoxOn=37
};
enum mainMegaIDs
{
    sea=0, bay=1, grass=2, sand=3,
    swamp=4, snow=5, rocky=6, forestID=7, taiga=8,
    oHill=25, o90Hill=26, o90180Hill=27,
    o180Hill=28, allHill=29,
    dHill=32, d90Hills=33, d180Hills=34, d90180Hills=35,
    allDHills=36, stoneBack=38, ironBack=39,
    horsesBack=40, jewelsBack=41, goldBack=42, //43-47 additional resources

    //48-49 unupgraded
    roadBegin=50,
    //50 - roadBegin
    locked=66, cityTex=67, wall=72, gate=73,
    riverBegin=80,
    dPart=100, d90Parts=101, d180Parts=102, d3Parts=103, d4Parts=104

};
enum AttackTypes{meleeAttack=0, rangedAttack=1, halvedRangedAttack=2};

int myRand(int minV, int maxV); ///used for battle randomising
template <typename T>
inline int sgn(T val) {
    return (T(0) < val) - (val < T(0));
}
template <typename T>
std::string my_to_string(const T& value) {
    std::stringstream ss;
    ss << value;
    return ss.str();
}
template <typename T>
inline std::string MToStr(const T& value) {return my_to_string(value);}

my_vector2i mapToTile(const my_vector2f& v, bool isIso=false);
//inline my_vector2i mapToTile(const my_vector2i& v, bool isIso=false) {return mapToTile((my_vector2f)v, isIso);}
my_vector2f tileToMap(int x, int y, bool isIso=false);
inline my_vector2f tileToMap(const my_vector2i& tile, bool isIso=false)
{
    return tileToMap(tile.x,tile.y, isIso);
}
inline int sqrDist(const my_vector2i& u, const my_vector2i& v)
{
    return (u.x-v.x)*(u.x-v.x)+(u.y-v.y)*(u.y-v.y);
}
inline int sqrTileDist(const my_vector2f& u, const my_vector2f& v)
{
    my_vector2i tU = mapToTile(u);
    my_vector2i tV = mapToTile(v);
    return sqrDist(tU,tV);
}inline uint8_t tileDir(const my_vector2f& u, const my_vector2f& v)
{
    my_vector2i tU = mapToTile(u);
    my_vector2i tV = mapToTile(v);
    return 4+3*sgn(tU.y-tV.y)+sgn(tU.x-tV.x);
}
///-1 if more than one
template<typename T>
std::optional<T> MaxEl(std::vector<T> v)
{
    if(v.empty()) return std::nullopt;
    int maxInd=0;
    bool isMult=false;
    for(std::size_t i=1u;i<v.size();i++)
    {
        if(v[i]>v[maxInd])
        {
            maxInd=i;
            isMult=false;
        }
        else if(v[i]==v[maxInd])
            isMult=true;
    }
    if(isMult)
    return std::nullopt;
    return v[maxInd];
}
inline void RemoveSpaces(std::string& a)
{
    std::replace(a.begin(),a.end(),' ','_');
}
inline void RemoveSpaces(std::wstring& a)
{
    std::replace(a.begin(),a.end(),' ','_');
}
inline void RemoveSpaces(sf::String& a)
{
    std::replace(a.begin(),a.end(),' ','_');
}
inline void AddSpaces(std::string& a)
{
    std::replace(a.begin(),a.end(),'_',' ');
}
inline void AddSpaces(std::wstring& a)
{
    std::replace(a.begin(),a.end(),'_',' ');
}
inline void AddSpaces(sf::String& a)
{
    std::replace(a.begin(),a.end(),'_',' ');
}
inline bool inBounds(my_vector2f position, sf::FloatRect fr)
{
    return position.x>=fr.left && position.x<=fr.left+fr.width && position.y>=fr.top && position.y<=fr.top+fr.height;
}
void saveText(std::ostream& out, const sf::String& text);
sf::String loadText(std::istream& in, uint16_t size);
inline void write(std::ostream& out, uint16_t a) {
    ///function for writing binary single short int
    out << char(a>>8) << char(a%0x100);
}
inline uint16_t load(std::istream& in) {
    ///function for writing binary single short int
    unsigned char a,b;
    a=in.get();
    b=in.get();
    return (uint16_t(a)<<8)+uint16_t(b);
}
sf::String Lang(const std::string& val);
#endif // CONSTANTS_H_INCLUDED


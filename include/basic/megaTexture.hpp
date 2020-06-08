#ifndef MEGATEXTURE_H_INCLUDED
#define MEGATEXTURE_H_INCLUDED
#include <SFML/Graphics.hpp>
#pragma once
#include "basic/constants.hpp"
using std::string;

namespace bfUI{class Button;}
class MegaTexture
{
    friend class bfUI::Button;
private:
    int tileSizeX, tileSizeY;
    //sf::Texture mega[256];
    sf::Texture mega;
public:
    bool init(string fileName, int tSize=128) {return init(fileName,tSize,tSize);}
    bool init(string fileName, int xSize, int ySize);
    void assignTexture(sf::Sprite& s, int index) const;
    void assignTexture(sf::RectangleShape& rect, int index) const;
    void assignTexture(bfUI::Button& but, int index, int activeIndex=-1) const;
    const int& getTileSize() const{return tileSizeX;}
    const int& getTileSizeY() const{return tileSizeY;}
    sf::Texture& getTexture() {return mega;}
    const sf::Texture& getTexture() const {return mega;}
    int getIndex(const sf::IntRect& s) const;
};
struct AnimationInfo
{
    int firstIndex;
    int lastIndex;
    float frameDuration; //in seconds
    bool isLooped;
    std::string name;
};

#endif // MEGATEXTURE_H_INCLUDED

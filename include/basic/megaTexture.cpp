#include "megaTexture.hpp"
#include "bfUI.hpp"

bool MegaTexture::init(string fileName, int xSize, int ySize)
{
    tileSizeX=xSize;
    tileSizeY=ySize;
    if(!mega.loadFromFile(fileName))
        return EXIT_FAILURE;
    //if(mega.getSize().x==mega.getSize().y)
    mega.generateMipmap();
    return EXIT_SUCCESS;
}
void MegaTexture::assignTexture(sf::Sprite& s, int index) const
{
    s.setTexture(mega, true);
    s.setTextureRect(sf::IntRect((index%16)*tileSizeX,(index/16)*tileSizeY,tileSizeX,tileSizeY));
}
/*void MegaTexture::assignTexture(sw::ElasticSprite& s, int index) const
{
    s.setTexture(mega, true);
    s.setTextureRect(sf::FloatRect((index%16)*tileSizeX,(index/16)*tileSizeY,tileSizeX,tileSizeY));
}*/
void MegaTexture::assignTexture(sf::RectangleShape& rect, int index) const
{
    rect.setTexture(&mega, true);
    rect.setTextureRect(sf::IntRect((index%16)*tileSizeX,(index/16)*tileSizeY,tileSizeX,tileSizeY));
}

void MegaTexture::assignTexture(bfUI::Button& but, int index, int activeIndex) const
{
    if(activeIndex<0) activeIndex=index;
    assignTexture(but.icon, index);
    assignTexture(but.activeIcon, activeIndex);
}
int MegaTexture::getIndex(const sf::IntRect& s) const
{
    return 16*s.top/tileSizeY+s.left/tileSizeX;
}

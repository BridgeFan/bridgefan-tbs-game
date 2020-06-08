#include "path.hpp"

void Path::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    for(int i=0;i<MAX_PATH_LENGTH;i++)
        if(isVisible[i])
            target.draw(spr[i], states);
}
void Path::init(float scale)
{
    for(int i=0;i<MAX_PATH_LENGTH;i++)
    {
        //spr[i].setTexture(movePath);
        isVisible[i]=false;
        spr[i].setRadius(16.f);
        spr[i].setFillColor(sf::Color::Red);
        spr[i].setScale(scale,scale);
        //spr[i].setPosition(0.f,0.f);
    }
}
void Path::clear()
{
    for(int i=0;i<MAX_PATH_LENGTH;i++)
    {
        isVisible[i]=false;
        spr[i].setPosition(0.f,0.f);
    }
}

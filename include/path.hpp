#pragma once
#ifndef PATH_H_INCLUDED
#define PATH_H_INCLUDED
#include <SFML/Graphics.hpp>
#include "basic/my_vector.hpp"

#define MAX_PATH_LENGTH 100
class Path: public sf::Drawable
{
private:
    bool isVisible[MAX_PATH_LENGTH];
    sf::CircleShape spr[MAX_PATH_LENGTH];
public:
    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
    void init(float);
    void clear();
    void setVisible(int index){isVisible[index]=true;}
    void setUnvisible(int index){isVisible[index]=false;}
    bool getVisible(int index){return isVisible[index];}
    void setIndPosition(int index, float x, float y) {spr[index].setPosition(x+64.f-spr[index].getRadius(),y+64.f-spr[index].getRadius());}
    void setIndPosition(int index, my_vector2f& pos) {setIndPosition(index,pos.x,pos.y);}
    void setIndPosition(int index, my_vector2f&& pos) {setIndPosition(index,pos.x,pos.y);}
    my_vector2f getIndPosition(int index) {return my_vector2f(spr[index].getPosition().x-64.f+spr[index].getRadius(),spr[index].getPosition().y-64.f+spr[index].getRadius());}
};

#endif // PATH_H_INCLUDED

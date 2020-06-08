#pragma once
#ifndef LOADINGSCREEN_H
#define LOADINGSCREEN_H
#include <SFML/Graphics.hpp>
#include "basic/constants.hpp"
#include "bfUI.hpp"
extern sf::RenderWindow appWindow;
class LoadingScreen: public sf::Drawable
{
    public:
        sf::Sprite loadingBackground;
        sw::ProgressBar loadingBar;
        LoadingScreen();
        bool init();
        bool isBarShown;
        virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const
            {target.draw(loadingBackground,states); if(isBarShown) target.draw(loadingBar);}
        void DisplayOnly() const {appWindow.clear();appWindow.draw(*this);appWindow.display();}
        void addPoints(int pts) {loadingBar.setRatio(loadingBar.getRatio()+(float)pts/MAX_LOADING_PTS);DisplayOnly();}
        void actualizeSize();
        virtual ~LoadingScreen();

    protected:

    private:
        sf::Texture loadingTexture, barTexture;
};

#endif // LOADINGSCREEN_H

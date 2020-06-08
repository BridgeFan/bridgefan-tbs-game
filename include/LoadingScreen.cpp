#include "LoadingScreen.hpp"
#include "bfUI.hpp"

LoadingScreen::LoadingScreen()
{

    if(!loadingTexture.loadFromFile("data/loading.png")) throw std::exception();
    loadingBackground.setTexture(loadingTexture);
    loadingBar.setFrameColor(sf::Color::Black);
    loadingBar.setFrameThickness(appWindow.getSize().y*5.f/1080.f);
    loadingBar.setBackgroundColor(sf::Color::White);
    loadingBar.setColor(sf::Color::Green);
    loadingBar.setShowBackgroundAndFrame(true);
    loadingBar.setSize({appWindow.getSize().x*.9f,appWindow.getSize().y*.05f});
    loadingBar.setPosition(appWindow.getSize().x*.05f,.8f*appWindow.getSize().y);
    //ctor
}
bool LoadingScreen::init()
{
    actualizeSize();
    return EXIT_SUCCESS;
}
void LoadingScreen::actualizeSize()
{
	loadingBackground.setScale(appWindow.getSize().x/1920.f,appWindow.getSize().y/1080.f);
}
LoadingScreen::~LoadingScreen()
{
    //dtor
}

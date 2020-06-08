//
// Created by kamil on 28.01.2020.
//

#include "Settings.hpp"
#include <fstream>

/*bool Settings::load(std::string fileName)
{
    std::ifstream settingsFile;
    settingsFile.open("settings.json");
    //sleep(seconds(1.f));
    if(!settingsFile.good())
    {
        winSize.x=800;
        winSize.y=600;
        isFullscreen=false;
        return 1;
    }
    Json::Value value;
    settingsFile >> value;
    settingsFile.close();
    isFullscreen=value.get("is_fullscreen",false).asBool();
    winSize.x=value.get("screen_x",800).asInt();
    winSize.y=value.get("screen_y",600).asInt();
    languageName=value.get("language","english").asString();
    //if(loadLanguage("data/lang/"+languageName+".json",mainMenu,false));
    //return -1;
    //return 0;
}
bool Settings::save(std::string fileName)
{
    Json::Value saveValue;
    saveValue["is_fullscreen"]=isFullscreen;
    saveValue["screen_x"]=getSize().x;
    saveValue["screen_y"]=getSize().y;
    saveValue["language"]=languageName;
    std::ofstream settingsFile;
    settingsFile.open("settings.json");
    if(!settingsFile.good())
        return EXIT_FAILURE;
    settingsFile << saveValue << '\n';
    settingsFile.close();
    return EXIT_SUCCESS;
}*/
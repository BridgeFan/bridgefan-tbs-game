#include "Data.hpp"
#include "LoadingScreen.hpp"
#include "unit.hpp"
#ifdef __linux__
#include <jsoncpp/json/json.h>
#include <jsoncpp/json/reader.h>
#include <jsoncpp/json/writer.h>
#include <jsoncpp/json/value.h>
#else
#include "json/json.h"
#endif
#include <fstream>
#include <iostream>

Data::Data(State& state)
{
    if(init(state))
        throw std::bad_exception();


    //ctor
}

Data::~Data()
{
    //dtor
}

bool Data::init(State& state)
{
    if(!barTexture.loadFromFile("data/bfui-bar.png")) return EXIT_FAILURE;
    if(!sliderTexture.loadFromFile("data/bfui-slider.png")) return EXIT_FAILURE;
    if (!font.loadFromFile("font.ttf"))
    {logStr+="[ERROR]Font loading error!\n"; return EXIT_FAILURE;}
    //loadingScreen.addPoints(1)
    logStr+="Font loaded successfully\n";
    std::cout << "Font loaded successfully\n";
    nullTexture.create(1,1);
    //initializing megaTextures
    if(mainMega.init("data/texture0.png",128)) {logStr+="[ERROR]MainMega initializing error!\n"; return EXIT_FAILURE;}
    logStr+="MainMega initialized successfully\n";
    std::cout << "MainMega initialized successfully\n";
    //loadingScreen.addPoints(10);
    if(mainIsoMega.init("data/iso-texture.png",128)) {logStr+="[ERROR]MainIsoMega initializing error!\n"; return EXIT_FAILURE;}
    logStr+="MainIsoMega initialized successfully\n";
    std::cout << "MainIsoMega initialized successfully\n";
    //loadingScreen.addPoints(10);
    if(uiMega.init("data/uiTexture.png",64)) {logStr+="[ERROR]uiMega initializing error!\n"; return EXIT_FAILURE;}
    logStr+="uiMega initialized successfully\n";
    std::cout << "uiMega initialized successfully\n";
    //loadingScreen.addPoints(10);
    if(imprMega.init("data/buildings/buildings.png",128)) {logStr+="[ERROR]imprMega initializing error!\n"; return EXIT_FAILURE;}
    logStr+="imprMega initialized successfully\n";
    std::cout << "imprMega initialized successfully\n";
    //loadingScreen.addPoints(10);
    //map initializing
    if(!Grid.loadFromFile("data/grid.png")) {logStr+="[ERROR]data/grid.png loading error\n"; return EXIT_FAILURE;}
    logStr+="data/grid.png loaded successfully\n";
    //loadingScreen.addPoints(1);
    Grid.setSmooth(true);
    Grid.setRepeated(true);
    Grid.generateMipmap();
    std::cout << "data/grid.png loaded successfully\n";
    if(!bfUI::uiBackground.loadFromFile("data/ui-background.png")) {logStr+="[ERROR]data/ui-background.png loading error\n"; return EXIT_FAILURE;}
    logStr+="data/ui-background.png loaded successfully\n";
    std::cout << "data/ui-background.png loaded successfully\n";
    //loadingScreen.addPoints(1);
    bfUI::uiBackground.setSmooth(true);
    bfUI::uiBackground.setRepeated(true);
    ///loading sounds
    if (!bfUI::clickSoundBuffer.loadFromFile("data/click.wav"))
    {
        logStr+="[ERROR]data/click.wav initialization error!\n";
        return EXIT_FAILURE;
    }
    logStr+="click.wav initialized successfully\n";
    std::cout << "data/click.wav loaded successfully\n";
    //initializing unitTypes
    std::ifstream unitFile;
    unitFile.open("data/units/unitList.txt");
    if(!unitFile.good()) {logStr+="[ERROR]data/units/unitList.txt doesn't exist!\n";return EXIT_FAILURE;}
    std::string name;
    std::string pl("data/units/");
    //const std::string ext("/info.json");
    while(!unitFile.eof())
    {
        unitFile >> name;
        name=pl+name;
        std::shared_ptr<UnitKind> newUnit;
        newUnit=loadUnitFromFile(name, state);
        if(newUnit.get()!=nullptr)
            unitTypes.push_back(newUnit);
        //loadingScreen.addPoints(1);
    }
    unitFile.close();
    std::cout << "data/units loaded successfully\n";
    std::fstream buildingFile;
    buildingFile.open("data/buildings/buildingList.txt",std::ios::in);
    if(!buildingFile.good()) {logStr+="[ERROR]data/buildings/buildingList.txt doesn't exist!\n";return EXIT_FAILURE;}
    logStr+="data/buildings/buildingList.txt found\n";
    //loadingScreen.addPoints(1);
    pl="data/buildings/";
    while(!buildingFile.eof())
    {
        buildingFile >> name;
        name=pl+name;
        std::shared_ptr<BuildingType> newBuilding;
        newBuilding=loadBuildingFromFile(name);
        if(newBuilding.get()!=nullptr)
            buildingTypes.push_back(newBuilding);
        //loadingScreen.addPoints(1);
    }
    buildingFile.close();
    std::cout << "data/buildings loaded successfully\n";
    return EXIT_SUCCESS;
}
std::shared_ptr<UnitKind> Data::loadUnitFromFile(const std::string& fileName, State& state)
{
    //std::cout << "Loading " << fileName << "\n";
    std::string ext("/info.json");
    std::string textureExt("/texture.png");
    std::ifstream file;
    try {file.open(fileName+ext);}
    catch(...) {return nullptr;}
    if(!file.good())
        return nullptr;
    Json::Value values;
    Json::CharReaderBuilder b;
#ifdef __linux__
    string errs;
#else
    Json::String errs;
#endif
    if(!Json::parseFromStream(b, file, &values, &errs))
    {
        //std::cerr << errs << "\n";
        return nullptr;
    }
    file.close();
    std::shared_ptr<UnitKind> ret;
    std::string unitType;
    unitType=values.get("type",Json::Value("")).asString();
    if(unitType=="melee")
        ret.reset(new melee);
    else if(unitType == "ranged")
    {
        ranged* ran=new ranged;
        ran->rangedStr=values.get("ranged_damage",Json::Value(0)).asInt();
        ran->range=values.get("range",Json::Value(1)).asInt();
        ran->capacity=values.get("capacity",Json::Value(0)).asInt();
        ret.reset(ran);
    }
    else if(unitType == "officer")
    {
        officer* off=new officer;
        off->maxSize=values.get("slots",Json::Value(3)).asInt();
        off->totalNrCommandedUnits=values.get("capacity",Json::Value(20)).asInt();
        ret.reset(off);
    }
    else if(unitType == "settler")
        ret.reset(new settler);
    else
        return std::shared_ptr<UnitKind>();
    ret.get()->maxMP=values.get("mp",0).asInt();
    ret.get()->maxHP=values.get("hp",1).asInt();
    ret.get()->att=values.get("attack",0).asInt();
    ret.get()->def=values.get("defense",0).asInt();
    ret.get()->str=values.get("damage",Json::Value(0)).asInt();
    std::string tmpName(values.get("name","DEFAULT_UNIT_TYPE_NAME").asString());
    ret.get()->name=tmpName;
    //loading animation info
    Json::Value animValueList=values.get("animations",Json::Value());
    for(std::size_t i=0u;i<animValueList.size();i++)
    {
        AnimationInfo newInfo;
        Json::Value animValue=animValueList.get(i,Json::Value());
        if(animValue==Json::Value()) continue;
        newInfo.name=animValue.get("name","DEFAULT_NAME").asString();
        newInfo.firstIndex=animValue.get("first",Json::Value(0)).asInt();
        newInfo.lastIndex=animValue.get("last",Json::Value(0)).asInt();
        newInfo.frameDuration=animValue.get("frame_length",Json::Value(1.f)).asFloat();
        newInfo.isLooped=animValue.get("looped",false).asBool();
        ret.get()->animationInfo.push_back(newInfo);
    }
    //loading texture
    ret.get()->textureUP.reset(new MegaTexture);
    if(ret.get()->textureUP.get()->init(fileName+textureExt,128))
    {
        return nullptr;
    }
    ret.get()->setOwnerTexture(uiMega,circleBack);
    ret.get()->state=&state;
    return ret;
}
std::shared_ptr<BuildingType> Data::loadBuildingFromFile(const std::string& fileName)
{
    std::string ext("/info.json");
    std::string textureExt("/texture.png");
    std::ifstream file;
    std::vector<std::string> resourcesTab;
    initResourcesTab(resourcesTab);
    try {file.open(fileName+ext);}
    catch(...) {return nullptr;}
    if(!file.good())
    {
        //std::cout << "WARNING: " << fileName << " not found!\n";
        return nullptr;
    }
    Json::Value values;
    Json::CharReaderBuilder b;
#ifdef __linux__
    string errs;
#else
    Json::String errs;
#endif
    if(!Json::parseFromStream(b, file, &values, &errs))
    {
        //std::cerr << errs << "\n";
        return nullptr;
    }
    file.close();
    //std::cout << "Loading " << fileName << "\n";
    std::shared_ptr<BuildingType> ret;
    std::string buildingType;
    buildingType=values.get("type",Json::Value("")).asString();
    int stages_num=values.get("stages_num",1).asInt();
    if(buildingType=="resource")
    {
        ResourceBuilding* res=new ResourceBuilding();
        std::string givenResourceName=values.get("given_resource","?").asString();
        int j=0;
        ///given resource ID
        for(j=0;j<RESOURCE_TYPES_NUMBER;j++)
        {
            if(givenResourceName==resourcesTab[j])
            {
                res->givenResourceID=j+gold;
                break;
            }
        }
        if(j==RESOURCE_TYPES_NUMBER)
        {
            delete res;
            return nullptr;
        }
        //
        for(uint32_t i=0u;i<values["resource_for_worker"].size();i++)
            res->givenResourceNumber[i]=values["resource_for_worker"][i].asInt();
        ret.reset(res);
    }
    else if(buildingType=="farm")
    {
        Farm* farm=new Farm();
        for(uint32_t i=0u;i<values["resource_for_worker"].size();i++)
            farm->givenFood[i]=values["resource_for_worker"][i].asInt();
        ret.reset(farm);
    }
    else if(buildingType=="recruit")
    {
        RecruitBuilding* rec=new RecruitBuilding();
        rec->recruitedUnit=values.get("trained_unit",0).asInt();
        for(int i=0;i<RESOURCE_TYPES_NUMBER;i++)
        {
            rec->recruitCost[i]=values["recruit_cost"].get(resourcesTab[i],0).asInt();
        }
        //std::cout << "REC: " << fileName << "\n";
        ret.reset(rec);
    }
    else if(buildingType=="house")
    {
        HouseBuilding* hou=new HouseBuilding();
        for(int i=0;i<stages_num;i++)
        {
            hou->heldPopulation[i]=values["held_population"][i].asInt();
        }
        ret.reset(hou);
    }
    else if(buildingType=="path")
    {
        std::string tmpName=values.get("name","").asString();
        roadInfo.name=tmpName;
        roadInfo.maxStage=stages_num;
        roadInfo.mapIconsMega=&mainMega;
        roadInfo.ownerTextureInd=circleBack;
        roadInfo.otMega=&uiMega;
        roadInfo.iconIndFirst=0;
        roadInfo.iconIndLast=stages_num-1;
        for(int i=0;i<roadInfo.maxStage;i++)
        {
            roadInfo.mapIconsIndexes[i]=4*i+roadBegin;
            roadInfo.roadMoveCost[i]=values["mp"][i].asInt();
            for(int j=0;j<RESOURCE_TYPES_NUMBER+1;j++)
            {
                roadInfo.buildingCost[i][j]=values["building_cost"][i].get(resourcesTab[j],0).asInt();
            }
        }
        return std::shared_ptr<BuildingType>();
    }
    else
    {
        return std::shared_ptr<BuildingType>();
    }
    ret.get()->name=values.get("name","").asString();
    ret.get()->maxStage=stages_num;
    for(int i=0;i<ret->maxStage;i++)
    {
        ret.get()->maxEmployedNr[i]=values["max_workers"][i].asInt();
        ret.get()->requiredTechnologyId[i]=values.get("required_tech",Json::Value()).get(i,0).asInt();
        for(int j=0;j<RESOURCE_TYPES_NUMBER+1;j++)
        {
            ret.get()->buildingCost[i][j]=values["building_cost"][i].get(resourcesTab[j],0).asInt();
        }
    }
    //Resource texture
    ret.get()->itMega.reset(new MegaTexture);
    if(ret.get()->itMega.get()->init(fileName+textureExt,128))
    {
        return std::shared_ptr<BuildingType>();
    }
    ret.get()->iconIndFirst=0;
    ret.get()->iconIndLast=ret->maxStage-1;
    ret.get()->otMega=&uiMega;
    ret.get()->ownerTextureInd=circleBack;
    return ret;
}

void Data::initResourcesTab(std::vector<std::string>& rTab)
{
    rTab.push_back("gold");
    rTab.push_back("wood");
    rTab.push_back("stone");
    rTab.push_back("iron");
    rTab.push_back("horse");
    rTab.push_back("jewels");
    for(int i=6;i<RESOURCE_TYPES_NUMBER;i++)
        rTab.push_back("");
    rTab.push_back("workers");
}
#ifndef DATA_HPP
#define DATA_HPP
#include <SFML/Graphics.hpp>
#include "basic/megaTexture.hpp"
#include "building.hpp"
struct State;
class UnitKind;
class LoadingScreen;
extern std::string logStr;
//extern LoadingScreen loadingScreen;
class Data
{
    public:
        Data(Data const&)               = delete;
        void operator=(Data const&)  = delete;
    public:
        Data(State& state);
        virtual ~Data();
        const sf::Texture& getNullTexture() const {return nullTexture;}
        const sf::Texture& getLoadingTexture() const {return loadingTexture;}
        const sf::Texture& getBarTexture() const {return barTexture;}
        const sf::Texture& getSliderTexture() const {return sliderTexture;}
        const sf::Texture& getGridTexture() const {return Grid;}
        const sf::Font& getFont() const {return font;}
        const MegaTexture& getMainMegaTexture() const {return mainMega;}
        const MegaTexture& getMainIsoMegaTexture() const {return mainIsoMega;}
        const MegaTexture& getUiMegaTexture() const {return uiMega;}
        const std::shared_ptr<UnitKind>& getUnitType(int i) const {return unitTypes[i];}
        std::size_t getUnitTypeSize() const {return unitTypes.size();}
        const std::shared_ptr<BuildingType>& getBuildingType(int i) const {return buildingTypes[i];}
        std::size_t getBuildingTypesSize() const {return buildingTypes.size();}
        const RoadBuilding& getRoadInfo() const {return roadInfo;}

    protected:

    private:
        bool init(State& state);
        std::shared_ptr<UnitKind> loadUnitFromFile(const std::string&, State&);
        std::shared_ptr<BuildingType> loadBuildingFromFile(const std::string&);
        void initResourcesTab(std::vector<std::string>&);
        sf::Texture nullTexture;
        sf::Texture loadingTexture;
        sf::Texture barTexture, sliderTexture;
        sf::Texture Grid;
        sf::Font font;
        MegaTexture mainMega, imprMega;
        MegaTexture mainIsoMega;
        MegaTexture uiMega;
        std::vector<std::shared_ptr<UnitKind> > unitTypes;
        std::vector<std::shared_ptr<BuildingType> > buildingTypes;
        RoadBuilding roadInfo;
        std::map<std::string,sf::String> lang;
};

#endif // DATA_HPP

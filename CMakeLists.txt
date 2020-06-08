cmake_minimum_required(VERSION 3.15)
project(Strategia_CLion VERSION 0.18.1)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_FLAGS "-Wall -std=c++17 -lstdc++fs -pthread")

include_directories(include)
include_directories(lib)
include_directories(lib/SelbaWard)

add_executable(Strategia_CLion
        #[[lib/SelbaWard/BitmapFont.cpp
        lib/SelbaWard/BitmapFont.hpp
        lib/SelbaWard/BitmapText.cpp
        lib/SelbaWard/BitmapText.hpp
        lib/SelbaWard/ConsoleScreen.cpp
        lib/SelbaWard/ConsoleScreen.hpp
        lib/SelbaWard/ConsoleScreenOld.cpp
        lib/SelbaWard/ConsoleScreenOld.hpp
        lib/SelbaWard/Crosshair.cpp
        lib/SelbaWard/Crosshair.hpp
        lib/SelbaWard/ElasticSprite.cpp
        lib/SelbaWard/ElasticSprite.hpp
        lib/SelbaWard/GallerySprite.cpp
        lib/SelbaWard/GallerySprite.hpp
        lib/SelbaWard/PaletteEnums.hpp
        lib/SelbaWard/PieChart.cpp
        lib/SelbaWard/PieChart.hpp
        lib/SelbaWard/PixelDisplay.cpp
        lib/SelbaWard/PixelDisplay.hpp
        lib/SelbaWard/Ring.cpp
        lib/SelbaWard/Ring.hpp
        lib/SelbaWard/SpinningCard.cpp
        lib/SelbaWard/SpinningCard.hpp
        lib/SelbaWard/Spline.cpp
        lib/SelbaWard/Spline.hpp
        lib/SelbaWard/Sprite3d.cpp
        lib/SelbaWard/Sprite3d.hpp
        lib/SelbaWard/Starfield.cpp
        lib/SelbaWard/Starfield.hpp
        lib/SelbaWard/TileMap.hpp
        lib/SelbaWard/TileMap.inl
        lib/SelbaWard.hpp
        ]]#
        lib/SelbaWard/Common.hpp
        lib/SelbaWard/Exception.hpp
        lib/SelbaWard/Line.cpp
        lib/SelbaWard/Line.hpp
        lib/SelbaWard/NinePatch.cpp
        lib/SelbaWard/NinePatch.hpp
        lib/SelbaWard/ProgressBar.cpp
        lib/SelbaWard/ProgressBar.hpp

        include/ability.cpp
        include/ability.hpp
        include/Battle.cpp
        include/Battle.h
        include/bfUI.cpp
        include/bfUI.hpp
        include/building.cpp
        include/building.hpp
        include/city.cpp
        include/city.hpp
        include/basic/constants.hpp
        include/Data.cpp
        include/Data.hpp
        include/LoadingScreen.cpp
        include/LoadingScreen.hpp
        include/map.cpp
        include/map.hpp
        include/basic/megaTexture.cpp
        include/basic/megaTexture.hpp
        include/path.cpp
        include/path.hpp
        include/player.cpp
        include/player.hpp
        include/tile.cpp
        include/tile.hpp
        include/unit.cpp
        include/unit.hpp
        include/ui.cpp
        include/ui.hpp
        include/mainMenu.cpp
        include/mainMenu.hpp
        main.cpp
        include/Settings.cpp
        include/Settings.hpp
        include/basic/my_vector.cpp
        include/basic/my_vector.hpp)
#add_link_libraries(Strategia_CLion, "jsoncpp")
#target_link_libraries(Strategia_CLion /usr/lib/x86_64-linux-gnu/libsfml-system.so)
target_link_libraries(Strategia_CLion X11)
target_link_libraries(Strategia_CLion sfml-system)
target_link_libraries(Strategia_CLion sfml-window)
target_link_libraries(Strategia_CLion sfml-graphics)
target_link_libraries(Strategia_CLion sfml-audio)
target_link_libraries(Strategia_CLion jsoncpp)
target_link_libraries(Strategia_CLion tgui)
target_link_libraries(Strategia_CLion stdc++fs)
#include "building.hpp"
#include "player.hpp"
#include "city.hpp"
#include "basic/megaTexture.hpp"

Player::Player(bool isComp, const sf::String& plName): name(plName)
{
    techPts=0;
	this->isComp=isComp;
	for(int i=0;i<RESOURCE_TYPES_NUMBER;i++)
		resourceQuantity[i]=0;
}
Player::Player(int begRes[], bool isComp, const sf::String& plName): name(plName)
{
	this->isComp=isComp;
	for(int i=0;i<RESOURCE_TYPES_NUMBER;i++)
		resourceQuantity[i]=begRes[i];
}

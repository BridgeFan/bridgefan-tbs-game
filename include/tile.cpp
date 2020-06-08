#include "tile.hpp"
#include "ui.hpp"
#include "building.hpp"
#include "player.hpp"
#include "city.hpp"
#include "basic/megaTexture.hpp"
#include "Data.hpp"

tile::tile(const Data& data): data(data)
{
	level=0;
	forestType=0;
	owner=-1;
	isRiver=false;
	river=0x0;
	hDiff=0x0;
	resource=0;
	roadStage=0;
	cityField=-1;
	terrainType=grass;
	//ownerRect.setFillColor(sf::Color::Transparent);
	//ownerRect.setSize({tileSize,tileSize});
	background.setSize({tileSize,tileSize});
    background.setFillColor({63,63,63,127});
    influencePts.clear();
    for(int i=0;i<4;i++)
    {
        line[i].setThickness(32);
        line[i].setColor(sf::Color::Transparent);
    }
	//unitIndex=-1;
	//improIndex=-1;
}
void tile::draw(sf::RenderTarget& target, const State& state) const
{
	target.draw(mainSprite);
	//target.draw(addSprite, states); ///add OR river
	target.draw(riverSprite);
	if(state.isIso)
        return;
	target.draw(hPartSprite);
	target.draw(hDiffSprite);
	for(int i=0;i<8;i++)
		target.draw(roadSprites[i]);
    /*if(river>0)
	{
        target.draw(resourceBack, states);
        target.draw(resourceIcon, states);
    }*/
	//target.draw(ownerRect, states);
	for(int i=0;i<4;i++)
        target.draw(line[i]);
}

void tile::UpdateSprites()
{
	switch(level)
	{
	case -2:
	    data.getMainMegaTexture().assignTexture(mainSprite,sea);
	    break;
	case -1:
	    data.getMainMegaTexture().assignTexture(mainSprite,bay);
	    break;
	//case 0:
	//    data.getMainMegaTexture().assignTexture(mainSprite,sand);break;
	//	mainSprite.setTexture(data.getMainMegaTexture()[sand]); break;
    default:
	    data.getMainMegaTexture().assignTexture(mainSprite,terrainType);
	    break;
	}
	///river sprite instead of add sprite
	if(forestType==1) data.getMainMegaTexture().assignTexture(riverSprite,forestID);
	else if(forestType == 2) data.getMainMegaTexture().assignTexture(riverSprite,taiga);
	if(!forestType && resource>0)
    {
        //gameUI.uiMega.assignTexture(resourceIcon,resource);
        //gameUI.uiMega.assignTexture(resourceBack,circleBack);
        switch(resource)
        {
            case stone: data.getMainMegaTexture().assignTexture(riverSprite,stoneBack);break;
            case iron: data.getMainMegaTexture().assignTexture(riverSprite,ironBack);break;
            case horses: data.getMainMegaTexture().assignTexture(riverSprite,horsesBack);break;
            case jewels: data.getMainMegaTexture().assignTexture(riverSprite,jewelsBack);break;
            case gold: data.getMainMegaTexture().assignTexture(riverSprite,goldBack);break;
        }
    }
	for(int i=0;i<4;i++)
	{
	    roadSprites[i].setRotation(i*90.f);
        roadSprites[4+i].setRotation(i*90.f);
	}
	riverScript();
	heightScript();
}
void tile::setPosition(int x, int y, bool isIso)
{
    mainSprite.setPosition(x,y);
    //addSprite.setPosition(x,y);
    //ownerRect.setPosition(x,y);
    background.setPosition(x,y);
    //resourceBack.setPosition(x+64,y);
    //resourceIcon.setPosition(x+64,y);
    setPositionTmp(x,y,riverSprite);
    if(isIso)
        return;
    for(int i=0;i<8;i++)
        setPositionTmp(x,y,roadSprites[i]);
    setPositionTmp(x,y,hDiffSprite);
    setPositionTmp(x,y,hPartSprite);
	line[0].setPoints({(float)x,y+16.f},{x+128.f,y+16.f});
	line[1].setPoints({x+112.f,(float)y},{x+112.f,y+128.f});
	line[2].setPoints({x+128.f,y+112.f},{(float)x,y+112.f});
	line[3].setPoints({x+16.f,y+128.f},{x+16.f,(float)y});
}
void tile::setPositionTmp(int x, int y, sf::Sprite& tmpSpr)
{
    if(tmpSpr.getRotation()==90.f)
        if(tmpSpr.getScale().y==-1.f)
            tmpSpr.setPosition(x,y);
        else
            tmpSpr.setPosition(x+128,y);
    else if(tmpSpr.getRotation()==180.f)
        if(tmpSpr.getScale().y==-1.f)
            tmpSpr.setPosition(x+128,y);
        else
            tmpSpr.setPosition(x+128,y+128);
    else if(tmpSpr.getRotation()==270.f)
        if(tmpSpr.getScale().y==-1.f)
            tmpSpr.setPosition(x+128,y+128);
        else
            tmpSpr.setPosition(x,y+128);
    else if(tmpSpr.getScale().y==-1.f)
        tmpSpr.setPosition(x,y+128);
    else
        tmpSpr.setPosition(x,y);
}
void tile::riverScript()
{
    if(isRiver)
        data.getMainMegaTexture().assignTexture(riverSprite,riverBegin+(river&0x0F));
    /*if(!isRiver)
    {
        if((river&0x0F)==0x0F)
        {
            switch((river&0xF0)>>4)
            {
            case 0x0:
                data.getMainMegaTexture().assignTexture(riverSprite,d4Parts); return;
            case 0x8:
                data.getMainMegaTexture().assignTexture(riverSprite,d3Parts); return;
            case 0x1:
                data.getMainMegaTexture().assignTexture(riverSprite,d3Parts);
                riverSprite.setRotation(90.f); return;
            case 0x2:
                data.getMainMegaTexture().assignTexture(riverSprite,d3Parts);
                riverSprite.setRotation(180.f); return;
            case 0x4:
                data.getMainMegaTexture().assignTexture(riverSprite,d3Parts);
                riverSprite.setRotation(270.f); return;
            case 0xA:
                data.getMainMegaTexture().assignTexture(riverSprite,d180Parts); return;
            case 0x5:
                data.getMainMegaTexture().assignTexture(riverSprite,d180Parts);
                riverSprite.setRotation(90.f); return;
            }
        }
        if((river&0x37)==0x7)
        {
            data.getMainMegaTexture().assignTexture(riverSprite,d90Parts);
        }
        else if((river&0x6E)==0xE)
        {
            data.getMainMegaTexture().assignTexture(riverSprite,d90Parts);
            riverSprite.setRotation(90.f);
        }
        else if((river&0xCD)==0xD)
        {
            data.getMainMegaTexture().assignTexture(riverSprite,d90Parts);
            riverSprite.setRotation(180.f);
        }
        else if((river&0x9B)==0xB)
        {
            data.getMainMegaTexture().assignTexture(riverSprite,d90Parts);
            riverSprite.setRotation(270.f);
        }
        else if((river&0x13)==0x3)
        {
            data.getMainMegaTexture().assignTexture(riverSprite,dPart);
        }
        else if((river&0x26)==0x6)
        {
            data.getMainMegaTexture().assignTexture(riverSprite,dPart);
            riverSprite.setRotation(90.f);
        }
        else if((river&0x4C)==0xC)
        {
            data.getMainMegaTexture().assignTexture(riverSprite,dPart);
            riverSprite.setRotation(180.f);
        }
        else if((river&0x89)==0x9)
        {
            data.getMainMegaTexture().assignTexture(riverSprite,dPart);
            riverSprite.setRotation(270.f);
        }
    }
    else if((river&0xF)==0x3)
    {
        if((river&0x40)==0x40) data.getMainMegaTexture().assignTexture(riverSprite,o90225Rivers);
        else data.getMainMegaTexture().assignTexture(riverSprite,o90Rivers);
    }
    else if((river&0xF)==0x5)
    {
        data.getMainMegaTexture().assignTexture(riverSprite,o180Rivers);
    }
    else if((river&0xF)==0x6)
    {
        if(river&0x80) data.getMainMegaTexture().assignTexture(riverSprite,o90225Rivers);
        else data.getMainMegaTexture().assignTexture(riverSprite,o90Rivers);
        riverSprite.setRotation(90.f);
    }
    else if((river&0xF)==0x7)
    {
        data.getMainMegaTexture().assignTexture(riverSprite,o90180Rivers);
    }
    else if((river&0xF)==0x9)
    {
        riverSprite.setRotation(270.f);
        if(river&0x20) data.getMainMegaTexture().assignTexture(riverSprite,o90225Rivers);
        else data.getMainMegaTexture().assignTexture(riverSprite,o90Rivers);
    }
    else if((river&0xF)==0xA)
    {
        data.getMainMegaTexture().assignTexture(riverSprite,o180Rivers);
        riverSprite.setRotation(90.f);
    }
    else if((river&0xF)==0xB)
    {
        data.getMainMegaTexture().assignTexture(riverSprite,o90180Rivers);
        riverSprite.setRotation(270.f); //
    }
    else if((river&0xF)==0xC)
    {
        riverSprite.setRotation(180.f);
        if(river&0x10) data.getMainMegaTexture().assignTexture(riverSprite,o90225Rivers);
        else data.getMainMegaTexture().assignTexture(riverSprite,o90Rivers);
    }
    else if((river&0xF)==0xD)
    {
        data.getMainMegaTexture().assignTexture(riverSprite,o90180Rivers);
        riverSprite.setRotation(180.f);
    }
    else if((river&0xF)==0xE)
    {
        data.getMainMegaTexture().assignTexture(riverSprite,o90180Rivers);
        riverSprite.setRotation(90.f); //
    }
    else if((river&0xF)==0xF)
    {
        data.getMainMegaTexture().assignTexture(riverSprite,PlusRivers);
    }
	else switch(river)
	{
    case 0x01: case 0x11: case 0x81: case 0x91:
        data.getMainMegaTexture().assignTexture(riverSprite,oneRiver); return;
    case 0x02: case 0x12: case 0x22: case 0x32:
        data.getMainMegaTexture().assignTexture(riverSprite,oneRiver);
        riverSprite.setRotation(90.f);return;
    case 0x04: case 0x24: case 0x44: case 0x64:
        data.getMainMegaTexture().assignTexture(riverSprite,oneRiver);
        riverSprite.setRotation(180.f); return;
    case 0x08: case 0x48: case 0x88: case 0xC8:
        data.getMainMegaTexture().assignTexture(riverSprite,oneRiver);
        riverSprite.setRotation(270.f); return;
    case 0x14: case 0x34: case 0x54: case 0x74:
        data.getMainMegaTexture().assignTexture(riverSprite,o135Rivers);
        riverSprite.setRotation(180.f);
        riverSprite.setScale(1.f,-1.f); return;
    case 0x18: case 0x58: case 0x98: case 0xD8:
        data.getMainMegaTexture().assignTexture(riverSprite,o135Rivers);
        riverSprite.setRotation(270.f); return;
    case 0x28: case 0x68: case 0xA8: case 0xE8:
        data.getMainMegaTexture().assignTexture(riverSprite,o135Rivers);
        riverSprite.setRotation(270.f);
        riverSprite.setScale(1.f,-1.f); return;
    case 0x21: case 0x31: case 0xA1: case 0xB1:
        data.getMainMegaTexture().assignTexture(riverSprite,o135Rivers); return;
    case 0x41: case 0x51: case 0xC1: case 0xD1:
        data.getMainMegaTexture().assignTexture(riverSprite,o135Rivers);
        riverSprite.setScale(1.f,-1.f); return;
    case 0x42: case 0x52: case 0x62: case 0x72:
        data.getMainMegaTexture().assignTexture(riverSprite,o135Rivers);
        riverSprite.setRotation(90.f); return;
    case 0x82: case 0x92: case 0xA2: case 0xB2:
        data.getMainMegaTexture().assignTexture(riverSprite,o135Rivers);
        riverSprite.setRotation(90.f);
        riverSprite.setScale(1.f,-1.f); return;
    case 0x84: case 0xA4: case 0xC4: case 0xE4:
        data.getMainMegaTexture().assignTexture(riverSprite,o135Rivers);
        riverSprite.setRotation(180.f); return;
    case 0x10:
        data.getMainMegaTexture().assignTexture(riverSprite,d1River); return;
    case 0x20:
        data.getMainMegaTexture().assignTexture(riverSprite,d1River);
        riverSprite.setRotation(90.f); return;
    case 0x40:
        data.getMainMegaTexture().assignTexture(riverSprite,d1River);
        riverSprite.setRotation(180.f); return;
    case 0x80:
        data.getMainMegaTexture().assignTexture(riverSprite,d1River);
        riverSprite.setRotation(270.f); return;
    case 0x30:
        data.getMainMegaTexture().assignTexture(riverSprite,d90Rivers); return;
    case 0x60:
        data.getMainMegaTexture().assignTexture(riverSprite,d90Rivers);
        riverSprite.setRotation(90.f); return;
    case 0xC0:
        data.getMainMegaTexture().assignTexture(riverSprite,d90Rivers);
        riverSprite.setRotation(180.f); return;
    case 0x90:
        data.getMainMegaTexture().assignTexture(riverSprite,d90Rivers);
        riverSprite.setRotation(270.f); return;
    case 0x50:
        data.getMainMegaTexture().assignTexture(riverSprite,d180Rivers); return;
    case 0xA0:
        data.getMainMegaTexture().assignTexture(riverSprite,d180Rivers);
        riverSprite.setRotation(90.f); return;
    case 0x70:
        data.getMainMegaTexture().assignTexture(riverSprite,d90180Rivers); return;
    case 0xE0:
        data.getMainMegaTexture().assignTexture(riverSprite,d90180Rivers);
        riverSprite.setRotation(90.f); return;
    case 0xD0:
        data.getMainMegaTexture().assignTexture(riverSprite,d90180Rivers);
        riverSprite.setRotation(180.f); return;
    case 0xB0:
        data.getMainMegaTexture().assignTexture(riverSprite,d90180Rivers);
        riverSprite.setRotation(270.f); return;
    case 0xF0:
        data.getMainMegaTexture().assignTexture(riverSprite,XRivers);  return;
    case 0x61: case 0x71: case 0xE1: case 0xF1:
        data.getMainMegaTexture().assignTexture(riverSprite,o135225Rivers);  return;
    case 0xC2: case 0xD2: case 0xE2: case 0xF2:
        data.getMainMegaTexture().assignTexture(riverSprite,o135225Rivers);
        riverSprite.setRotation(90.f); return;
    case 0x94: case 0xB4: case 0xD4: case 0xF4:
        data.getMainMegaTexture().assignTexture(riverSprite,o135225Rivers);
        riverSprite.setRotation(180.f); return;
    case 0x38: case 0x78: case 0xB8: case 0xF8:
        data.getMainMegaTexture().assignTexture(riverSprite,o135225Rivers);
        riverSprite.setRotation(270.f); return;
	}*/

}
/*void tile::setColor(const sf::Color& color)
{
    //ownerRect.setFillColor(color);
}*/
void tile::heightScript()
{
    switch(hDiff&0x0F)
    {
    case 0x0:
        break;
    case 0x1:
        data.getMainMegaTexture().assignTexture(hDiffSprite,oHill);
        break;
    case 0x2:
        data.getMainMegaTexture().assignTexture(hDiffSprite,oHill);
        hDiffSprite.setRotation(90.f);
        break;
    case 0x4:
        data.getMainMegaTexture().assignTexture(hDiffSprite,oHill);
        hDiffSprite.setRotation(180.f);
        break;
    case 0x8:
        data.getMainMegaTexture().assignTexture(hDiffSprite,oHill);
        hDiffSprite.setRotation(270.f);
        break;
    case 0x3:
        data.getMainMegaTexture().assignTexture(hDiffSprite,o90Hill);
        break;
    case 0x6:
        data.getMainMegaTexture().assignTexture(hDiffSprite,o90Hill);
        hDiffSprite.setRotation(90.f);
        break;
    case 0xC:
        data.getMainMegaTexture().assignTexture(hDiffSprite,o90Hill);
        hDiffSprite.setRotation(180.f);
        break;
    case 0x9:
        data.getMainMegaTexture().assignTexture(hDiffSprite,o90Hill);
        hDiffSprite.setRotation(270.f);
        break;
    case 0x5:
        data.getMainMegaTexture().assignTexture(hDiffSprite,o180Hill);
        break;
    case 0xA:
        data.getMainMegaTexture().assignTexture(hDiffSprite,o180Hill);
        hDiffSprite.setRotation(90.f);
        break;
    case 0x7:
        data.getMainMegaTexture().assignTexture(hDiffSprite,o90180Hill);
        break;
    case 0xE:
        data.getMainMegaTexture().assignTexture(hDiffSprite,o90180Hill);
        hDiffSprite.setRotation(90.f);
        break;
    case 0xD:
        data.getMainMegaTexture().assignTexture(hDiffSprite,o90180Hill);
        hDiffSprite.setRotation(180.f);
        break;
    case 0xB:
        data.getMainMegaTexture().assignTexture(hDiffSprite,o90180Hill);
        hDiffSprite.setRotation(270.f);
        break;
    case 0xF:
        data.getMainMegaTexture().assignTexture(hDiffSprite,allHill);
        break;
    }
    if(hDiff==0xF0)
    {
        data.getMainMegaTexture().assignTexture(hPartSprite,d90180Hills);
    }
    else if((hDiff&0x7F)==0x70)
    {
        data.getMainMegaTexture().assignTexture(hPartSprite,d90180Hills);
    }
    else if((hDiff&0xEF)==0xE0)
    {
        data.getMainMegaTexture().assignTexture(hPartSprite,d90180Hills);
        hPartSprite.setRotation(90.f);
    }
    else if((hDiff&0xDF)==0xD0)
    {
        data.getMainMegaTexture().assignTexture(hPartSprite,d90180Hills);
        hPartSprite.setRotation(180.f);
    }
    else if((hDiff&0xBF)==0xB0)
    {
        data.getMainMegaTexture().assignTexture(hPartSprite,d90180Hills);
        hPartSprite.setRotation(270.f);
    }
    else if((hDiff&0x5F)==0x50)
    {
        data.getMainMegaTexture().assignTexture(hPartSprite,d180Hills);
    }
    else if((hDiff&0xAF)==0xA0)
    {
        data.getMainMegaTexture().assignTexture(hPartSprite,d180Hills);
        hPartSprite.setRotation(90.f);
    }
    else if((hDiff&0x37)==0x30)
    {
        data.getMainMegaTexture().assignTexture(hPartSprite,d90Hills);
    }
    else if((hDiff&0x6E)==0x60)
    {
        data.getMainMegaTexture().assignTexture(hPartSprite,d90Hills);
        hPartSprite.setRotation(90.f);
    }
    else if((hDiff&0xCD)==0xC0)
    {
        data.getMainMegaTexture().assignTexture(hPartSprite,d90Hills);
        hPartSprite.setRotation(180.f);
    }
    else if((hDiff&0x9B)==0x90)
    {
        data.getMainMegaTexture().assignTexture(hPartSprite,d90Hills);
        hPartSprite.setRotation(270.f);
    }
    else if((hDiff&0x13)==0x10)
    {
        data.getMainMegaTexture().assignTexture(hPartSprite,dHill);
    }
    else if((hDiff&0x26)==0x20)
    {
        data.getMainMegaTexture().assignTexture(hPartSprite,dHill);
        hPartSprite.setRotation(90.f);
    }
    else if((hDiff&0x4C)==0x40)
    {
        data.getMainMegaTexture().assignTexture(hPartSprite,dHill);
        hPartSprite.setRotation(180.f);
    }
    else if((hDiff&0x89)==0x80)
    {
        data.getMainMegaTexture().assignTexture(hPartSprite,dHill);
        hPartSprite.setRotation(270.f);
    }
}
int tile::tileValue(int l, bool isDiagonal, bool diagCrosRiv, int8_t fromRoadStage) const
{
	if(level<0)
		return INT_MAX/2;
    if(fromRoadStage>0 && roadStage>0)
    {
        if(isDiagonal)
            return 0.7*(data.getRoadInfo().roadMoveCost[roadStage-1]+data.getRoadInfo().roadMoveCost[fromRoadStage-1]);
        else
            return 0.5*(data.getRoadInfo().roadMoveCost[roadStage-1]+data.getRoadInfo().roadMoveCost[fromRoadStage-1]);
    }
	int ret=20;
	if(isDiagonal) ret=28;
	if(forestType>0) ret*=2;
	if(isRiver || diagCrosRiv) ret+=120;
	ret+=(60*abs(l-level));
	return ret;
}
void tile::setRoadTexture(int8_t index)
{
    if(roadStage==0)
        roadSprites[index].setTexture(data.getNullTexture());
    else if(index<4)
        data.getMainMegaTexture().assignTexture(roadSprites[index],roadBegin-3+4*roadStage);
    else if (index<8)
        data.getMainMegaTexture().assignTexture(roadSprites[index],roadBegin-2+4*roadStage);

}
void tile::setOwner(int16_t owner)
{
    this->owner=owner;
    actualizeColor();
}
void tile::actualizeColor()
{
    if(owner<0 || cityField<0)
    {
        //ownerRect.setFillColor(sf::Color::Transparent);
        return;
    }
    //sf::Color color=sf::Color::White;//players[cities[owner].owner].playerColor;
    //ownerRect.setFillColor({color.r,color.g,color.b,150});
}
void tile::addInfluence(short owner, int pts)
{
    //found owner
    auto it=find_if(influencePts.begin(),influencePts.end(),[&](const std::pair<int,int>& a){return a.first==owner;});
    if(it==influencePts.end())
        influencePts.push_back(std::make_pair(owner,pts));
    else
        it->second+=pts;
}
int tile::getInfluence(short owner) const
{
    //found owner
    auto it=find_if(influencePts.begin(),influencePts.end(),[&](const std::pair<int,int>& a){return a.first==owner;});
    if(it==influencePts.end())
        return 0;
    else
        return it->second;
}
void tile::setIsoTextures (bool isIso)
{
    if(isIso)
    {
        data.getMainIsoMegaTexture().assignTexture(mainSprite,data.getMainMegaTexture().getIndex(mainSprite.getTextureRect()));
        if(riverSprite.getTexture())
            data.getMainIsoMegaTexture().assignTexture(riverSprite,data.getMainMegaTexture().getIndex(riverSprite.getTextureRect()));
    }
    else
    {
        data.getMainMegaTexture().assignTexture(mainSprite,data.getMainIsoMegaTexture().getIndex(mainSprite.getTextureRect()));
        if(riverSprite.getTexture())
            data.getMainMegaTexture().assignTexture(riverSprite,data.getMainIsoMegaTexture().getIndex(riverSprite.getTextureRect()));
    }
}

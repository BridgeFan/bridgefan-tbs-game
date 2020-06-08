#include "map.hpp"
#include "unit.hpp"
#include "basic/constants.hpp"
#include "player.hpp"
#include "building.hpp"
#include "basic/megaTexture.hpp"
#include "city.hpp"
#include "Data.hpp"
#include <cstdlib>
#include <cmath>
#include <list>
#include <fstream>
#include <typeinfo>
#include <iostream>

bool Map::init(int x, int y)
{
    /*try
    {
        mapa = new tile*[x];
        //isVis = new uint64_t*[x];
        //everVis = new uint64_t*[x];
    }
    catch(...)
    {
        logStr+="[ERROR]Map memory allocation error\n";
        return EXIT_FAILURE;
    }*/
    for (int i = 0; i < x; i++)
	{
        std::vector<uint64_t> tmp;
        std::vector<tile> tmpT;
	    /*try
	    {
            mapa[i]=new tile[y];
            //isVis[i] = new uint64_t[y];
            //everVis[i] = new uint64_t[y];
	    }
	    catch(...)
        {
            for(int j=0;j<i;j++)
            {
                delete[] mapa[j];
                //delete[] isVis[j];
                //delete[] everVis[j];
            }
            delete[] mapa; mapa=nullptr;
            //delete[] isVis; isVis=nullptr;
            //delete[] everVis; everVis=nullptr;
            logStr+="[ERROR]Map memory allocation error\n";
            return EXIT_FAILURE;
        }*/
        for(int j=0;j<x;j++)
        {
            tmp.push_back(0);
            tmpT.push_back(tile(*data));
            //isVis[i][j]=0;
            //everVis[i][j]=0;
        }
        mapa.push_back(tmpT);
        isVis.push_back(tmp);
        everVis.push_back(tmp);
	}
    miniMapBase.create(x,y);
    miniMap.create(x,y);
    mapSize=my_vector2i(x,y);
    return EXIT_SUCCESS;
}
void Map::draw(const State& state, sf::RenderTarget& target, bool isBackground) const
{
    if(!state.isIso)
    for(int i=std::max(0.f,(state.view.getCenter().x-state.view.getSize().x/2.f)/data->getMainMegaTexture().getTileSize()-1);i<std::min((float)getSize().x,(state.view.getCenter().x+state.view.getSize().x/2.f)/data->getMainMegaTexture().getTileSize());i++)
        for(int j=std::max(0.f,(state.view.getCenter().y-state.view.getSize().y/2.f)/data->getMainMegaTexture().getTileSize()-1);j<std::min((float)getSize().y,(state.view.getCenter().y+state.view.getSize().y/2.f)/data->getMainMegaTexture().getTileSize());j++)
        {
            if(isBackground)
            {
                if(wasEverVisible(i,j,state.actualPlayer) && !isVisible(i,j,state.actualPlayer))
                    target.draw(mapa[i][j].background);
            }
            else
            {
                if(state.drawAll || (wasEverVisible(i,j,state.actualPlayer) && !isBackground))
                    mapa[i][j].draw(target, state);
                else
                    target.draw(mapa[i][j].background);
            }
        }
    else
    {
    for(int i=0;i<getSize().x;i++)
        for(int j=0;j<getSize().y;j++)
            mapa[i][j].draw(target,state);

    }
}
void Map::updateOwners()
{
    /*for(int i=0;i<cities.size();i++)
    {
        int s=cities[i].getStage();
        for(int j=int(cities[i].getPosition().x/tileSize)-2*s-2;j<=int(cities[i].getPosition().x/tileSize)+2*s+2;j++)
            for(int k=int(cities[i].getPosition().y/tileSize)-2*s-2;k<=int(cities[i].getPosition().y/tileSize+2*s+2);k++)
                mapa[j][k].addInfluence(i,100);
    }*/
    for(int i=0;i<mapSize.x;i++)
        for(int j=0;j<mapSize.y;j++)
        {
            int tileOwner=-1; //bool isOwn=false;
            for(std::size_t k=0;k<cities.size();k++)
            {
                if(mapa[i][j].getInfluence(k)>mapa[i][j].getInfluence(tileOwner))
                {
                    tileOwner=k;//isOwn=true;
                }
                else if(mapa[i][j].getInfluence(k)==mapa[i][j].getInfluence(tileOwner))
                {
                    //isOwn=false;
                }
            }
            //if(isOwn)   mapa[i][j].setColor(players[cities[tileOwner].owner].playerColor);
            //else        mapa[i][j].setColor(sf::Color::Transparent);
        }
}
void Map::clear()
{
    mapa.clear();
    isVis.clear();
    everVis.clear();
    mapSize=my_vector2i(0,0);
}

bool Map::postInit(const State& state)
{
    std::cout << "Post-initialization\n";
    //post-initialization
    for(int i=0;i<getSize().x;i++)
    {
        for(int j=0;j<getSize().y;j++)
        {
            if(mapa[i][j].level>=0)
            {
                if (i > 0 && mapa[i][j].level<mapa[i-1][j].level)
                    mapa[i][j].hDiff += 0x1;
                if (j > 0 && mapa[i][j].level<mapa[i][j-1].level)
                    mapa[i][j].hDiff += 0x2;
                if (i < mapSize.x - 1 && mapa[i][j].level<mapa[i+1][j].level)
                    mapa[i][j].hDiff += 0x4;
                if (j < mapSize.y - 1 && mapa[i][j].level<mapa[i][j+1].level)
                    mapa[i][j].hDiff += 0x8;
                if (i > 0 && j > 0 && mapa[i][j].level<mapa[i-1][j-1].level)
                    mapa[i][j].hDiff += 0x10;
                if (i < mapSize.x - 1 && j>0 && mapa[i][j].level<mapa[i+1][j-1].level)
                    mapa[i][j].hDiff += 0x20;
                if (i < mapSize.x - 1 && j < mapSize.y - 1 && mapa[i][j].level<mapa[i+1][j+1].level)
                    mapa[i][j].hDiff += 0x40;
                if (i > 0 && j < mapSize.y - 1 && mapa[i][j].level<mapa[i-1][j+1].level)
                    mapa[i][j].hDiff += 0x80;
            }
            if (mapa[i][j].isRiver)
			{
				if (i > 0 && (mapa[i-1][j].isRiver || mapa[i-1][j].level<0))
                    mapa[i][j].river += 0x1;
                if (j > 0 && (mapa[i][j-1].isRiver || mapa[i][j-1].level<0))
                    mapa[i][j].river += 0x2;
                if (i < mapSize.x - 1 && (mapa[i+1][j].isRiver || mapa[i+1][j].level<0))
                    mapa[i][j].river += 0x4;
                if (j < mapSize.y - 1 && (mapa[i][j+1].isRiver || mapa[i][j+1].level<0))
                    mapa[i][j].river += 0x8;
                /*if (i > 0 && j > 0 && (mapa[i-1][j-1].isRiver || mapa[i-1][j-1].level<0))
                    mapa[i][j].river += 0x10;
                if (i < mapSize.x - 1 && j>0 && (mapa[i+1][j-1].isRiver|| mapa[i+1][j-1].level<0))
                    mapa[i][j].river += 0x20;
                if (i < mapSize.x - 1 && j < mapSize.y - 1 && (mapa[i+1][j+1].isRiver|| mapa[i+1][j+1].level<0))
                    mapa[i][j].river += 0x40;
                if (i > 0 && j < mapSize.y - 1 && (mapa[i-1][j+1].isRiver || mapa[i-1][j+1].level<0))
                    mapa[i][j].river += 0x80;*/
			}
            /*else if(!mapa[i][j].isRiver)
            {
                if (i > 0 && mapa[i-1][j].isRiver)
                    mapa[i][j].river += 0x1;
                if (j > 0 && mapa[i][j-1].isRiver)
                    mapa[i][j].river += 0x2;
                if (i < mapSize.x - 1 && mapa[i+1][j].isRiver)
                    mapa[i][j].river += 0x4;
                if (j < mapSize.y - 1 && mapa[i][j+1].isRiver)
                    mapa[i][j].river += 0x8;
                if (i > 0 && j > 0 && mapa[i-1][j-1].isRiver)
                    mapa[i][j].river += 0x10;
                if (i < mapSize.x - 1 && j>0 && mapa[i+1][j-1].isRiver)
                    mapa[i][j].river += 0x20;
                if (i < mapSize.x - 1 && j < mapSize.y - 1 && mapa[i+1][j+1].isRiver)
                    mapa[i][j].river += 0x40;
                if (i > 0 && j < mapSize.y - 1 && mapa[i-1][j+1].isRiver)
                    mapa[i][j].river += 0x80;
            }*/
            mapa[i][j].UpdateSprites();
            mapa[i][j].setPosition(i*tileSize,j*tileSize);
            sf::Color tmpColor;
            if(mapa[i][j].level==-2)
                miniMapBase.setPixel(i,j,sf::Color(0,0,127));
            else if(mapa[i][j].level==-1)
                miniMapBase.setPixel(i,j,sf::Color(0,0,255));
            else if(mapa[i][j].isRiver)
                miniMapBase.setPixel(i,j,sf::Color(0,0,127));
            else if(mapa[i][j].forestType)
                miniMapBase.setPixel(i,j,sf::Color(0,127,0));
            else switch(mapa[i][j].terrainType)
            {
            case grass:
                miniMapBase.setPixel(i,j,sf::Color(0,255,0));
                break;
            case sand:
                miniMapBase.setPixel(i,j,sf::Color(255,255,0));
                break;
            case rocky:
                miniMapBase.setPixel(i,j,sf::Color(127,127,127));
                break;
            case snow:
                miniMapBase.setPixel(i,j,sf::Color(255,255,255));
                break;
            default:
                miniMapBase.setPixel(i,j,sf::Color(255,0,255));
            }
        }
    }
    for(int i=0;i<getSize().x;i++)
    {
        for(int j=0;j<getSize().y;j++)
        {
            isVis[i][j]=0;
            everVis[i][j]=0;
        }
    }
    for(std::size_t i=0u;i<players.size();i++)
        actualizeVisibility(i, state);
    miniMapTexture.loadFromImage(miniMap);
    miniMapTexture.setSmooth(false);
    miniMapSprite.setTexture(miniMapTexture);
    miniMapSprite.setTextureRect({0,0,getSize().x,getSize().y});
    return EXIT_SUCCESS;
}

bool Map::generate(my_vector2i mSize, const State& state)
{
    int tmpSQD=std::max(mSize.x,mSize.y);
    //diamond-square alogorithm
    for(int i=1;i<10e3;i*=2)
    {
        if(i+1>=tmpSQD) {tmpSQD=i+1;break;}
    }
    //maxSize
    float height[512][512];
    //setting corners (range: -15,20)
    height[0][0]=rand(-15.f,15.f); //-2;8
    height[tmpSQD-1][0]=rand(-15.f,15.f);
    height[0][tmpSQD-1]=rand(-15.f,15.f);
    height[tmpSQD-1][tmpSQD-1]=rand(-15.f,15.f);
    height[tmpSQD-1][tmpSQD-1]=rand(-15.f,15.f);
    for(int i=tmpSQD/2;i>0;i/=2)
    {
        //diamond (left corner of part)
        //X-X
        //-O-
        //X-X
        for(int j=0;j<=tmpSQD-2*i;j+=2*i)
            for(int k=0;k<=tmpSQD-2*i;k+=2*i)
            {
                height[j+i][k+i]=rand((height[j][k]+height[j+2*i][k]+height[j][k+2*i]+height[j+2*i][k+2*i])/4.f,.5f*i);
            }
        //square
        //-X-
        //XOX
        //-X-
        for(int j=0;j<tmpSQD;j+=i)
            for(int k=i*(1-(j/i)%2);k<tmpSQD;k+=2*i)
            {
                if(j==0 && k==0) height[j][k]=(height[j+i][k]+height[j][k+i])*.5f;
                else if(j==0) height[j][k]=(height[j+i][k]+height[j][k-i]+height[j][k+i])/3.f;
                else if (k==0) height[j][k]=(height[j+i][k]+height[j-i][k]+height[j][k+i])/3.f;
                else if(j>=tmpSQD-1 && k>=tmpSQD-1) height[j][k]=(height[j-i][k]+height[j][k-i])*.5f;
                else if(j>=tmpSQD-1) height[j][k]=(height[j-i][k]+height[j][k+i]+height[j][k-i])/3.f;
                else if (k>=tmpSQD-1) height[j][k]=(height[j-i][k]+height[j+i][k]+height[j][k-i])/3.f;
                else height[j][k]=(height[j-i][k]+height[j+i][k]+height[j][k-i]+height[j][k+i])*.25f;
                height[j][k]=rand(height[j][k],i*.5f*.7071067811865475244f);
            }
    }
    //normalization
    {
        float maxHeight=INT_MIN;
        float minHeight=INT_MAX;
        for(int i=0;i<mSize.x;i++)
        {
            for(int j=0;j<mSize.y;j++)
            {
                if(height[i][j]<minHeight)
                    minHeight=height[i][j];
                if(height[i][j]>maxHeight)
                    maxHeight=height[i][j];
            }
        }
        for(int i=0;i<mSize.x;i++)
        {
            for(int j=0;j<mSize.y;j++)
            {
                height[i][j]=(height[i][j]-minHeight)/(maxHeight-minHeight);
            }
        }
    }
    //interpreting DSA heightmap
    int fullDesertDistance=275*std::min(1.,std::sqrt(mSize.x*mSize.y)/160);///275
    int beginDesertDistance=225*std::min(1.,std::sqrt(mSize.x*mSize.y)/160);///225
    int fullSteppeDistance=170*std::min(1.,std::sqrt(mSize.x*mSize.y)/160);///170
    int beginSteppeDistance=120*std::min(1.,std::sqrt(mSize.x*mSize.y)/160);///120
    int maxHeight=std::min(10,getSize().x/16);
    int snowHeight=9;
    int rockHeight=7;//no trees from that height
    int taigaHeight=5;
    float coastLevel=0.25f; //0 - minValue, 1-maxValue
	if(init(mSize.x,mSize.y))
    {logStr+="[ERROR]Map memory initialization error\n"; return EXIT_FAILURE;}
    logStr+="Map memory initialized successfully\n";
    for(int i=0;i<mSize.x;i++)
    {
        for(int j=0;j<mSize.y;j++)
        {
            if(height[i][j]>=coastLevel)
            {
                mapa[i][j].level=(int)round((height[i][j]-coastLevel)/(1.-coastLevel)*maxHeight);
            }
            else if(height[i][j]>coastLevel/2.f)
                mapa[i][j].level=-1;
            else
                mapa[i][j].level=-2;
            mapa[i][j].isRiver=false;
            mapa[i][j].forestType=0;
        }
    }
    //generating beaches and setting desertTab
    short desertTab[mSize.x][mSize.y]; //diag-7, norm-5
    for(int i=0;i<mSize.x;i++)
    {
        for(int j=0;j<mSize.y;j++)
        {
            if(mapa[i][j].level==0 && ((i>0 && mapa[i-1][j].level<0) || (i<mSize.x-1 && mapa[i+1][j].level<0)
                || (j>0 && mapa[i][j-1].level<0) || (j<mSize.y-1 && mapa[i][j+1].level<0)))
                mapa[i][j].terrainType=sand;
            if(mapa[i][j].level<0)
                desertTab[i][j]=0;
            else
                desertTab[i][j]=SHRT_MAX;
        }
    }
    for(int k=0;k<=fullDesertDistance;k++)
    {
        for(int i=0;i<getSize().x;i++)
        {
            for(int j=0;j<getSize().y;j++)
            {
                if(desertTab[i][j]==k)
                {
                    if(i>0 && j>0 && desertTab[i-1][j-1]>desertTab[i][j]+7)
                    {
                        desertTab[i-1][j-1]=desertTab[i][j]+7;
                    }
                    if(i>0 && desertTab[i-1][j]>desertTab[i][j]+5)
                    {
                        desertTab[i-1][j]=desertTab[i][j]+5;
                    }
                    if(i>0 && j<getSize().y-1 && desertTab[i-1][j+1]>desertTab[i][j]+7)
                    {
                        desertTab[i-1][j+1]=desertTab[i][j]+7;
                    }
                    if(j>0 && desertTab[i][j-1]>desertTab[i][j]+5)
                    {
                        desertTab[i][j-1]=desertTab[i][j]+5;
                    }
                    if(j<getSize().y-1 && desertTab[i][j+1]>desertTab[i][j]+5)
                    {
                        desertTab[i][j+1]=desertTab[i][j]+5;
                    }
                    if(i<getSize().x-1 && j>0 && desertTab[i+1][j-1]>desertTab[i][j]+7)
                    {
                        desertTab[i+1][j-1]=desertTab[i][j]+7;
                    }
                    if(i<getSize().x-1 && desertTab[i+1][j]>desertTab[i][j]+5)
                    {
                        desertTab[i+1][j]=desertTab[i][j]+5;
                    }
                    if(i<getSize().x-1 && j<getSize().y-1 && desertTab[i+1][j+1]>desertTab[i][j]+7)
                    {
                        desertTab[i+1][j+1]=desertTab[i][j]+7;
                    }
                }
            }
        }
    }
    //setting terrain types
    for(int i=0;i<mSize.x;i++)
    {
        for(int j=0;j<mSize.y;j++)
        {
            if(mapa[i][j].terrainType==sand) continue; ///set as beach
            else if(mapa[i][j].level>=snowHeight) mapa[i][j].terrainType=snow;
            else if(mapa[i][j].level>=rockHeight) mapa[i][j].terrainType=rocky;
            else if(desertTab[i][j]>fullDesertDistance) mapa[i][j].terrainType=sand;
            else if(desertTab[i][j]<beginDesertDistance) mapa[i][j].terrainType=grass;
            else
            {
                float r = static_cast <float> (rand()) / static_cast <float> (RAND_MAX); //generating from 0 to 1
                if(r*(fullDesertDistance-beginDesertDistance)>desertTab[i][j]-beginDesertDistance)
                    mapa[i][j].terrainType=grass;
                else
                    mapa[i][j].terrainType=sand;
            }
        }
    }
    //generating rivers
    std::vector<std::vector<int> > costs; //0-lower, 3-the same
    for(int i=0;i<mSize.x;i++)
    {
        std::vector<int> tmpVectoiana;
        for(int j=0;j<mSize.y;j++)
            tmpVectoiana.push_back(INT_MAX/2);
        costs.push_back(tmpVectoiana);
    }
    std::list<my_vector2i> visiting; //cost: 0-lower, 1-the same
    for(int kkk=0;kkk<std::max(100,mSize.x*mSize.y/256);kkk++)
    {
        int bX=rand()%mSize.x;
        int bY=rand()%mSize.y;
        if(mapa[bX][bY].level<=0 || mapa[bX][bY].isRiver || (mapa[bX][bY].level<rockHeight && desertTab[bX][bY]>beginDesertDistance)) continue;
        //Dijkstra road (finding way of river)
        for(int i=0;i<mSize.x;i++)
            for(int j=0;j<mSize.y;j++)
                costs[i][j]=INT_MAX/2;
        costs[bX][bY]=0;
        visiting.push_back(my_vector2i(bX,bY));
        int dist=INT_MAX/2; my_vector2i tarPos(-1,-1);
        while(visiting.size()>0)
        {
            if(costs[visiting.begin()->x][visiting.begin()->y]<=dist)
            {
                short x=visiting.begin()->x; short y=visiting.begin()->y;
                for(int j=0;j<9;j++)
                {
                    short rnd=rand()%30;
                    if(j==4) continue;
                    else if(j%2==0) continue;
                    else if(j%3==0 && x==0) continue;
                    else if(j<3 && y==0) continue;
                    else if(j%3==2 && x==mSize.x-1) continue;
                    else if(j>5 && y==mSize.y-1) continue;
                    else if(dist>costs[x][y] && (mapa[x-1+j%3][y-1+j/3].level<0 || (mapa[x-1+j%3][y-1+j/3].isRiver && mapa[x-1+j%3][y-1+j/3].level<=mapa[x][y].level)))
                    {
                        dist=costs[x][y];
                        tarPos=*visiting.begin();
                    }
                    else if(costs[x-1+j%3][y-1+j/3]>costs[x][y]+1)
                    {
                        if(mapa[x-1+j%3][y-1+j/3].level<mapa[x][y].level)
                        {
                            costs[x-1+j%3][y-1+j/3]=costs[x][y]+1;
                            visiting.push_back(my_vector2i(x-1+j%3,y-1+j/3));
                        }
                        else if(mapa[x-1+j%3][y-1+j/3].level==mapa[x][y].level && costs[x-1+j%3][y-1+j/3]>costs[x][y]+rnd+4-j%2)
                        {
                            costs[x-1+j%3][y-1+j/3]=costs[x][y]+rnd+4-j%2;
                            visiting.push_back(my_vector2i(x-1+j%3,y-1+j/3));
                        }
                    }
                }
            }
            visiting.pop_front();
        }
        //making river
        if(tarPos==my_vector2i(-1,-1)) continue;
        mapa[bX][bY].isRiver=true;
        while(tarPos!=my_vector2i(bX,bY))
        {
            int mI=4;
            mapa[tarPos.x][tarPos.y].isRiver=true;
            for(int i=0;i<9;i++)
                if(i==4) continue;
                else if(i%2==0) continue;
                else if(i%3==0 && tarPos.x==0) continue;
                else if(i<3 && tarPos.y==0) continue;
                else if(i%3==2 && tarPos.x==mSize.x-1) continue;
                else if(i>5 && tarPos.y==mSize.y-1) continue;
                else if (costs[tarPos.x-1+i%3][tarPos.y-1+i/3]<costs[tarPos.x-1+mI%3][tarPos.y-1+mI/3] && mapa[tarPos.x-1+i%3][tarPos.y-1+i/3].level>=mapa[tarPos.x][tarPos.y].level) mI=i;
            if(mI==4) break;
            tarPos=my_vector2i(tarPos.x-1+mI%3,tarPos.y-1+mI/3);
        }
    }
    //RANDOM resources generator
    for(int i=0;i<getSize().x;i++)
    {
        for(int j=0;j<getSize().y;j++)
        {
            if(mapa[i][j].level<0 || mapa[i][j].isRiver || (mapa[i][j].level==0 && mapa[i][j].terrainType==sand)) continue;
            int ran=rand()%1000;
            if(ran<30) mapa[i][j].resource=stone;
            else if(ran<45) mapa[i][j].resource=iron;
            else if(ran<60)
            {
                if(mapa[i][j].level<6 && mapa[i][j].terrainType!=sand)
                    mapa[i][j].resource=horses;
                //else
                    //mapa[i][j].resource=stone;
            }
            else if(ran<65) mapa[i][j].resource=gold;
            else if(ran<70) mapa[i][j].resource=jewels;
            else if(ran<770)
            {
                if(mapa[i][j].level<taigaHeight && desertTab[i][j]<fullSteppeDistance)
                {
                    float r = static_cast <float> (rand()) / static_cast <float> (RAND_MAX); //generating from 0 to 1
                    if(desertTab[i][j]<beginSteppeDistance || r*(fullSteppeDistance-beginSteppeDistance)>desertTab[i][j]-beginSteppeDistance)
                    {
                        mapa[i][j].forestType=1; mapa[i][j].resource=wood;
                    }
                }
                else if(mapa[i][j].level>=taigaHeight)
                {
                    if(mapa[i][j].level<rockHeight && desertTab[i][j]<beginDesertDistance) {mapa[i][j].forestType=2; mapa[i][j].resource=wood;}
                    else if(mapa[i][j].level>=rockHeight && ran<370) mapa[i][j].resource=stone;
                }
                else mapa[i][j].resource=0;
            }
            else mapa[i][j].resource=0;
        }
    }
    int maxInd=0;
    //setting players
    my_vector2i playerIPos;
    my_vector2i prevPlayerPos[10];
    for(int i=0;i<(int)players.size();i++)
    {
        playerIPos=my_vector2i(rand()%(getSize().x-12)+5,rand()%(getSize().y-12)+5);
        if(mapa[playerIPos.x][playerIPos.y].level<=0/* || mapa[playerIPos.x-1][playerIPos.y].level<=0|| mapa[playerIPos.x][playerIPos.y-1].level<=0|| mapa[playerIPos.x-1][playerIPos.y-1].level<=0*/)
        {
            i--; maxInd++;
        }
        else
        {
            bool tmp517=false;
            for(int j=0;j<i;j++)
            {
                if(sqrDist(playerIPos,prevPlayerPos[j])<400.f)
                {tmp517=true;break;}
            }
            if(tmp517) {i--;maxInd++;break;}
            if(typeid(*data->getUnitType(0))==typeid(officer))
            {
                units.push_back(Unit(data->getUnitType(0)));
                units.back().commandedUnits.push_back(std::make_pair(1,3));
                units.back().commandedUnits.push_back(std::make_pair(2,5));
                units.back().commandedUnits.push_back(std::make_pair(1,3));
            }
            units.back().setPosition(playerIPos.x*tileSize,playerIPos.y*tileSize);
            units.back().setOwner(i);
            units.push_back(Unit(data->getUnitType(3)));
            units.back().setPosition((playerIPos.x+1)*tileSize,playerIPos.y*tileSize);
            units.back().setOwner(i);
            /*units.push_back(Unit(unitTypes[1]));
            units.back().setPosition(playerIPos.x*tileSize,(playerIPos.y-1)*tileSize);
            units.back().setOwner(i); units.back().kount=6;
            units.push_back(Unit(unitTypes[2]));
            units.back().setPosition((playerIPos.x-1)*tileSize,(playerIPos.y-1)*tileSize);
            units.back().setOwner(i); units.back().kount=6;*/
            prevPlayerPos[i]=playerIPos;
        }
        if(maxInd>20000) break;
    }

    if(postInit(state)) return EXIT_FAILURE;
    actualizeMiniMap(0);
    return EXIT_SUCCESS;
}
float rand(float rCent, float rDiff)
{
    if(rCent+rDiff>20.f && rCent-rDiff<-15.f) {rCent=2.5f;rDiff=17.5f;}
    else if(rCent+rDiff>20.f) rCent=20.f-rDiff*.5f;
    else if(rCent-rDiff<-15.f) rCent=-15.f+rDiff*.5f;
    return ((float)rand()/(float)RAND_MAX)*rDiff+rCent-rDiff*.5f;
}

void Map::setRoad(my_vector2i pos, int8_t roadStage)
{
    if(roadStage>4) return;
    mapa[pos.x][pos.y].setRoadStage(roadStage);
    //bool isOnlyOne=true;
    for(int i=0;i<9;i++)
    {
        if(i==4) continue;
        else if(i<2 && pos.x==0) continue;
        else if(i>5 && pos.x==mapSize.x-1) continue;
        else if(i%3 && pos.y==0) continue;
        else if(i%3==2 && pos.y==mapSize.y-1) continue;
        if(roadStage>0)
        {
            if(mapa[pos.x-1+i/3][pos.y-1+i%3].getRoadStage()>0)
            {
                //isOnlyOne=false;
                switch(i)
                {
                case 0:
                    mapa[pos.x-1+i/3][pos.y-1+i%3].setRoadTexture(6);
                    mapa[pos.x][pos.y].setRoadTexture(4); break;
                case 1:
                    mapa[pos.x-1+i/3][pos.y-1+i%3].setRoadTexture(2);
                    mapa[pos.x][pos.y].setRoadTexture(0); break;
                case 2:
                    mapa[pos.x-1+i/3][pos.y-1+i%3].setRoadTexture(5);
                    mapa[pos.x][pos.y].setRoadTexture(7); break;
                case 3:
                    mapa[pos.x-1+i/3][pos.y-1+i%3].setRoadTexture(3);
                    mapa[pos.x][pos.y].setRoadTexture(1); break;
                case 5:
                    mapa[pos.x-1+i/3][pos.y-1+i%3].setRoadTexture(1);
                    mapa[pos.x][pos.y].setRoadTexture(3); break;
                case 6:
                    mapa[pos.x-1+i/3][pos.y-1+i%3].setRoadTexture(7);
                    mapa[pos.x][pos.y].setRoadTexture(5); break;
                case 7:
                    mapa[pos.x-1+i/3][pos.y-1+i%3].setRoadTexture(0);
                    mapa[pos.x][pos.y].setRoadTexture(2); break;
                case 8:
                    mapa[pos.x-1+i/3][pos.y-1+i%3].setRoadTexture(4);
                    mapa[pos.x][pos.y].setRoadTexture(6); break;
                }
            }
        }
        else
        {
            //isOnlyOne=false;
            switch(i)
            {
            case 0:
                mapa[pos.x-1+i/3][pos.y-1+i%3].unsetRoadTexture(6); break;
            case 1:
                mapa[pos.x-1+i/3][pos.y-1+i%3].unsetRoadTexture(3); break;
            case 2:
                mapa[pos.x-1+i/3][pos.y-1+i%3].unsetRoadTexture(7); break;
            case 3:
                mapa[pos.x-1+i/3][pos.y-1+i%3].unsetRoadTexture(2); break;
            case 5:
                mapa[pos.x-1+i/3][pos.y-1+i%3].unsetRoadTexture(0); break;
            case 6:
                mapa[pos.x-1+i/3][pos.y-1+i%3].unsetRoadTexture(5); break;
            case 7:
                mapa[pos.x-1+i/3][pos.y-1+i%3].unsetRoadTexture(1); break;
            case 8:
                mapa[pos.x-1+i/3][pos.y-1+i%3].unsetRoadTexture(4); break;
            }
        }
        /*if(isOnlyOne)
        {
            mapa[pos.x][pos.y].setRoadTexture(0);
        }*/
    }
}

bool Map::saveToFile(const State& state, string name)
{
    bool isMapSave = name.substr(0,4)=="maps";
    std::fstream file;
    file.open(name, std::ios::out | std::ios::binary);
    if(!file.good())
    {
        return EXIT_FAILURE;
    }
    //map info
    ///maximum size of map: 65536
    ///maximum num of players: 64 (6 bits)
    ///maximum number of turn: 1024 (10 bits)
    write(file,mapSize.x%0x10000);
    write(file,mapSize.y%0x10000);
    //file << std::binary/*std::hex*/ << mapSize.x%0x10000 << /*" " << std::hex <<*/ mapSize.y%0x10000 /*<< " "*/;
    uint16_t tmp1=0x0;
    tmp1+=(state.actualPlayer%0x40)+((state.actualTurn%0x400)<<6);
    //file /*<< std::hex*/ << tmp1 /*<< " "*/;
    write(file,tmp1);
    /*file << mapSize.x << " " << mapSize.y << "\n";
    if(!isMapSave) file << state.actualPlayer << " " << state.actualTurn << "\n";*/
    std::string tmpStr;
    for(int i=0;i<mapSize.y;i++)
    {
        for(int j=0;j<mapSize.x;j++)
        {
            ///bit order (counting from least significiant bit:
            ///if somewhere is 0, it means that no that
            ///0-is 16 (0)or 32 (1) bit
            ///1-beginning of level (6 bits)
            ///7-beginning of terrain type (5 bits)
            ///12-if it's river (1 bit)
            ///13-beginning of forest type (3 bits)
            tmp1=0x0;
            tmp1+=((mapa[i][j].level+2)%0x40)<<1;
            tmp1+=((mapa[i][j].terrainType)%0x20)<<7;
            tmp1+=(mapa[i][j].isRiver%0x02)<<12;
            tmp1+=(mapa[i][j].forestType%0x08)<<13;
            uint16_t tmp2=0x0;
            ///second bit order:
            ///0-beginning of resource type (5 bits)
            ///5-beginning of roadLevel (4 bits)
            ///9-16 unused
            tmp2+=((mapa[i][j].resource)%0x20);
            if(mapa[i][j].getRoadStage()>0)
            {
                tmp2+=((mapa[i][j].getRoadStage()%0x10)<<5);
            }
            if(tmp2==0x0)
            {
                write(file,tmp1);
                //file << /*std::hex <<*/ tmp1;
            }
            else
            {
                tmp1++;
                write(file,tmp1);
                write(file,tmp2);
                //file << /*std::hex <<*/ tmp1 << /*" " << std::hex <<*/ tmp2;
            }
            //file << " ";
        }
    }
    //players info
    write(file,players.size());
    //file << playersNum /*<< "\n"*/;
    //tile visibility (for each player)
    for(std::size_t l=0u;l<players.size();l++)
    {
        for(int i=0;i<mapSize.y;i++)
        {
            for(int j=0;j<mapSize.x;j+=16)
            {
                tmp1=0x0;
                for(int k=0;k<std::min(16,mapSize.x-j);k++)
                {
                    if(wasEverVisible(i,j+k,l))
                        tmp1+=0x1<<k;
                }
                write(file,tmp1);
                //file << tmp1;
                /*if(j+16>=mapSize.x && i==mapSize.y-1)
                    file << "\n";
                else
                    file << " ";*/
            }
        }
    }
    ///information of each player
    for(std::size_t i=0u;i<players.size();i++)
    {
        int tmp852;
        for(int j=0;j<RESOURCE_TYPES_NUMBER;j++)
        {
            tmp852 = players[i].getResource(j+gold);
            write(file,tmp852>>16);
            write(file,tmp852%0x10000);
            //file << uint16_t(tmp852>>16) << /*" " <<*/ int16_t(tmp852%0x10000) /*<< " "*/;
        }
        tmp852 = players[i].playerColor.toInteger();
        tmp1 = uint16_t(tmp852>>16);
        //file << tmp1 /*<< " "*/;
        write(file,tmp1);
        //sf::String tmpName=players[i].name;
        //RemoveSpaces(tmpName);
        tmp1=uint16_t(players[i].name.getSize()%0x100);
        tmp1+=uint16_t(((tmp852>>8)%0x100)<<8);
        write(file,tmp1);
        //file << tmp1 /*<< " "*/;/* << tmpName.toAnsiString() << "\n";*/
        saveText(file,players[i].name);
    }
    //units info
    ///there can be ONLY one unit on tile
    write(file,units.size());
    //file << units.size() /*<< "\n"*/;
    for(std::size_t i=0u;i<units.size();i++)
    {
        uint16_t j;
        for(j=0;j<data->getUnitTypeSize();j++)
        {
            if(data->getUnitType(j)==units[i].type)
                break;
        }
        write(file,j);
        write(file,mapToTile(units[i].getPosition()).x);
        write(file,mapToTile(units[i].getPosition()).y);
        write(file,(uint16_t)units[i].owner);
        if(!isMapSave)
        {
            write(file,units[i].mp);
        }
        if(typeid(*(units[i].type.get()))==typeid(officer))
		{
		    ///max number of commandedUnits - 16
		    ///max count of units in a stack - 32767 (2^15-1)
		    ///max count of unit types - 512 (2^9)
            write(file,units[i].commandedUnits.size());
		    //file << /*" " <<*/ units[i].commandedUnits.size();
            for(std::size_t j=0u;j<units[i].commandedUnits.size();j++)
            {
                write(file,units[i].commandedUnits[j].first);
                write(file,units[i].commandedUnits[j].second);
            }
		}
        //file << "\n";
    }
    //improvement info
    //write(file,buildings.size());
    //file << buildings.size()/* << "\n"*/;
    /*for(int i=0;i<buildings.size();i++)
    {
        for(tmp1=0;tmp1<buildingTypes.size();tmp1++)
        {
            if(buildingTypes[tmp1]==buildings[i].type)
                break;
        }
        write(file,tmp1);
        write(file,mapToTile(buildings[i].getPosition()).x);
        write(file,mapToTile(buildings[i].getPosition()).y);
        //file << tmp1 << " ";
        //file << mapToTile(buildings[i].getPosition()).x << " ";
        //file << mapToTile(buildings[i].getPosition()).y << " ";
        tmp1=0x0;
        write(file,(uint16_t(buildings[i].stage)<<8)+uint16_t(buildings[i].owner));
        //file << (int16_t(buildings[i].stage)<<8)+int16_t(buildings[i].owner);
        if(!isMapSave)
        {
            write(file,buildings[i].lastManCost);
            //file << buildings[i].lastManCost;
        }
        //file << "\n";
        //file << (int16_t)buildings[i].stage << " ";
        //file << (int16_t)buildings[i].owner << "\n";
    }*/
    //city info - under construction (city class not finished)
    write(file,cities.size());
    //file << cities.size()/* << "\n"*/;
    for(std::size_t i=0u;i<cities.size();i++)
    {
        //sf::String tmpName=cities[i].getName();
        //RemoveSpaces(tmpName);
        my_vector2i tile=mapToTile(cities[i].getPosition());
        tmp1=(uint16_t(cities[i].owner)<<8)+uint16_t(cities[i].getName().getSize()%0x100);

        write(file,(uint16_t)cities[i].getStage());
        write(file,tile.x);
        write(file,tile.y);
        ///building info
        write(file,cities[i].getBuildingNum());
        for(int j=0;j<cities[i].getBuildingNum();j++)
        {
            for(tmp1=0;tmp1<data->getBuildingTypesSize();tmp1++)
            {
                if(data->getBuildingType(tmp1).get()==cities[i].getBuilding(j).type.get())
                    break;
            }
            write(file,tmp1);
            write(file,mapToTile(cities[i].getBuilding(j).getPosition()).x);
            write(file,mapToTile(cities[i].getBuilding(j).getPosition()).y);
            tmp1=0x0;
            write(file,uint16_t(cities[i].getBuilding(j).stage));
            if(!isMapSave)
            {
                write(file,cities[i].getBuilding(j).lastManCost);
                //file << buildings[i].lastManCost;
            }
        }
        //file << (short)cities[i].getStage() <<
        ///*" " <<*/ tile.x << /*" " <<*/ tile.y << /*" "
        //<<*/ cities[i].getPopulation() << /*" " <<*/ tmp1 /*<< " "*/;
        saveText(file,cities[i].getName());
    }
    file.close();
    return EXIT_SUCCESS;
}

bool Map::loadFromFile(State& state, string name)
{
    bool isMapSave = name.substr(0,4)=="maps";
    std::fstream file;
    file.open(name, std::ios::in | std::ios::binary);
    if(!file.good())
    {
        return EXIT_FAILURE;
    }
    mapSize.x=load(file);
    mapSize.y=load(file);
    //file >> /*std::hex >>*/ mapSize.x /*>> std::hex*/ >> mapSize.y;
	if(init(mapSize.x,mapSize.y))
    {logStr+="[ERROR]Map memory initialization error\n"; return EXIT_FAILURE;}
    logStr+="Map memory initialized successfully\n";
    std::cout << "Loading tiles...\n";
    //map info
    uint16_t tmp836;
    //file /*>> std::hex*/ >> tmp836;
    tmp836=load(file);
    ///Maximum number of players: 64
    ///Maximum number of turns: 1024
    state.actualPlayer=tmp836%0x40;
    state.actualTurn=(tmp836>>6);
    //if(!isMapSave) file >> state.actualPlayer >> state.actualTurn;
    for(int i=0;i<mapSize.y;i++)
    {
        for(int j=0;j<mapSize.x;j++)
        {
            try
            {
                tmp836=load(file);
                //file >> /*std::hex >>*/ tmp836;
                ///bit order (counting from least significiant bit:
                ///if somewhere is 0, it means that no that
                ///0-is 16 (0)or 32 (1) bit
                ///1-beginning of level (6 bits)
                ///7-beginning of terrain type (5 bits)
                ///12-if it's river (1 bit)
                ///13-beginning of forest type (3 bits)

                mapa[i][j].level=((tmp836>>1)%0x40)-2; ///bits
                mapa[i][j].terrainType=static_cast<mainMegaIDs>((tmp836>>7)%0x20);
                mapa[i][j].isRiver=((tmp836>>12)%0x2);
                mapa[i][j].forestType=((tmp836)>>13)%0x8;
                if(tmp836%2)
                {
                    tmp836=load(file);
                    ///second bit order:
                    ///0-beginning of resource type (5 bits)
                    ///5-beginning of roadLevel (4 bits)
                    ///9-16 unused
                    mapa[i][j].resource=tmp836%0x20;
                    if((tmp836>>5)%0x10>0)
                        setRoad(my_vector2i(i,j),(tmp836>>5)%0x10);

                }
            }
            catch(...)
            {
                logStr+="Bad file!\n";
                return EXIT_FAILURE;
            }
        }
    }
    ///player info - max.64
    ///max. length of name - 1024
    short playersNum;
    playersNum=load(file);
    players.clear();
    for(int i=0;i<playersNum;i++)
        players.push_back(Player());
    std::cout << "Loading player visibility...\n";
    //tile visibility (for each player)
    for(int l=0;l<playersNum;l++)
    {
        //std::cout << l << " ";
        for(int i=0;i<mapSize.y;i++)
        {
            for(int j=0;j<mapSize.x;j+=16)
            {
                tmp836=load(file);
                //file >> tmp836;
                for(int k=0;k<std::min(16,mapSize.x-j);k++)
                {
                    //if(wasEverVisible(i,j+k,l))
                    //    tmp1+=0x1<<l;
                    everVis[i][j+k]|=((tmp836>>k)%2)<<l;
                }
            }
        }
        //std::cout << l << " ";
    }
    std::cout << "Loading player info...\n";
    ///each player info
    for(int i=0;i<playersNum;i++)
    {
        uint16_t tmp837;
        ///Resource Quantity - you can only have max. 2^32-1 pieces of resource
        for(int j=0;j<RESOURCE_TYPES_NUMBER;j++)
        {
            tmp836=load(file);
            tmp837=load(file);
            players[i].resourceQuantity[j]=(((uint32_t)tmp836)<<16)+(uint32_t)tmp837;
        }
        ///color & player length
        ///24 bits for color
        ///8 last bits - length of player name (max size - 255)
        tmp836=load(file);
        tmp837=load(file);
        players[i].playerColor=sf::Color(tmp836>>8,tmp836%0x000f,tmp837>>8);
        players[i].name=loadText(file,tmp837%0x100);
    }
    //units info
    std::cout << "Loading units...\n";
    uint16_t tmpSize;
    tmpSize=load(file);
    for(int i=0;i<tmpSize;i++)
    {
        uint16_t x, y, j;
        try
        {
            j=load(file);
            units.push_back(Unit(data->getUnitType(j)));
            x=load(file);
            y=load(file);
            units.back().setPosition(tileToMap(x,y));
            units.back().kount=1;
            j=load(file);
            //file >> j;
            units.back().setOwner((int8_t)j);
            if(!isMapSave)
            {
                units.back().mp=load(file);
            }
            if(typeid(*(units.back().type))==typeid(officer))
            {
                short cSize;
                int f,s;
                cSize=load(file);
                for(int j=0;j<cSize;j++)
                {
                    f=load(file);
                    s=load(file);
                    //file >> f >> s;
                    units.back().commandedUnits.push_back(std::make_pair(f,s));
                }
            }
		}
        catch(...)
        {
            logStr+="Bad file!\n";
            return EXIT_FAILURE;
        }
    }
    //building info
    /*tmpSize=load(file);
    for(int i=0;i<tmpSize;i++)
    {
        uint16_t x,y,j;
        try
        {
            j=load(file);
            x=load(file);
            y=load(file);
            //file >> j >> x >> y;
            buildings.push_back(buildingTypes[j]);
            buildings[i].setPosition(tileToMap(x,y));
            j=load(file);
            //file >> j;
            buildings[i].setStage(j);
            j=load(file);
            //file >> j;
            buildings[i].setOwner(j);
            if(!isMapSave)
            {
                buildings[i].lastManCost=load(file);
                //file >> buildings[i].lastManCost;
            }
        }
        catch(...)
        {
            logStr+="Bad file!\n";
            return EXIT_FAILURE;
        }
    }*/
    //city info
    std::cout << "Loading cities...\n";
    tmpSize=load(file);
    //file >> tmpSize;
    for(int i=0;i<tmpSize;i++)
    {
        short level/*,tmpOwner*/, x, y;
        int pop;
        try
        {
            //std::string tmpName;
            //file >> tmpName;
            //AddSpaces(tmpName);
            //file.ignore();
            level=load(file);
            x=load(file);
            y=load(file);
            pop=load(file);
            tmp836=load(file);
            //file >> level >> x >> y >> pop >> tmp836;
            cities.push_back(City(*data,i,state,tmp836>>8,(my_vector2i)tileToMap(x,y),pop));
            cities.back().setStage(level%5);
            ///building info
            write(file,cities[i].getBuildingNum());
            for(int j=0;j<cities[i].getBuildingNum();j++)
            {
                int16_t x,y/*,st*/;
                tmp836=load(file);
                x=load(file);
                y=load(file);
                cities[i].addBuilding(Building(data->getBuildingType(tmp836),i,tileToMap(x,y),state,*data));
                //write(file,uint16_t(cities[i].getBuilding(j).stage));
                if(!isMapSave)
                {
                    cities[i].getBuilding(j).lastManCost=load(file);
                    //file << buildings[i].lastManCost;
                }
            }
            cities.back().setName(loadText(file,tmp836%0x100));
        }
        catch(...)
        {
            logStr+="Bad file!\n";
            return EXIT_FAILURE;
        }
    }

    file.close();
    if(postInit(state)) return EXIT_FAILURE;
    updateOwners();
    actualizeMiniMap(state.actualPlayer);
    actualizeVisibility(state.actualPlayer, state);
    actualizeInfluence();
    return EXIT_SUCCESS;
}
void Map::toIso()
{
    for(int i=0;i<getSize().x;i++)
    {
        for(int j=0;j<getSize().y;j++)
        {
            mapa[i][j].setIsoTextures(true);
            mapa[i][j].setPosition((i-j)*data->getMainMegaTexture().getTileSize()*.5f,(i+j-std::max((int)mapa[i][j].level,-1))*data->getMainMegaTexture().getTileSizeY()*.25f, true);
        }
    }
}
void Map::toTopDown()
{
    for(int i=0;i<getSize().x;i++)
        for(int j=0;j<getSize().y;j++)
        {
            mapa[i][j].setIsoTextures(false);
            mapa[i][j].setPosition(i*data->getMainMegaTexture().getTileSize(),j*data->getMainMegaTexture().getTileSizeY());
        }
    for(std::size_t i=0u;i<units.size();i++)
    {

    }
}
void Map::actualizeVisibility(uint8_t actPlayer, const State& state)
{
    for(int i=0;i<getSize().x;i++)
    {
        for(int j=0;j<getSize().y;j++)
        {
            isVis[i][j]&=~(0x1<<actPlayer);
            mapa[i][j].cityField=-1;
        }
    }
    for(std::size_t k=0u;k<units.size();k++)
    {
        if(!units[k].type || units[k].owner!=actPlayer) continue;
        my_vector2i unitPos=mapToTile(units[k].getPosition(), state.isIso);
        ///now there every unit has visible range 3
        for(int i=std::max(unitPos.x-3,0);i<=std::min(unitPos.x+3,getSize().x-1);i++)
            for(int j=std::max(unitPos.y-3,0);j<=std::min(unitPos.y+3,getSize().y-1);j++)
            {
                if((i-unitPos.x)*(i-unitPos.x)+(j-unitPos.y)*(j-unitPos.y)>12)
                    continue;
                isVis[i][j]|=0x1<<actPlayer;
                everVis[i][j]|=0x1<<actPlayer;
            }
    }
    /*for(int k=0;k<buildings.size();k++)
    {
        if(buildings[k].owner!=actPlayer) continue;
        my_vector2i improPos=mapToTile(buildings[k].getPosition());
        ///now there every impro has visible range 5
        int R=5;
        int sqR=(R+0.5f)*(R+0.5f);
        for(int i=std::max(improPos.x-R,0);i<=std::min(improPos.x+R,getSize().x-1);i++)
            for(int j=std::max(improPos.y-R,0);j<=std::min(improPos.y+R,getSize().y-1);j++)
            {
                if((i-improPos.x)*(i-improPos.x)+(j-improPos.y)*(j-improPos.y)>sqR)
                    continue;
                isVis[i][j]|=0x1<<actPlayer;
                everVis[i][j]|=0x1<<actPlayer;
            }
    }*/
    for(std::size_t k=0u;k<cities.size();k++)
    {
        if(cities[k].owner!=actPlayer) continue;
        my_vector2i cityPos=mapToTile(cities[k].getPosition(), state.isIso);
        ///now there every city has visible range 8
        int R=8;
        int sqR=(R+0.5f)*(R+0.5f);
        int internalR=(cities[k].getStage()+1.5f)*(cities[k].getStage()+1.5f);
        for(int i=std::max(cityPos.x-R,0);i<=std::min(cityPos.x+R,getSize().x-1);i++)
            for(int j=std::max(cityPos.y-R,0);j<=std::min(cityPos.y+R,getSize().y-1);j++)
            {
                if((i-cityPos.x)*(i-cityPos.x)+(j-cityPos.y)*(j-cityPos.y)>sqR)
                    continue;
                isVis[i][j]|=0x1<<actPlayer;
                everVis[i][j]|=0x1<<actPlayer;
                if((i-cityPos.x)*(i-cityPos.x)+(j-cityPos.y)*(j-cityPos.y)<=internalR)
                {
                    mapa[i][j].cityField=k;
                    mapa[i][j].actualizeColor();
                }
            }
        for(int l=0;l<cities[k].getBuildingNum();l++)
        {
            my_vector2i improPos=mapToTile(cities[k].getBuilding(l).getPosition(), state.isIso);
            ///now there every impro has visible range 5
            int R=5;
            int sqR=(R+0.5f)*(R+0.5f);
            for(int i=std::max(improPos.x-R,0);i<=std::min(improPos.x+R,getSize().x-1);i++)
                for(int j=std::max(improPos.y-R,0);j<=std::min(improPos.y+R,getSize().y-1);j++)
                {
                    if((i-improPos.x)*(i-improPos.x)+(j-improPos.y)*(j-improPos.y)>sqR)
                        continue;
                    isVis[i][j]|=0x1<<actPlayer;
                    everVis[i][j]|=0x1<<actPlayer;
                }
        }
    }
    actualizeMiniMap(actPlayer);
}
void Map::actualizeMiniMap(int playerIndex)
{
    for(int i=0;i<getSize().x;i++)
    {
        for(int j=0;j<getSize().y;j++)
        {
            if(wasEverVisible(i,j,playerIndex))
            {
                if(isVisible(i,j,playerIndex))
                    miniMap.setPixel(i,j,miniMapBase.getPixel(i,j));
                else
                    miniMap.setPixel(i,j,miniMapBase.getPixel(i,j)*sf::Color(127,127,127));
            }
            else
                miniMap.setPixel(i,j,sf::Color::Black);
        }
    }
    for(std::size_t k=0u;k<cities.size();k++)
    {
        my_vector2i cityTile = mapToTile(cities[k].getPosition());
        int st=cities[k].getStage()+1;
        for(int i=std::max(0,cityTile.x-st);i<=std::max(getSize().x-1,cityTile.x+st);i++)
        {
            for(int j=std::max(0,cityTile.y-st);j<=std::max(getSize().y-1,cityTile.y+st);j++)
            {
                if(i*i+j*j>(st+0.5f)*(st+0.5f)) continue;
                if(i==0 && j==0) miniMap.setPixel(cityTile.x+i,cityTile.y+j,{127,127,127});
                else miniMap.setPixel(cityTile.x+i,cityTile.y+j,sf::Color::White);
            }
            /*if(i==4) {miniMap.setPixel(cityTile.x,cityTile.y,{127,127,127}); continue;}
            if(cities[k].getPosition().x==0 && i<3) continue;
            if(cities[k].getPosition().x==getSize().x-1 && i>5) continue;
            if(cities[k].getPosition().y==0 && i%3) continue;
            if(cities[k].getPosition().y==getSize().y-1 && i%3==2) continue;
            miniMap.setPixel(cityTile.x+i/3-1,cityTile.y+i%3-1,sf::Color::White; continue);*/
        }

    }
    miniMapTexture.loadFromImage(miniMap);
}
void Map::actualizeInfluence()
{
    ///zeroing previous influence pts
    for(int i=0;i<getSize().x;i++)
        for(int j=0;j<getSize().y;j++)
        {
            mapa[i][j].clearInfluence();
            for(int k=0;k<4;k++)
                mapa[i][j].line[k].setColor(sf::Color::Transparent);
        }
    my_vector2i tmpTile;
    ///(5.5-distance)
    /*for(int k=0;k<buildings.size();k++)
    {
        tmpTile=mapToTile(buildings[k].getPosition());
        int r=3;
        int maxR=(r+0.5f)*(r+0.5f);
        for(int i=std::max(tmpTile.x-r,0);i<=std::min(tmpTile.x+r,getSize().x-1);i++)
        {
            for(int j=std::max(tmpTile.y-r,0);j<=std::min(tmpTile.y+r,getSize().y-1);j++)
            {
                if((i-tmpTile.x)*(i-tmpTile.x)+(j-tmpTile.y)*(j-tmpTile.y)<maxR)
                    mapa[i][j].influencePts[buildings[k].owner]+=maxR-((i-tmpTile.x)*(i-tmpTile.x)+(j-tmpTile.y)*(j-tmpTile.y));
            }
        }
    }*/
    for(std::size_t k=0u;k<cities.size();k++)
    {
        tmpTile=mapToTile(cities[k].getPosition());
        int r=6;
        int maxR=(r+0.5f)*(r+0.5f);
        for(int i=std::max(tmpTile.x-r,0);i<=std::min(tmpTile.x+r,getSize().x-1);i++)
        {
            for(int j=std::max(tmpTile.y-r,0);j<=std::min(tmpTile.y+r,getSize().y-1);j++)
            {
                if((i-tmpTile.x)*(i-tmpTile.x)+(j-tmpTile.y)*(j-tmpTile.y)<=maxR)
                    mapa[i][j].addInfluence(k,5000);
            }
        }
        for(int l=0;l<cities[k].getBuildingNum();l++)
        {
            tmpTile=mapToTile(cities[k].getBuilding(l).getPosition());
            int r=3;
            int maxR=10*r+5;
            for(int i=std::max(tmpTile.x-r,0);i<=std::min(tmpTile.x+r,getSize().x-1);i++)
            {
                for(int j=std::max(tmpTile.y-r,0);j<=std::min(tmpTile.y+r,getSize().y-1);j++)
                {
                    int diff=10*std::round(std::sqrt(float(i-tmpTile.x)*float(i-tmpTile.x)+float(j-tmpTile.y)*float(j-tmpTile.y)));
                    if(diff==0)
                        mapa[i][j].addInfluence(k,300);
                    else if(diff<maxR)
                        mapa[i][j].addInfluence(k,maxR-diff);
                }
            }
        }
    }
    ///setting tile owner
    for(int i=0;i<getSize().x;i++)
        for(int j=0;j<getSize().y;j++)
        {
            std::optional<std::pair<int,int> > v=mapa[i][j].maxInfluenceIt();
            if(v.has_value())
            {
                mapa[i][j].setOwner(v->first);
            }
            else
            {
                mapa[i][j].setOwner(-1);
            }
        }
    for(int i=0;i<getSize().x;i++)
        for(int j=0;j<getSize().y;j++)
        {
            if(mapa[i][j].getOwner()==-1) continue;
            sf::Color color=players[cities[mapa[i][j].getOwner()].owner].playerColor;
            color.a=180;
            if(j==0 || mapa[i][j-1].getOwner()!=mapa[i][j].getOwner())
                mapa[i][j].line[0].setColor(color);
            if(i==getSize().x-1 || mapa[i+1][j].getOwner()!=mapa[i][j].getOwner())
                mapa[i][j].line[1].setColor(color);
            if(j==getSize().y-1 || mapa[i][j+1].getOwner()!=mapa[i][j].getOwner())
                mapa[i][j].line[2].setColor(color);
            if(i==0 || mapa[i-1][j].getOwner()!=mapa[i][j].getOwner())
                mapa[i][j].line[3].setColor(color);
        }
}

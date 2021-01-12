#ifndef _SQUARD_HPP_
#define _SQUARD_HPP_

#include "../model/Model.hpp"
#include "../helpers.hpp"
#include <map>
#include <cmath>

using namespace std;

enum SquardType {
    ATTACK = 0,
    PROTECTION = 1,
    BUILDER = 2,
    REPAIRER = 3,
    MINER = 4
};

struct Squard {
public:
    map<int,Entity> units;
    vector<Vec2Int> pathToTarget;
    SquardType squadGoal;
    int enemyId = -1;
    Squard(){};
    
    void addAndUpdate(int presentTick, Entity entity){
        if(!units.count(entity.id))
                {
                    units[entity.id] = Entity(entity);
                    units[entity.id].tick = presentTick;
                }
                else {
                    units[entity.id].position = entity.position;
                    units[entity.id].tick = presentTick;
        }
    }

    void removeDead(int presentTick){
        std::map<int, Entity>::iterator sBIter = units.begin();
        while (sBIter != units.end()) {
            if (sBIter->second.tick!=presentTick)
                sBIter = units.erase(sBIter); 
            else
                ++sBIter;
        }
    }

    Vec2Int getCenter()
    {
        std::map<int, Entity>::iterator iter = units.begin();
        int sumx = 0;
        int sumy = 0;
        while (iter != units.end()) {
            sumx+=iter->second.position.x;
            sumy+=iter->second.position.y;
            ++iter;
        }

        if(units.size()==0)
            return Vec2Int(0,0);
        return Vec2Int(sumx/units.size(),sumy/units.size());
    }

    Vec2Int getDispersion()
    {
        auto mean = getCenter();
        std::map<int, Entity>::iterator iter = units.begin();
        int sumx = 0;
        int sumy = 0;
        while (iter != units.end()) {
            sumx+=(iter->second.position.x-mean.x)*(iter->second.position.x-mean.x);
            sumy+=(iter->second.position.y-mean.y)*(iter->second.position.y-mean.y);
            ++iter;
        }

        if(units.size()==0)
            return Vec2Int(0,0);
        return Vec2Int(sumx/units.size(),sumy/units.size());
    }

    double getPercentageNormalDistribution()
    {
        auto mean = getCenter();
        auto disp = getDispersion();
        auto dispersionX = sqrt(disp.x);
        auto dispersionY = sqrt(disp.y);

        std::map<int, Entity>::iterator iter = units.begin();
        double sum = 0;
        while (iter != units.end()) {
            if( iter->second.position.x>=mean.x-dispersionX-1 && iter->second.position.x<=mean.x+dispersionX+1 &&
                iter->second.position.y>=mean.y-dispersionY-1 && iter->second.position.y<=mean.y+dispersionY+1)
                sum++;
            ++iter;
        }
        return (sum/(double)units.size())*100;
    }

    vector<int> entitiesCloserEqToPoint(Vec2Int p,int dist)
    {
        std::vector<int> result = {};
        std::map<int, Entity>::iterator iter = units.begin();
        while (iter != units.end()) {
            if(distanceSqr(iter->second.position, p)<=dist*dist) //&& (sourceEntityId==0 || sourceEntityId!=iter->second.id) 
                result.push_back(iter->second.id);
            ++iter;
            
        }
        return result;
    }

private:
    
};

#endif
#ifndef _SQUARD_HPP_
#define _SQUARD_HPP_

#include "model/Model.hpp"
#include <map>

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
    Squard(){};
    
    void addAndUpdate(int presentTick, Entity entity){
        if(!units.count(entity.id))
                {
                    units[entity.id] = Entity(entity);
                    units[entity.id].tick = presentTick;
                }
                else {
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

private:
    
};

#endif
#ifndef _CITADEL_HPP_
#define _CITADEL_HPP_

#include "model/Model.hpp"
#include <map>

using namespace std;

struct Citadel {
public:
    map<int,Entity> buildings;
    map<int,Entity> rangeBuilds;
    map<int,Entity> builderBuilds;
    map<int,Entity> meleeBuilds;
    map<int,Entity> houseBuilds;
    map<int,Entity> turretBuilds;
    Citadel(){};
    
    void addAndUpdate(int presentTick, Entity entity){
        if(entity.entityType==RANGED_BASE)
            _addAndUpdate(presentTick, entity, rangeBuilds);
        if(entity.entityType==MELEE_BASE)
            _addAndUpdate(presentTick, entity, meleeBuilds);
        if(entity.entityType==HOUSE)
            _addAndUpdate(presentTick, entity, houseBuilds);
        if(entity.entityType==TURRET)
            _addAndUpdate(presentTick, entity, turretBuilds);
        if(entity.entityType==BUILDER_BASE)
            _addAndUpdate(presentTick, entity, builderBuilds);
    }


    void removeRuin(int presentTick){
        _removeRuin(presentTick, rangeBuilds);
        _removeRuin(presentTick, meleeBuilds);
        _removeRuin(presentTick, houseBuilds);
        _removeRuin(presentTick, turretBuilds);
        _removeRuin(presentTick, builderBuilds);
    }

private:
    void _removeRuin(int presentTick, map<int,Entity> items){
        std::map<int, Entity>::iterator sBIter = items.begin();
        while (sBIter != items.end()) {
            if (sBIter->second.tick!=presentTick)
                sBIter = items.erase(sBIter); 
            else
                ++sBIter;
        }
    }


    void _addAndUpdate(int presentTick, Entity entity, map<int,Entity> &items){
        if(!items.count(entity.id))
                {
                    items[entity.id] = Entity(entity);
                    items[entity.id].tick = presentTick;
                }
                else {
                    items[entity.id].tick = presentTick;
        }
    }
};

#endif
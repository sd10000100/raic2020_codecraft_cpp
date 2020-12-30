#ifndef _HELPERS_HPP_
#define _HELPERS_HPP_

#include "model/Model.hpp"



int distanceSqr(Vec2Int a, Vec2Int b) {
    return (a.x - b.x) * (a.x - b.x) + (a.y - b.y) * (a.y - b.y);
}


bool IsPlaceEmptyForHouse(PlayerView playerView, Vec2Int p, int identity, int size)
{
    for ( const Entity &entity: playerView.entities) {
        if(p.x<=entity.position.x && p.x+size>entity.position.x && p.y<=entity.position.y && p.y+size>entity.position.y)
            return false;
    }
    return true;
}

bool is_building(EntityType entity){
        switch(entity) {
            case WALL:
            case BUILDER_BASE:
            case MELEE_BASE:
            case RANGED_BASE:
            case HOUSE:
            case TURRET:
                return true;
            default: // if nothing else matches
                return false;
        }
}

#endif
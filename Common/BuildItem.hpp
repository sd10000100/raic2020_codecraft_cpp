#ifndef _BUILD_ITEM_HPP_
#define _BUILD_ITEM_HPP_

#include "../model/Model.hpp"

using namespace std;

struct BuildItem {
    EntityType typeByild;
    Vec2Int position;
    int builderId;
    int itemid;
    bool isBuilding = false;
    bool isPriority = false;
    bool started = false;
    BuildItem(EntityType typeByild,Vec2Int position, bool isPriority = false){
        this->typeByild = typeByild;
        this->position = position;
        this->itemid = itemid;
        this->isPriority = isPriority;
    }

};

#endif
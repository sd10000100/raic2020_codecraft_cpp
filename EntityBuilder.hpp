#ifndef _ENTITY_BUILDER_HPP_
#define _ENTITY_BUILDER_HPP_

#include "model/Model.hpp"

using namespace std;

struct EntityBuilder{
    Vec2Int position;
    bool isBuild = false;
    Vec2Int BuildPosition;
    int id;
    int tick = 0;
};

#endif
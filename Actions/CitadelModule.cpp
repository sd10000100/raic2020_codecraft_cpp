#include "../WinStrategy.hpp"
//#include "helpers.hpp"
#include "../Common/MagicProperties.hpp"

inline Action WinStrategy::getCitadelCommand(Action res, const PlayerView& playerView, DebugInterface* debugInterface)
{    
    int countRangedUnit = playerView.entityProperties.at(RANGED_UNIT).buildScore+info.rangeCount;
    int countMeleeUnit = playerView.entityProperties.at(MELEE_UNIT).buildScore+info.rangeCount;
    int countBuilderUnit = playerView.entityProperties.at(BUILDER_UNIT).buildScore+info.buildCount;
    int reservForBuild = 0;

        shared_ptr<MoveAction> moveAction = nullptr;
        shared_ptr<BuildAction> buildAction = nullptr;
        shared_ptr<RepairAction> repairAction = nullptr;
        shared_ptr<AttackAction> attackAction = nullptr;

    if(buildQueue.size()>0)
        reservForBuild = playerView.entityProperties.at(buildQueue[0].typeByild).initialCost;
//лучники
    std::map<int, Entity>::iterator iter =  citadel.rangeBuilds.begin();
    while (iter != citadel.rangeBuilds.end()) {
        const Entity& entity = iter->second;

        const EntityProperties& properties = playerView.entityProperties.at(entity.entityType);
        EntityType entityType = properties.build->options[0];
        if (info.resCount-reservForBuild>countRangedUnit && countBuilderUnit>MAX_BUILDER_UNITS-5)
        {
            buildAction = shared_ptr<BuildAction>(new BuildAction(
                RANGED_UNIT,
                Vec2Int(entity.position.x + properties.size, entity.position.y + properties.size - 1)));
            info.resCount-=countRangedUnit;
        }
        else
        {
            buildAction = nullptr;
        }

        res.entityActions[entity.id] = EntityAction(
            moveAction,
            buildAction,
            attackAction,
            repairAction);
        ++iter;
    }
// создание рабочих
    iter = citadel.builderBuilds.begin();
    while (iter != citadel.builderBuilds.end()) {
        const Entity& entity = iter->second;


        
        const EntityProperties& properties = playerView.entityProperties.at(entity.entityType);
        EntityType entityType = properties.build->options[0];
       if (info.resCount>countBuilderUnit && info.buildCount<MAX_BUILDER_UNITS)// && info.buildCount<=MAX_BUILDER_UNITS
       {
            buildAction = shared_ptr<BuildAction>(new BuildAction(
                BUILDER_UNIT,
                Vec2Int(entity.position.x + properties.size, entity.position.y + properties.size - 1)));
        }
       else if (info.buildCount>=MAX_BUILDER_UNITS)//10+info.houseCount*2
       {
           buildAction = nullptr;
       }

        res.entityActions[entity.id] = EntityAction(
            moveAction,
            buildAction,
            attackAction,
            repairAction);
        ++iter;
    }

// турели
    iter = citadel.turretBuilds.begin();
    const EntityProperties& properties = playerView.entityProperties.at(TURRET);
    while (iter != citadel.turretBuilds.end()) {
        const Entity& entity = iter->second;

        res.entityActions[entity.id] = EntityAction(nullptr, nullptr,
            shared_ptr<AttackAction>(new AttackAction(
                                    nullptr, shared_ptr<AutoAttack>(new AutoAttack(properties.sightRange, {BUILDER_UNIT,MELEE_UNIT ,RANGED_UNIT})))),
            nullptr);
        ++iter;
    }

    return res;
}
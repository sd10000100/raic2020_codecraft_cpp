#include "WinStrategy.hpp"
#include "helpers.hpp"
#include "MagicProperties.hpp"

Action WinStrategy::getCitadelCommand(Action res, const PlayerView& playerView, DebugInterface* debugInterface)
{    
    int countRangedUnit = playerView.entityProperties.at(RANGED_UNIT).buildScore+info.rangeCount;
    int countMeleeUnit = playerView.entityProperties.at(MELEE_UNIT).buildScore+info.rangeCount;
//мечники
    std::map<int, Entity>::iterator iter = citadel.meleeBuilds.begin();
    while (iter != citadel.meleeBuilds.end()) {
        const Entity& entity = iter->second;

        shared_ptr<MoveAction> moveAction = nullptr;
        shared_ptr<BuildAction> buildAction = nullptr;
        shared_ptr<RepairAction> repairAction = nullptr;
        shared_ptr<AttackAction> attackAction = nullptr;
        const EntityProperties& properties = playerView.entityProperties.at(entity.entityType);
        EntityType entityType = properties.build->options[0];

        if (info.resCount>countMeleeUnit && (countRangedUnit>countMeleeUnit*3 || info.rangeCount==0))
        {
            buildAction = shared_ptr<BuildAction>(new BuildAction(
                MELEE_UNIT,
                Vec2Int(entity.position.x + properties.size, entity.position.y + properties.size - 1)));
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
// лучники
    iter = citadel.rangeBuilds.begin();
    while (iter != citadel.rangeBuilds.end()) {
        const Entity& entity = iter->second;

        shared_ptr<MoveAction> moveAction = nullptr;
        shared_ptr<BuildAction> buildAction = nullptr;
        shared_ptr<RepairAction> repairAction = nullptr;
        shared_ptr<AttackAction> attackAction = nullptr;

        const EntityProperties& properties = playerView.entityProperties.at(entity.entityType);
        EntityType entityType = properties.build->options[0];
        if (info.resCount>countRangedUnit && countRangedUnit<=countMeleeUnit*3)
        {
            buildAction = shared_ptr<BuildAction>(new BuildAction(
                RANGED_UNIT,
                Vec2Int(entity.position.x + properties.size, entity.position.y + properties.size - 1)));
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

        shared_ptr<MoveAction> moveAction = nullptr;
        shared_ptr<BuildAction> buildAction = nullptr;
        shared_ptr<RepairAction> repairAction = nullptr;
        shared_ptr<AttackAction> attackAction = nullptr;
        int countBuilderUnit = playerView.entityProperties.at(BUILDER_UNIT).buildScore+info.buildCount;
        const EntityProperties& properties = playerView.entityProperties.at(entity.entityType);
        EntityType entityType = properties.build->options[0];
        if (info.resCount>countBuilderUnit && info.buildCount<5+info.houseCount*2 && info.buildCount<=MAX_BUILDER_UNITS)
        {
            buildAction = shared_ptr<BuildAction>(new BuildAction(
                BUILDER_UNIT,
                Vec2Int(entity.position.x + properties.size, entity.position.y + properties.size - 1)));
        }
        else if (info.buildCount>=5+info.houseCount*2)
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
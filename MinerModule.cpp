#include "WinStrategy.hpp"
#include "helpers.hpp"

Action WinStrategy::getMinerCommand(Action res, const PlayerView& playerView, DebugInterface* debugInterface)
{
    vector<EntityType> validAutoAttackTargets;
        //if (entity.entityType == BUILDER_UNIT) {
            validAutoAttackTargets.push_back(RESOURCE);
       // }
    std::map<int, Entity>::iterator iter = squardMiner.units.begin();

    while (iter != squardMiner.units.end()) {
        const Entity& entity = iter->second;

        shared_ptr<MoveAction> moveAction = nullptr;
        shared_ptr<BuildAction> buildAction = nullptr;
        shared_ptr<RepairAction> repairAction = nullptr;
        shared_ptr<AttackAction> attackAction = nullptr;

 const Entity *nearestResource = nullptr;
                            for (size_t j = 0; j < playerView.entities.size(); j++) {
                                const Entity& resource = playerView.entities[j];
                                if (resource.entityType==RESOURCE) {
                                    if (nearestResource == nullptr ||
                                                    distanceSqr(entity.position, resource.position) <
                                                        distanceSqr(entity.position, nearestResource->position)
                                                    ) {
                                                nearestResource = &resource;
                                            }
                                }
                            }
                            if (nearestResource!=nullptr){
                                Vec2Int target = nearestResource->position;
                                //Nullable!(AutoAttack) autoattack = AutoAttack(60, [EntityType.Resource, EntityType.BuilderUnit]);
                                moveAction = shared_ptr<MoveAction>(new MoveAction(nearestResource->position,true,false));
                                attackAction = shared_ptr<AttackAction>(new AttackAction(
                                    nullptr, shared_ptr<AutoAttack>(new AutoAttack(60, validAutoAttackTargets))));
                            }
                            else if(attackers.count(entity.id)){

                                int randx = rand() % 80;
                     int randy = rand() % 80;
                                attackers[entity.id] = Vec2Int(randx, randy);
                                moveAction = shared_ptr<MoveAction>(new MoveAction(
                                Vec2Int(randx, randy),
                                true,
                                true));
                            }
        res.entityActions[entity.id] = EntityAction(
            moveAction,
            buildAction,
            attackAction,
            repairAction);
        ++iter;
    }


    return res;
}

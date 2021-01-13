#include "../WinStrategy.hpp"
//#include "helpers.hpp"



inline Action WinStrategy::getBuildCommand(Action res, const PlayerView& playerView, DebugInterface* debugInterface)
{
    vector<EntityType> validAutoAttackTargets;
        //if (entity.entityType == BUILDER_UNIT) {
            validAutoAttackTargets.push_back(RESOURCE);
       // }


       
    std::map<int, Entity>::iterator iter = squardBuilders.units.begin();

    while (iter != squardBuilders.units.end()) {
        const Entity& entity = iter->second;

        shared_ptr<MoveAction> moveAction = nullptr;
        shared_ptr<BuildAction> buildAction = nullptr;
        shared_ptr<RepairAction> repairAction = nullptr;
        shared_ptr<AttackAction> attackAction = nullptr;
        int houseCost = playerView.entityProperties.at(HOUSE).buildScore;


        const Entity *newBase = nullptr;
               for (size_t j = 0; j < playerView.entities.size(); j++) {
                    const Entity& building = playerView.entities[j];
                    if (building.playerId != nullptr && *building.playerId == playerView.myId && is_building(building.entityType) && building.health<playerView.entityProperties.at(building.entityType).maxHealth) {
                        if(building.entityType==RANGED_BASE || building.entityType==MELEE_BASE)
                        {
                            newBase = &building;
                            break;
                        }
                        else if ((building.entityType!=RANGED_BASE && building.entityType!=MELEE_BASE) && (newBase == nullptr || 
                                        distanceSqr(entity.position, building.position) <
                                            distanceSqr(entity.position, newBase->position)
                        )) {
                                    newBase = &building;
                                }
                    }
                }

        if(newBase!=nullptr)
        {
            moveAction = shared_ptr<MoveAction>(new MoveAction(newBase->position,true,true));
            repairAction = shared_ptr<RepairAction>(new RepairAction(newBase->id));
        }
        else if(buildQueue.size()>0 && info.resCount>houseCost){
                    
            int newHouseX = 0;
            int newHouseY  =0;
            EntityType tpe = HOUSE;
            //TODO:
            bool isEmpty = false;
            bool isPrior = false;
                     
            int l = 0;
            int minDist = 10000;
            while(l<buildQueue.size())
            {
                isEmpty = IsPlaceEmptyForHouse(playerView, buildQueue[l].position,entity.id, playerView.entityProperties.at(buildQueue[l].typeByild).size);
                int dist = distanceSqr(entity.position, buildQueue[l].position);
                if((isEmpty || buildQueue[l].isPriority) && !buildQueue[l].started && dist<minDist)// || buildQueue[l].isPriority
                {
                    minDist = dist;
                    newHouseX = buildQueue[l].position.x;
                    newHouseY = buildQueue[l].position.y;
                    tpe = buildQueue[l].typeByild;
                    isPrior = buildQueue[l].isPriority;
                    // if (!isPrior)
                    buildQueue[l].started = true;
                    builders[entity.id].BuildPosition = Vec2Int(newHouseX,newHouseY);
                    builders[entity.id].isBuild=false;
                    //break;
                }
                l++;
            }
                            Vec2Int target = Vec2Int(newHouseX,newHouseY);
                            if(isPrior && !isEmpty)
                            {
                                
                                moveAction = shared_ptr<MoveAction>(new MoveAction(Vec2Int(newHouseX,newHouseY-1),true,false));
                                attackAction = shared_ptr<AttackAction>(new AttackAction(
                                    nullptr, shared_ptr<AutoAttack>(new AutoAttack(1, validAutoAttackTargets))));
                            }
                            else{
                                buildAction = shared_ptr<BuildAction>(new BuildAction(tpe, target));
                                moveAction = shared_ptr<MoveAction>(new MoveAction(target,true,true));
                            
                            }
                        }
                        else{

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

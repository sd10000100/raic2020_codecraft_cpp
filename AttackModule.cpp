#include "WinStrategy.hpp"
#include "helpers.hpp"


Action WinStrategy::getAttackCommand(Action res, const PlayerView& playerView, DebugInterface* debugInterface)
{
     vector<EntityType> validAutoAttackTargets;
      
    validAutoAttackTargets.push_back(BUILDER_UNIT);
    validAutoAttackTargets.push_back(MELEE_UNIT); 
    validAutoAttackTargets.push_back(RANGED_UNIT); 
    validAutoAttackTargets.push_back(TURRET); 
    validAutoAttackTargets.push_back(WALL); 
    validAutoAttackTargets.push_back(HOUSE); 
    validAutoAttackTargets.push_back(BUILDER_BASE); 
    validAutoAttackTargets.push_back(MELEE_BASE); 
    validAutoAttackTargets.push_back(RANGED_BASE);
    
    std::map<int, Entity>::iterator iter = squardArmy.units.begin();
    while (iter != squardArmy.units.end()) {
        const Entity& entity = iter->second;
        const EntityProperties& properties = playerView.entityProperties.at(entity.entityType);
        shared_ptr<MoveAction> moveAction = nullptr;
        shared_ptr<BuildAction> buildAction = nullptr;
        shared_ptr<RepairAction> repairAction = nullptr;
        shared_ptr<AttackAction> attackAction = nullptr;


            const Entity *nearestEnemy = nullptr;
                for (size_t j = 0; j < playerView.entities.size(); j++) {
                    const Entity& enemyEntity = playerView.entities[j];
                    if (enemyEntity.playerId != nullptr && *enemyEntity.playerId != playerView.myId) { //&& (rangeCount+meleeCount>40 || distanceSqr(enemyEntity.position, Vec2Int(0,0))<40*40)
                        if (nearestEnemy == nullptr ||
                                        distanceSqr(entity.position, enemyEntity.position) <
                                            distanceSqr(entity.position, nearestEnemy->position)
                                        ) {
                                    nearestEnemy = &enemyEntity;
                                }
                    }
                }
                if (nearestEnemy != nullptr){
                    moveAction = shared_ptr<MoveAction>(new MoveAction(//Vec2Int(40,40),
                        nearestEnemy->position,
                        true,
                        true));
                    attackAction = shared_ptr<AttackAction>(new AttackAction(
                        nullptr, shared_ptr<AutoAttack>(new AutoAttack(properties.sightRange, validAutoAttackTargets))));

                    std::map<int,Vec2Int>::iterator it = attackers.find(entity.id);             // by iterator (b), leaves acdefghi.
                        if( it != attackers.end() )
                            attackers.erase (it);
                }
                else if(!attackers.count(entity.id) || (attackers[entity.id].x == entity.position.x && attackers[entity.id].y == entity.position.y)){ 
                     int randx = rand() % 80;
                     int randy = rand() % 80;
                    moveAction = shared_ptr<MoveAction>(new MoveAction(Vec2Int(randx, randy),
                        //nearestEnemy->position,
                        true,
                        true));
                    attackAction = shared_ptr<AttackAction>(new AttackAction(
                        nullptr, shared_ptr<AutoAttack>(new AutoAttack(properties.sightRange, validAutoAttackTargets))));
                   attackers[entity.id] = Vec2Int(randx, randy);
             
                }
                else{
                    moveAction = shared_ptr<MoveAction>(new MoveAction( attackers[entity.id],
                        //nearestEnemy->position,
                        true,
                        true));
                    attackAction = shared_ptr<AttackAction>(new AttackAction(
                        nullptr, shared_ptr<AutoAttack>(new AutoAttack(properties.sightRange, validAutoAttackTargets))));
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
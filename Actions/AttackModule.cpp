#include "../WinStrategy.hpp"

//#include "helpers/AStar.h"
//#include "helpers.hpp"
#include <iostream>
#include <cmath>

inline Action WinStrategy::getAttackCommand(Action res, const PlayerView& playerView, DebugInterface* debugInterface, Squard Army)
{
    //AStar aStarPathfinder = AStar();
    int width = playerView.mapSize;
    int height = playerView.mapSize;
    double **a = array_generator(width+1,height+1);
    for (int i=0;i<height;i++) {
        for (int j=0;j<width;j++) {
            a[i][j] = 0;
        }
    }


    for (size_t j = 0; j < playerView.entities.size(); j++) {
        const Entity& enemyEntity = playerView.entities[j];
        if (enemyEntity.playerId == nullptr && enemyEntity.entityType==RESOURCE) { //&& (rangeCount+meleeCount>40 || distanceSqr(enemyEntity.position, Vec2Int(0,0))<40*40)
            PutPotential(5, 5, a, width, height, enemyEntity.position);
        }
        if (enemyEntity.playerId != nullptr && *enemyEntity.playerId!=playerView.myId) { //&& (rangeCount+meleeCount>40 || distanceSqr(enemyEntity.position, Vec2Int(0,0))<40*40)
            PutPotential(-60, 1, a, width, height, enemyEntity.position);
        }
    }
    AStar astar = AStar();
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


    shared_ptr<MoveAction> moveAction = nullptr;
    shared_ptr<BuildAction> buildAction = nullptr;
    shared_ptr<RepairAction> repairAction = nullptr;
    shared_ptr<AttackAction> attackAction = nullptr;

    Vec2Int center = Army.getCenter();
    const Entity *nearestEnemy = nullptr;
    for (size_t j = 0; j < playerView.entities.size(); j++) {
        const Entity& enemyEntity = playerView.entities[j];
        if (enemyEntity.playerId != nullptr && *enemyEntity.playerId == Army.enemyId) {
            if (nearestEnemy == nullptr ||
                            distanceSqr(center, enemyEntity.position) <
                                distanceSqr(center, nearestEnemy->position)
                            ) {
                        nearestEnemy = &enemyEntity;
                    }
        }
    }


    //  if (nearestEnemy != nullptr && (playerView.currentTick%20==0 )){//squardArmy.pathToTarget.size()==0 || 
    //     squardArmy.pathToTarget = astar.FindPath(center,nearestEnemy->position, 80,80, a);
    //  }

    auto goodPointInInflMap =  GetMinPotentialByRadius(25, a, 80,80, center);
//     for(Vec2Int v : ress)
//                 {
//                     PutPotential(1, 1, a, width, height, v);
//                 }
    // for (int i=0;i<width;i++) {
    //         for (int j=height-1;j>=0;j--) {
    //                 cerr << a[j][i]<<' ';
    //         }
    //         cerr<<endl;
    //     }
// 
// cerr<<squardArmy.pathToTarget.size();
// cerr<<endl;
//cerr<<center.x<<' '<<center.y;
//cerr<<endl;


double percentageNormalDistribution = Army.getPercentageNormalDistribution();
//cerr<<percentageNormalDistribution<<'%';
//cerr<<endl;
    std::map<int, Entity>::iterator iter = Army.units.begin();
    while (iter != Army.units.end()) {
        const Entity& entity = iter->second;
        const EntityProperties& properties = playerView.entityProperties.at(entity.entityType);

        if(percentageNormalDistribution<60)
        {
            moveAction = shared_ptr<MoveAction>(new MoveAction(//Vec2Int(40,40),
                center,
                true,
                true));
            attackAction = shared_ptr<AttackAction>(new AttackAction(
                nullptr, shared_ptr<AutoAttack>(new AutoAttack(properties.sightRange, validAutoAttackTargets))));
            
        }
        else{

    
            if (nearestEnemy != nullptr){    

                int attack = properties.attack->attackRange;
                //attack->attack_range;

                vector<int>  misfortuneCompanion =  Army.entitiesCloserEqToPoint(nearestEnemy->position,properties.sightRange+1);
                int distSQRToEnemy = distanceSqr(entity.position, nearestEnemy->position);

                if(distSQRToEnemy==attack*attack)
                {
                    moveAction = nullptr;
                    attackAction = shared_ptr<AttackAction>(new AttackAction(
                        nullptr, shared_ptr<AutoAttack>(new AutoAttack(properties.sightRange, validAutoAttackTargets))));
                }
                else if(distSQRToEnemy<attack*attack)
                {
                    Vect2DVect2Int v = Vect2DVect2Int(entity.position,nearestEnemy->position);
                    v.turn(3.14159);
                     moveAction = shared_ptr<MoveAction>(new MoveAction(//Vec2Int(40,40),
                        v.finish,
                        true,
                        true));
                    attackAction = shared_ptr<AttackAction>(new AttackAction(
                        nullptr, shared_ptr<AutoAttack>(new AutoAttack(properties.sightRange, validAutoAttackTargets))));
                }
                else if(distSQRToEnemy>attack*attack && misfortuneCompanion.size()>1)
                {
                    moveAction = shared_ptr<MoveAction>(new MoveAction(//Vec2Int(40,40),
                        nearestEnemy->position,
                        true,
                        true));
                    attackAction = shared_ptr<AttackAction>(new AttackAction(
                        nullptr, shared_ptr<AutoAttack>(new AutoAttack(properties.sightRange, validAutoAttackTargets))));
                }
                else {
                    Vec2Int target = goodPointInInflMap;
                    
                    moveAction = shared_ptr<MoveAction>(new MoveAction(//Vec2Int(40,40),
                        target,
                        true,
                        true));
                    attackAction = shared_ptr<AttackAction>(new AttackAction(
                        nullptr, shared_ptr<AutoAttack>(new AutoAttack(properties.sightRange, validAutoAttackTargets))));

                    // std::map<int,Vec2Int>::iterator it = attackers.find(entity.id);             // by iterator (b), leaves acdefghi.
                    //     if( it != attackers.end() )
                    //         attackers.erase (it);
                }   
            }

            //это когда в тумане нихрена нету
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
        }
        res.entityActions[entity.id] = EntityAction(
            moveAction,
            buildAction,
            nullptr,
            repairAction);
        ++iter;
    }


    return res;
}
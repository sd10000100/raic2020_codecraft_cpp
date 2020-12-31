#include "WinStrategy.hpp"

//#include "helpers/AStar.h"
//#include "helpers.hpp"
#include <iostream>
#include <cmath>

// inline double **array_generator(unsigned int dim1, unsigned int dim2) {
//     double **ptrary = new double * [dim1];
//     for (int i = 0; i < dim1; i++) {
//         ptrary[i] = new double [dim2];
//     }
//     return ptrary;
// }


// inline int signedMax(int a, int b)
// {
//     if(abs(a)>=abs(b))
//         return a;
//     else return b;

// }
// inline int getSign(int x)
// {
//     if (x>0)
//         return 1;
//     else return -1;
// }

// inline void PutPotential(double power, double step, double** matr, int sizeX, int sizeY, Vec2Int p)
// {
//     int s = 0;
//     int x = int(abs(floor(p.x)));
//     int y = int(abs(floor(p.y)));
//     for(int l = 0;l<fabs(power);l=l+step, s++)
//     {
//         for(int temp = y-s;temp<=s+y;temp++)
//         {
//             int tempArrMinX = int(floor(x-s));
//             int tempArrMaxX = int(floor(x+s));
//             if(tempArrMinX>=0 && tempArrMinX<sizeX && temp>=0 && temp<sizeY)
//                 matr[tempArrMinX][temp]=signedMax(matr[tempArrMinX][temp],getSign(power)*(abs(power)-l));
//             if(tempArrMaxX<sizeX && tempArrMaxX>=0 && temp>=0 && temp<sizeY)
//                 matr[tempArrMaxX][temp]=signedMax(matr[tempArrMaxX][temp],getSign(power)*(abs(power)-l));
//         }
//         for(int temp = x-s+1;temp<=s+x-1;temp++)
//         {
//             int tempArrMinY = int(floor(y-s));
//             int tempArrMaxY = int(floor(y+s));
//             if(tempArrMinY>=0 && tempArrMinY<sizeY && temp>=0 && temp<sizeX)
//                 matr[temp][tempArrMinY]=signedMax(matr[temp][tempArrMinY],getSign(power)*(abs(power)-l));
//             if(tempArrMaxY>=0 && tempArrMaxY<sizeY  && temp>=0 && temp<sizeX)
//                 matr[temp][tempArrMaxY]=signedMax(matr[temp][tempArrMaxY],getSign(power)*(abs(power)-l));
//         }
//     }
// }

inline Action WinStrategy::getAttackCommand(Action res, const PlayerView& playerView, DebugInterface* debugInterface)
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
            PutPotential(8, 2, a, width, height, enemyEntity.position);
        }
    }
    //поиск пути работает, стоит прикрутить к отряду и считать
//     AStar astar = AStar();
//     std::vector<Vec2Int> ress = astar.FindPath(Vec2Int(0,0),Vec2Int(79,79), 80,80, a);
//     for(Vec2Int v : ress)
//                 {
//                     PutPotential(1, 1, a, width, height, v);
//                 }
//     for (int i=0;i<width;i++) {
//             for (int j=height-1;j>=0;j--) {
//                     cerr << a[j][i]<<' ';
//             }
//             cerr<<endl;
//         }
// cerr<<endl;
// cerr<<endl;
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
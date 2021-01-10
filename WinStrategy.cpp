#include "WinStrategy.hpp"
//#include "helpers.hpp"
#include "BaseDevelopmentScheme.hpp"
#include <exception>
#include <map>
#include <algorithm>
#include <iostream>
#include "AttackModule.cpp"
#include "CitadelModule.cpp"
#include "MinerModule.cpp"
#include "BuilderModule.cpp"
using namespace std;



WinStrategy::WinStrategy(){
    info = StrategyInfo();
    attackers = {};
    idBuilder = -1;
    builders = {};
    squardBuilders = Squard();
    squardRepairs = Squard();
    squardMiner = Squard();
    squardRanges = Squard();
    squardArmy = Squard();
    squardArmy2 = Squard();
    squardArmy3 = Squard();
    citadel = Citadel();
}

Squard& WinStrategy::getMinArmy(){

    Squard& smallest = squardArmy;
    if (smallest.units.size() > squardArmy2.units.size()) smallest = squardArmy2;
    if (smallest.units.size() > squardArmy3.units.size()) smallest = squardArmy3;
    return smallest;
}

Action WinStrategy::getAction(const PlayerView& playerView, DebugInterface* debugInterface){
    Action result = Action(unordered_map<int, EntityAction>());
    
    info.clear();
    for (const Player &player: playerView.players){
        if (player.id==playerView.myId){
                info.resCount = player.resource;
                break;
        }
        else
        {
            if(playerView.players.size()==2 && squardArmy.enemyId==-1)
            {
                squardArmy.enemyId==player.id;
                squardArmy2.enemyId==player.id;
                squardArmy3.enemyId==player.id;
            }
            else{
                if (squardArmy.enemyId==-1)
                {
                    squardArmy.enemyId==player.id;
                }
                else if(squardArmy.enemyId!=player.id && squardArmy2.enemyId==-1)
                {squardArmy2.enemyId==player.id;}
                else if(squardArmy.enemyId!=player.id && squardArmy2.enemyId!=player.id && squardArmy3.enemyId==-1)
                {squardArmy3.enemyId==player.id;}
            }
        }
    }
    if (playerView.fogOfWar)
         buildQueue = buildQueueSecondRaund;
    else buildQueue = buildQueueFirstRaund;

    for (size_t i = 0; i < playerView.entities.size(); i++) {
        const Entity& entity = playerView.entities[i];
        if (entity.playerId != nullptr && *entity.playerId == playerView.myId) {
            if (entity.entityType == BUILDER_UNIT){
                info.buildCount++;
                if(squardBuilders.units.size()<3 || squardBuilders.units.count(entity.id))
                    squardBuilders.addAndUpdate(playerView.currentTick, entity);
                else squardMiner.addAndUpdate(playerView.currentTick, entity);
            }
            if(entity.entityType == RANGED_UNIT || entity.entityType == MELEE_UNIT)
            {
                if (entity.entityType == RANGED_UNIT)
                    info.rangeCount++;
                if (entity.entityType == MELEE_UNIT)
                    info.meleeCount++;
                //getMinArmy().addAndUpdate(playerView.currentTick, entity);

                int smallest = 1;
                int countsm = squardArmy.units.size();
                if (countsm > squardArmy2.units.size()) {smallest = 2;countsm = squardArmy2.units.size();}
                if (countsm > squardArmy3.units.size()) {smallest = 3;countsm = squardArmy3.units.size();}
                if(smallest==1 || squardArmy.units.count(entity.id))
                    squardArmy.addAndUpdate(playerView.currentTick, entity);
                else if(smallest==2 || squardArmy2.units.count(entity.id))
                    squardArmy2.addAndUpdate(playerView.currentTick, entity);
                else 
                    squardArmy3.addAndUpdate(playerView.currentTick, entity);
                // if(squardArmy.units.size()<squardArmy2.units.size() || squardArmy.units.size()<squardArmy3.units.size())
                //     squardArmy.addAndUpdate(playerView.currentTick, entity);
                // else if (squardArmy2.units.size()<squardArmy.units.size() || squardArmy2.units.size()<squardArmy3.units.size())
                //     squardArmy2.addAndUpdate(playerView.currentTick, entity);
                // else squardArmy3.addAndUpdate(playerView.currentTick, entity);
            }
            if(is_building(entity.entityType)){
                std::vector<BuildItem>::iterator it = buildQueue.begin();
                while (it != buildQueue.end()) {
                    if (entity.position.x == it->position.x && entity.position.y == it->position.y)
                        it = buildQueue.erase(it); // after erasing, 'it' will be set to the next element in v
                    else
                        ++it;             // manually set 'it' to the next element in v
                }
                citadel.addAndUpdate(playerView.currentTick, entity);
            }
        }
    }



    squardBuilders.removeDead(playerView.currentTick);
    squardArmy.removeDead(playerView.currentTick);
    squardArmy2.removeDead(playerView.currentTick);
    squardArmy3.removeDead(playerView.currentTick);
    squardMiner.removeDead(playerView.currentTick);
    citadel.removeRuin(playerView.currentTick);


    result = getCitadelCommand(result, playerView, debugInterface);
    result = getBuildCommand(result, playerView, debugInterface);
    result = getMinerCommand(result, playerView, debugInterface);
    result = getAttackCommand(result, playerView, debugInterface,squardArmy);
    result = getAttackCommand(result, playerView, debugInterface,squardArmy2);
    result = getAttackCommand(result, playerView, debugInterface,squardArmy3);
    return result;
}
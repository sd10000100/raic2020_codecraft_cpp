#ifndef _WIN_STRATEGY_HPP_
#define _WIN_STRATEGY_HPP_

#include "DebugInterface.hpp"
#include "BuildItem.hpp"
#include "Squard.hpp"
#include "Citadel.hpp"
#include "EntityBuilder.hpp"

#include <memory>
#include <map>
#include <vector>

using namespace std;
struct StrategyInfo
{
    int myId;
    int buildCount;
    int rangeCount;
    int meleeCount;
    int houseCount;
    int finalhouseCount;
    int turretCount;
    int resCount ;

    StrategyInfo(){
        myId = 0;
        buildCount = 0;
        rangeCount = 0;
        meleeCount = 0;
        houseCount = 0;
        finalhouseCount = 0;
        turretCount = 0;
        resCount = 0;
    }

    void clear(){
        myId = 0;
        buildCount = 0;
        rangeCount = 0;
        meleeCount = 0;
        houseCount = 0;
        finalhouseCount = 0;
        turretCount = 0;
        resCount = 0;
    }
};
 
class WinStrategy {
public:
    StrategyInfo info;
    map<int, Vec2Int> attackers;
    map<int, Entity> myEntities;
    vector<BuildItem> buildQueue;
    map<int,EntityBuilder> builders;
     Squard squardBuilders;
     Squard squardMiner;
     Squard squardRepairs;
     Squard squardRanges;
     Squard squardArmy;
     Citadel citadel;
    int idBuilder;
    WinStrategy();
    Action getAttackCommand(Action res, const PlayerView& playerView, DebugInterface* debugInterface);
    Action getMinerCommand(Action res, const PlayerView& playerView, DebugInterface* debugInterface);
    Action getBuildCommand(Action res, const PlayerView& playerView, DebugInterface* debugInterface);
    Action getCitadelCommand(Action res, const PlayerView& playerView, DebugInterface* debugInterface);
    Action getAction(const PlayerView& playerView, DebugInterface* debugInterface);

private:
    
};

#endif
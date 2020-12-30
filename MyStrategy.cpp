#include "MyStrategy.hpp"
#include "WinStrategy.hpp"
#include <exception>
#include <map>

using namespace std;

MyStrategy::MyStrategy() {
    strategy = WinStrategy();
}


Action MyStrategy::getAction(const PlayerView& playerView, DebugInterface* debugInterface)
{
    // Action result = Action(unordered_map<int, EntityAction>());
    // return result;
    return strategy.getAction(playerView, debugInterface);
}

void MyStrategy::debugUpdate(const PlayerView& playerView, DebugInterface& debugInterface)
{
    debugInterface.send(DebugCommand::Clear());
    debugInterface.getState();
}
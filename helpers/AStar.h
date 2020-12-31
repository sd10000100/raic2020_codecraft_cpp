//
// Created by badru on 15.01.2020.
//

#ifndef AICUP2019_ASTAR_H
#define AICUP2019_ASTAR_H

#include "vector"
#include <cmath>
#include <list>
#include <algorithm>
#include "InfluenceMapBuilder.h"

// A*
struct PathNode
{
    // Координаты точки на карте.
    Vec2Int Position;
    // Длина пути от старта (G).
    double PathLengthFromStart=10000;
    // Точка, из которой пришли в эту точку.
    PathNode* CameFrom;
    // Примерное расстояние до цели (H).
    double HeuristicEstimatePathLength;
    double potential = 0; 

    std::vector<Vec2Int> path = {};
    // Ожидаемое полное расстояние до цели (F).
    double EstimateFullPathLength() {
        return this->PathLengthFromStart  + potential;//+ this->HeuristicEstimatePathLength
    }

    const double EstimateFullPathLengthConst() const {
        return this->PathLengthFromStart + potential;//+ this->HeuristicEstimatePathLength 
    }

    bool operator < (const PathNode &node) const
    {
        return EstimateFullPathLengthConst()<node.EstimateFullPathLengthConst();
    }

    bool operator == (const PathNode &node)
    {
        return floor(Position.x) == floor(node.Position.x) && floor(Position.y) == floor(node.Position.y);
    }
};

class AStar {
public :
    AStar(){};

    bool isPointInUnit(const Vec2Int point, const Vec2Int unitPosition, const Vec2Int unitSize);

    double GetHeuristicPathLength(Vec2Int from, Vec2Int to);

    PathNode GetMinF(std::list<PathNode> list);

    std::vector<Vec2Int> GetPathForNode(PathNode pathNode);

    std::vector<PathNode> GetNeighbours(PathNode pathNode, Vec2Int goal,int sizeX, int sizeY, double** matr);//, const Game &game, const Unit &currentUnit

    std::vector<Vec2Int> FindPath(Vec2Int from, Vec2Int to, int sizeX, int sizeY,double** matr);//, const Game &game, const Unit &currentUnit
};

#endif //AICUP2019_ASTAR_H

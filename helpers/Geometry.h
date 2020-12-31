//
// Created by badru on 15.01.2020.
//

#include "../model/Vec2Int.hpp"
#include "Vect2D.h"
// TODO: поечму без inline повторяется где то?

// расстояние между 2-мя точками
// inline double distanceSqr(Vec2Int a, Vec2Int b) {
//     return (a.x - b.x) * (a.x - b.x) + (a.y - b.y) * (a.y - b.y);
// }

// ориентированная площадь треугольника
// Исп-я понятие косого (псевдополяр) произвед векторов
// a ^ b = |a||b|sin(<(a,b))=2S
// угол вращения между векторами против часовой стрелки
// 2s=|x1 y1 1|
//    |x2 y2 1|=(x2-x1)(y3-y1)-(y2-y1)(x3-x1)
//    |x3 y3 1|
template<typename T>
inline double OrientedArea (Point2D<T> a, Point2D<T> b, Point2D<T> c) {
    return (b.x - a.x) * (c.y - a.y) - (b.y - a.y) * (c.x - a.x);
}

// проверка, что точки не лежат на одной плоскости
inline bool isPointNotOnSamePlane (double a, double b, double c, double d) {
    if (a > b) std::swap(a, b);
    if (c > d) std::swap(c, d);
    return std::max(a, c) <= std::min(b, d);
}

// Чтобы отрезки AB и CD пересекались, нид чтобы A и B находились
// по разные стороны от прямой CD и аналогично C и D по разные стороны от AB
// Нужно вычислить ориентированные площади треугольников и сравнить знаки
template<typename T>
inline bool intersect (Point2D<T> a, Point2D<T> b, Point2D<T> c, Point2D<T> d) {
    return isPointNotOnSamePlane (a.x, b.x, c.x, d.x)
           && isPointNotOnSamePlane (a.y, b.y, c.y, d.y)
           && OrientedArea(a,b,c) * OrientedArea(a,b,d) <= 0
           && OrientedArea(c,d,a) * OrientedArea(c,d,b) <= 0;
}

// Only for CodeSide
inline bool intersect (Vec2Int a, Vec2Int b, Vec2Int c, Vec2Int d) {
    using Point2Double = Point2D<double>;
    return isPointNotOnSamePlane (a.x, b.x, c.x, d.x)
           && isPointNotOnSamePlane (a.y, b.y, c.y, d.y)
           && OrientedArea(Point2Double(a.x,a.y),
                           Point2Double(b.x,b.y),
                           Point2Double(c.x,c.y))
                           *
              OrientedArea(Point2Double(a.x,a.y),
                           Point2Double(b.x,b.y),
                           Point2Double(d.x,d.y)) <= 0
           && OrientedArea(Point2Double(c.x,c.y),
                           Point2Double(d.x,d.y),
                           Point2Double(a.x,a.y))
                           *
              OrientedArea(Point2Double(c.x,c.y),
                           Point2Double(d.x,d.y),
                           Point2Double(b.x,b.y)) <= 0;
}

// проверка на нахождение точки внутри треугольника (подходит для выпуклых многоугольников)
// Проверяем каждую грань на пересечение с лучом из искомой точки (луч в рандомное место)
// Если кол-во перемещений кратно 2-м - точка не в многоугольнике
// инеаче - внутри
// Примеч: можно случайно попасть на стык ребер - решается выбором луча поизощреннее

inline bool isPointInTriangle(Vec2Int p, Vec2Int p1, Vec2Int p2,Vec2Int p3){
    using Point2Double = Point2D<double>;
    using Vec2Int = Vect2D<double>;

    std::vector<Vec2Int> stackEnges = {};
    int countIntersect = 0;
    stackEnges.push_back(Vec2Int(Point2Double(p1.x,p1.y),Point2Double(p2.x,p2.y)));
    stackEnges.push_back(Vec2Int(Point2Double(p2.x,p2.y),Point2Double(p3.x,p3.y)));
    stackEnges.push_back(Vec2Int(Point2Double(p3.x,p3.y),Point2Double(p1.x,p1.y)));

    for(Vec2Int item : stackEnges)
    {
        if(intersect (Point2Double(p.x,p.y), Point2Double(10000,1), item.start, item.finish))
            countIntersect++;
    }
    if(countIntersect%2==0)
        return false;
    else {
        return true;
    }
}

// вариативный шаблон для построения вектора из точек из списка аргументов
template <class Arg>
std::vector<Arg> GetArgsToArray(Arg first)
{
    return {first};
}

template <class Arg, class... Other>
std::vector<Arg> GetArgsToArray(Arg first, Other... other)
{
    std::vector<Arg> res = {first};
    std::vector<Arg> sec = GetArgsToArray(other...);
    res.insert(res.end(), sec.begin(), sec.end());
    return res;
}

template <class... Args>
std::vector<Vec2Int> GetToArray(Args... args)
{
    std::vector<Vec2Int> stackEnges = {};
    stackEnges = GetArgsToArray(args...);
    return stackEnges;
}

// проверка на нахождение точки внутри выпуклого многоугольника
// Проверяем каждую грань на пересечение с лучом из искомой точки (луч в рандомное место)
// Если кол-во перемещений кратно 2-м - точка не в многоугольнике
// инеаче - внутри
// Примеч: можно случайно попасть на стык ребер - решается выбором луча поизощреннее
template <typename T,class... Args>
inline bool isPointInPolygon(Vec2Int p, Args ...args){
    std::vector<Point2D<T>> points = {};
    std::vector<Vect2D<T>> stackEnges = {};
    int countIntersect = 0;

    points = GetToArray(args...);

    if(points.size()==0) return false;

    Vec2Int first = points[0];
    for(int i=1; i<points.size();i++){
            stackEnges.push_back(Edge(first,points[i]));
            first = points[i];
    }

    stackEnges.push_back(Edge(first,stackEnges[0].start));
    for(Vect2D<T> item : stackEnges)
    {
        if(intersect (p, Vec2Int(10000,1), item.start, item.finish))
            countIntersect++;
    }
    if(countIntersect%2==0)
        return false;
    else {
        return true;
    }
}
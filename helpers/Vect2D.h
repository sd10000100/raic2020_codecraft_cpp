//
// Created by badru on 18.01.2020.
//

#ifndef AICUP2019_VECT2D_H
#define AICUP2019_VECT2D_H

#include "Point2D.h"

template<typename T>
struct Vect2D {
    using Point2D = Point2D<T>;
    Point2D start;
    Point2D finish;

    Vect2D(Point2D start, Point2D finish): start(start),finish(finish){}
    Point2D V() {
        return Point2D(finish.x - start.x, finish.y - start.y);
    }

//    double length() { return sqrt(p1.x*p1.x + p1.y*p1.y); }
//    void mult(double coeff) {
//        p2.x = p1.x + V().x * coeff;
//        p2.y = p1.y + V().y * coeff;
//    }
    void turn(double angle)
    {
        double x = V().x * cos(angle) - V().y * sin(angle);
        double y = V().y * cos(angle) + V().x * sin(angle);
        finish.x = start.x + x;
        finish.y = start.y + y;
    }
};
#endif //AICUP2019_VECT2D_H

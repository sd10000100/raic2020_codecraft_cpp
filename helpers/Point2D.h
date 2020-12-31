//
// Created by badru on 18.01.2020.
//

#ifndef AICUP2019_POINT2D_H
#define AICUP2019_POINT2D_H
#include "math.h"

template<typename T>
struct Point2D {
    T x,y;
    Point2D() {}
    Point2D(const Point2D &p) : x(p.x), y(p.y) {}
    Point2D(double x, double y) : x(x), y(y) {}

    Point2D& operator=(const Point2D &p)
    {
        x = p.x;
        y = p.y;
        return *this;
    }

    Point2D operator+(Point2D &p) {
        return { x+p.x, y+p.y };
    }

    double operator*(Point2D &p) {
        return x*p.x+y*p.y ;
    }

    Point2D operator-(Point2D &p) {
        return { x-p.x, y-p.y };
    }

    Point2D operator/(double a) {
        return (*this)*(1/a) ;
    }

    Point2D& operator+=(Point2D &p) {
        x += p.x;
        y += p.y;
        return *this;
    }

    Point2D& operator-=(Point2D &p) {
        x -= p.x;
        y -= p.y;
        return *this;
    }

    Point2D& operator*=(double a) {
        x*=a;
        y*=a;
        return *this;
    }

    Point2D& operator/=(double a) {
        return (*this)*=(1/a) ;
    }

    double dist() {
        return sqrt(x*x + y*y);
    }

    Point2D normalize(double len) {
        return { x/len, y/len };
    }

    Point2D normalize() {
        return { x*(1/dist()), y*(1/dist())};
    }

    double distTo(double x_, double y_) {
        return sqrt((x-x_)*(x-x_) + (y-y_)*(y-y_));
    }

    double distTo(Point2D p) {
        return distTo(p.x, p.y);
    }
};
#endif //AICUP2019_POINT2D_H

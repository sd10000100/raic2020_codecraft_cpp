//
// Created by badru on 15.01.2020.
//

#ifndef AICUP2019_INFLUENCEMAPBUILDER_H
#define AICUP2019_INFLUENCEMAPBUILDER_H

#include <cmath>
#include <vector>
#include "Geometry.h"

double **array_generator(unsigned int dim1, unsigned int dim2);

void array_destroyer(double **ary, unsigned int dim1);

int signedMax(int a, int b);

int getSign(int x);

void PutPotential(double power, double step, double** matr, int sizeX, int sizeY, Vec2Int p);

double putp(double oldV, double newV);

void PutAvgPotential(double power, double step, double** matr, int sizeX, int sizeY, Vec2Int p);

bool isCorrectCoordinate(int x, int y, int sizeX, int sizeY);

double getSumOfVectorOnInfluenseMap(Vec2Int fromV, Vec2Int toV, double** matr, int sizeX, int sizeY);

Vec2Int GetMinPotentialByRadius(int radius, double** matr, int sizeX, int sizeY, Vec2Int source);

#endif //AICUP2019_INFLUENCEMAPBUILDER_H

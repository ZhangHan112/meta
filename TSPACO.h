#ifndef __TSPACO_H__
#define __TSPACO_H__

#include<iostream>
#include<vector>
#include<cstdio>
#include<ctime>
#include<fstream>
#include<cmath>
#include <algorithm>
using namespace std;

typedef vector<int> solution;
typedef vector<solution> set;
typedef vector<double> parameter;
typedef vector<parameter> matrix;

#define ACS 1
#define MMAS 2

typedef struct ListPointer{
    int data;
    ListPointer *next;
}Node;


class TSPACO{
    int iter;
    int num;
    double rate;
    int run;
    int algo;

    double Best;
    int cities;
    
    double tau_max;
    double tau_min;

    solution X;
    solution Y;
    matrix distance;
    matrix pheromone;

    set Ant;

    void Initialize();
    solution Travel(int start);
    void Pheromone(const set &Ant);
    void Pheromone_MM(const set &Ant);
    solution two_opt(solution sol);
    double Fitness(solution sol);
    void print(matrix temp);
    void Print(parameter temp);
    void printSol(solution temp);
    void three_opt(solution sol);

    void ReadFile();

public:
    TSPACO(int Iteraiton,int AntNumber,double Evaporation,int Run,int Algo);
    void ACO();
};

#endif
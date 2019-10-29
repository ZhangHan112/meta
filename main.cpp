#include"TSPACO.h"

int main(int argc,char** argv){
    int Iteration = atoi(argv[1]);
    int AntNumber = atoi(argv[2]);
    double Evaporation = atof(argv[3]);
    int Run = atoi(argv[4]);
    int Algo = atoi(argv[5]);

    TSPACO aco(Iteration,AntNumber,Evaporation,Run,Algo);
    aco.ACO();
}
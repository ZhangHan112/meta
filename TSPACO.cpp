#include "TSPACO.h"

TSPACO::TSPACO(int Iteraiton,int AntNumber,double Evaporation,int Run,int Algo):
            iter(Iteraiton),num(AntNumber),rate(Evaporation),run(Run),algo(Algo){
    
    ReadFile();

    srand(time(NULL));
}

void TSPACO::ACO(){

    double avg=0;
    parameter res = parameter(iter);
    
    for(int r=0;r<run;r++){
        Initialize(); 
        for(int i=0;i<=iter;i++){
            for(int n=0;n<num;n++)
                Ant[n] = Travel(n);    
            
            for(int n=0;n<num;n++)
                Ant[n] = two_opt(Ant[n]);
                
            switch (algo){
                case ACS:Pheromone(Ant);   break;
                case MMAS:Pheromone_MM(Ant);   break;
                default:    break;
            }

            res[i] += Best; 
            
            cout << i << " " << res[i]/(r+1) << endl;

        }
        avg += Best;

    }
    avg /= run;

    // for(int i=0;i<iter;i++){
    //     res[i]/=run;
    //     cout << i << " " << res[i] << endl;
    // }


}

solution TSPACO::Travel(int start){

    solution sol;
    // start = rand()%51;
    sol.push_back(start);

    Node *header = NULL;
    Node *current;
    for(int i=0;i<cities;i++){ // construct unvisited cities set
        if(i==start) continue;

        if(!header){
            header = new Node;
            header->data = i;
            header->next = NULL;
            current = header;
        }
        else{
            current->next = new Node;
            current = current->next;
            current->data = i;
            current->next = NULL; 
        }
    }

    int currentPos = start;
    
    while(header){

        parameter probability;
        double Denominator = 0;
        int chosen=0; // selected from roulette wheel
        double max=0;
        Node *next = header; // traverse next unvisited cities
        Node *assign = header; 

        while(next){ // calculate the probability of cities (next city)
            double prob = pheromone[currentPos][next->data] / pow(distance[currentPos][next->data],2);

            probability.push_back(prob);
            Denominator += prob;
            next = next->next;

            if(max<prob){ //find the maximum probability route in this turn
                max = prob;
                chosen = probability.size()-1;
            }
                
        }

        double random = (double) (rand()%RAND_MAX)/RAND_MAX;
        if(random<1){ // decide whether to use roulette wheel

            //make roulette wheel
            parameter rw = parameter(probability.size());
            rw[0] = (double) (probability[0]/Denominator);
            for(int i=1;i<probability.size();i++){
                probability[i] = (double) (probability[i]/Denominator);
                rw[i] = probability[i]+rw[i-1];
            }

            //make rw decision   
            double randNum = (double) (rand()%RAND_MAX)/RAND_MAX;
            
            for(int i=0;i<rw.size();i++)
                if(randNum<rw[i]){
                    chosen = i;
                    break;
                }
        }

        for(int i=0;i<chosen;i++) //find element in unvisited cities which was selected from roulette wheel
            assign = assign->next;
        
        currentPos = assign->data; // move to next city

        sol.push_back(assign->data);
        
        // delete current city from unvisited set 
        Node *front = header;
        Node *rear;

        while (front!=NULL && front->data != assign->data){
            rear = front;
            front = front->next;
        }
        if (front == header){
            header = front->next;
            delete front;
        }
        else {
            rear->next = front->next;
            delete front;
        }

    }
    
    return sol;
}

void TSPACO::Pheromone(const set &Ant){

    for(int i=0;i<cities;i++)
        for(int j=0;j<cities;j++){
            if(i==j)
                continue;
            else if(i<j){
                pheromone[i][j] = (double) (rate*pheromone[i][j]); 
                pheromone[j][i] = pheromone[i][j];
            }
        }
    
    for(int n=0;n<num;n++){
        int front=0,rear=0;
        double sum=0;
        for(int i=0;i<cities;i++){
            front = Ant[n][i];
            rear = Ant[n][(i+1)%cities];
            sum += distance[front][rear];
        }
        
        for(int i=0;i<cities;i++){
            front = Ant[n][i];
            rear = Ant[n][(i+1)%cities];
            
            pheromone[front][rear] +=  (double) 1/sum;
            pheromone[rear][front] = pheromone[front][rear];

        }
        if(Best>sum)
            Best = sum;
    }

}

void TSPACO::Pheromone_MM(const set &Ant){

    for(int i=0;i<cities;i++)
        for(int j=0;j<cities;j++){
            if(i==j)
                continue;
            else if(i<j){
                pheromone[i][j] = (double) (rate*pheromone[i][j]); 
                pheromone[j][i] = pheromone[i][j];
            }
        }
    

    double iter_best = numeric_limits<double>::infinity();
    int pos = 0;
    int front=0,rear=0;

    for(int n=0;n<num;n++){
        double sum=Fitness(Ant[n]);
        if(iter_best>sum){
            iter_best = sum;
            pos = n;
        }
        if(Best>sum)
            Best = sum;
    }

    tau_max = (double)( 1/(1-rate) ) * (1/iter_best);
    double p_dec = pow(0.05,(double)1/cities);
    tau_min = ( tau_max*(1-p_dec) ) / ( ((cities/2)-1)*p_dec);

    // cout << tau_min << " " << tau_max << endl;

    for(int i=0;i<cities;i++){
        front = Ant[pos][i];
        rear = Ant[pos][(i+1)%cities];
        pheromone[front][rear] +=  (double) 1/iter_best;
        pheromone[rear][front] = pheromone[front][rear];

        if(pheromone[front][rear]>tau_max)
            pheromone[front][rear] = pheromone[rear][front] = tau_max;
        if(pheromone[front][rear]<tau_min)
            pheromone[front][rear] = pheromone[rear][front] = tau_min;
    }

    
    

}

solution TSPACO::two_opt(solution sol){

    double before = Fitness(sol);
    double after = 0;

    for(int i=0;i<cities-1;i++){
        for(int j=i+1;j<cities;j++){
            reverse(sol.begin()+i,sol.begin()+j);
            after = Fitness(sol);
            if(before>=after){
                before = after;
                continue;
            }
            reverse(sol.begin()+i,sol.begin()+j);
        }
    }
    if(Best>before) Best = before;

    return sol;
}

double TSPACO::Fitness(solution sol){
    double sum = 0;
    
    for(int i=0;i<cities;i++){
        int p1 = sol[i];
        int p2 = sol[(i+1)%cities];
        sum += distance[p1][p2];
    }
    return sum;
}

void TSPACO::Initialize(){
    
    Best = std::numeric_limits<int>::max();
    Ant = set(num,solution(cities));    
    pheromone = matrix(cities,parameter(cities));

    for(int i=0;i<cities;i++)
        for(int j=0;j<cities;j++){
            if(i==j)
                pheromone[i][j] = 0;
            else if(i<j){
                pheromone[i][j] = 1/distance[i][j];
                pheromone[j][i] = pheromone[i][j];
            }
        }
            

}

void TSPACO::ReadFile(){

    ifstream inputFile("input.txt");
    int temp,x,y;
    X.clear();
    Y.clear();
    while(inputFile>>temp>>x>>y){
        X.push_back(x);
        Y.push_back(y);
    }
    inputFile.close();
    cities = X.size();

    distance = matrix(cities,parameter(cities));

    for(int i=0;i<cities;i++){ //initialize distance[i][j]
        for(int j=0;j<cities;j++){
            if(i==j)
                distance[i][j] = 0;
            else if(i<j){
                distance[i][j] = (double) sqrt( pow(X[i]-X[j],2)+pow(Y[i]-Y[j],2) );
                distance[j][i] = distance[i][j];
            }
        }
    }

    ofstream base_line("base.dat");
    for(int i=0;i<=iter;i++)
        base_line << i << " " << 426 << endl;
    base_line.close();
}
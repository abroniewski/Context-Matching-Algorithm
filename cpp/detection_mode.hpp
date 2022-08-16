#ifndef DETECTION_MODE_HPP
#define DETECTION_MODE_HPP
#include "class.hpp"
//#include <vector>
#include <iostream>
#include <stdlib.h>

using namespace std;



class sample
{
    public:
    //Attributs
    vector<state> states;
    int taille;
    //Fonctions
    sample();
    sample(vector<state> states_init);
    sample(int n);
};

ostream & operator << (ostream & out, state S);

vector<double> importance_sampling(vector<state> S, point z);

vector<state> resample(vector<state> S, vector<double> w, int n);

state translation(state etat, double d,roadNetwork Roads);

state update(state etat,double t);
vector<state> update(vector<state> echantillon,double t);

vector<state> particle_filter(vector<observation> obs,int n,roadNetwork Roads);


//double normRoadstate(roadNetwork Roads,state i, state j);
#endif
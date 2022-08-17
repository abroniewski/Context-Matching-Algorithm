#ifndef MATCHING_HPP
#define MATCHING_HPP
#include "class.hpp"


void graphFromStates(vector<vector<state>> states);

vector<vector<state>>  matching1(roadNetwork R, vector<point> traj,int k);
vector<state> viterbi(roadNetwork R, vector<vector<state>> candidates, vector<point> traj);


#endif
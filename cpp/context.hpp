#ifndef FISHING_HPP
#define FISHING_HPP
#include "class.hpp"





class state_c
{
    public:
    //double time;
    point pos;
    point dir;
    point prec_dir;
    double speed;
    int context; // 0 : anchored ; 1 : going to fishing ; 2 : fishing ; 3 : back to home
    state_c();
    state_c( point position,double vitesse,point direction,point precdir,int contexte);

};





double proba_emission_fish(line coast, observation z, observation zAnt, state_c etat);
double proba_emission_base(observation obs, state_c state);
double proba_transi_fish(state_c s1, state_c s2);



/*
double norm_GC(point a, point b);
*/

vector<double> importance_sampling_fish(vector<state_c> S, point z);

vector<state_c> resample_fish(vector<state_c> S, vector<double> w, int n);


state_c update_fishv1(state_c etat, int t);
state_c update_fish(state_c etat, int t,line coast);
state_c update_fishv2(state_c etat, int t,line coast);
state_c update_from_obs(state_c etat, int t, line coast);


vector<state_c> update(vector<state_c> echantillon, int t,line coast);
vector<state_c> update_fishv3(state_c etat, int t,int n);
//vector<state> viterbi_fish(roadNetwork R, vector<vector<state>> candidates, vector<point> traj);


vector<state_c> particle_filter(vector<observation> obs, line coast,int n);
vector<state_c> particle_filterv2(vector<observation> obs, line coast,int n);
vector<state_c> particle_filterv3(vector<observation> obs, line coast, int n);
vector<int> lissage(vector<state_c> context,int seuil);
vector<double> speed(vector<observation> obs);

vector<point> metersFromStart(vector<point> gpx);

void outputv2(char* arg,vector<state_c> states, vector<point> pointss,vector<int> label);

state_c update_city(state_c etat, int t,vector<point> station);
vector<state_c> update_city(vector<state_c> echantillon, int t,vector<point> station);
state_c update_cityv2(state_c etat, int t);
vector<state_c> update_cityv3(state_c etat, int t,int n,vector<point> station);
double distance_from_station(point pos,vector<point> station);
vector<double> importance_sampling_city(vector<state_c> S, point z);
vector<state_c> particle_filter_city(vector<observation> obs, vector<point> station,int n);
vector<state_c> particle_filterv_cityv3(vector<observation> obs,vector<point> station,int n);
vector<int> SavitzkyGolay(vector<int> context, int l);

vector<state_c> process_tram(vector<state_c> states,vector<road> rails);
#endif
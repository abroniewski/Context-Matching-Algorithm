#ifndef FISHING_HPP
#define FISHING_HPP
#include "class.hpp"





class state_fish
{
    public:
    //double time;
    point pos;
    point dir;
    point prec_dir;
    double speed;
    int context; // 0 : anchored ; 1 : going to fishing ; 2 : fishing ; 3 : back to home
    state_fish();
    state_fish( point position,double vitesse,point direction,point precdir,int contexte);

};





double proba_emission_fish(line coast, observation z, observation zAnt, state_fish etat);
double proba_emission_base(observation obs, state_fish state);
double proba_transi_fish(state_fish s1, state_fish s2);



/*
double norm_GC(point a, point b);
*/

vector<double> importance_sampling_fish(vector<state_fish> S, point z);

vector<state_fish> resample_fish(vector<state_fish> S, vector<double> w, int n);


state_fish update_fishv1(state_fish etat, int t);
state_fish update_fish(state_fish etat, int t,line coast);
state_fish update_fishv2(state_fish etat, int t,line coast);
state_fish update_from_obs(state_fish etat, int t, line coast);


vector<state_fish> update(vector<state_fish> echantillon, int t,line coast);
vector<state_fish> update_fishv3(state_fish etat, int t,int n);
//vector<state> viterbi_fish(roadNetwork R, vector<vector<state>> candidates, vector<point> traj);


vector<state_fish> particle_filter(vector<observation> obs, line coast,int n);
vector<state_fish> particle_filterv2(vector<observation> obs, line coast,int n);
vector<state_fish> particle_filterv3(vector<observation> obs, line coast, int n);
vector<int> lissage(vector<state_fish> context,int seuil);
vector<double> speed(vector<observation> obs);

vector<point> metersFromStart(vector<point> gpx);

void outputv2(char* arg,vector<state_fish> states, vector<point> pointss,vector<int> label);

state_fish update_city(state_fish etat, int t,vector<point> station);
vector<state_fish> update_city(vector<state_fish> echantillon, int t,vector<point> station);
state_fish update_cityv2(state_fish etat, int t);
vector<state_fish> update_cityv3(state_fish etat, int t,int n,vector<point> station);
double distance_from_station(point pos,vector<point> station);
vector<double> importance_sampling_city(vector<state_fish> S, point z);
vector<state_fish> particle_filter_city(vector<observation> obs, vector<point> station,int n);
vector<state_fish> particle_filterv_cityv3(vector<observation> obs,vector<point> station,int n);
vector<int> SavitzkyGolay(vector<int> context, int l);

vector<state_fish> process_tram(vector<state_fish> states,vector<road> rails);
#endif
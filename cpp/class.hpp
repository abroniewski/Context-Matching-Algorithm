#ifndef CLASS_HPP
#define CLASS_HPP
#include <vector>
#include <iostream>

using namespace std;

/*############################################################################################*/

int arg_max(std::vector<double> const& vec);



class point
{
    public:
    //Attributs
    double x;
    double y;
    //Fonctions
    point(double a=0, double b=0);
    double arg();
    point operator +();
    point operator -();
};



point operator + (point S1, point S2);
point operator - (point S1, point S2);
point operator * (double lambda, point u);
ostream & operator << (ostream & out, point S);

double operator | (point S1, point S2);
bool operator == (point S1, point S2);
bool operator != (point S1, point S2);
double norm(point u);
double norm_GC(point a, point b);
point GC_to_meters(point a, point b);
point meters_to_GC(point un, point ref);

point intersect(point A, point B, point z);

/*############################################################################################*/



class line
{
    public:
    //Attributs
    vector<point> points;
    int taille;
    //Fonctions
    line();
    line(vector<point> Points_init);
};

point findx(line roadline, point z);

int findIndx(line roadline, point z);

/*############################################################################################*/

class road
{
    public:
    //Attributs
    string roadID;
    string fromNodeID;
    string toNodeID;
    bool twoWays;
    int speed;
    int vertexCount;
    line lineString;
    double Length;
    //Fonctions
    road();
    road(string roadid,string fromnodeid, string tonodeid, bool twoways, int spid, int vertexcount, line linestring);

};

/*############################################################################################*/

class roadNetwork
{
    public :
    //Attributs
    vector<road> roads;
    vector<string> Refroads;

    vector<string> Refnodes;
    vector<point> nodes;


    vector<vector<double>> Adjacence;
    vector<vector<int>> VoisinRoad;
    int taille;
    //Fonctions
    roadNetwork();
    roadNetwork(vector<road> roadz);
    double dist(int i, int j);
};

/*############################################################################################*/


class match
{
    public:
    road edge;
    double distance;
    match();
    match(road e, double d);
    match(road e, point z);

};


ostream & operator << (ostream & out, match M);
/*############################################################################################*/

class state
{
    public:
    //Attributs
    point x;
    road edge;
    double distance;
    point dir;

    double velocity;
    int context;
    double error;
    double parking_lot;
    double bus_stop ;
    //Fonctions
    state(point xx, road e, double v, double o);
    state(road e, double d,double v,double o);
    state(match matc, double v);
    state();
    void set_bus(line bus);
    void set_park(line park);
};







/*############################################################################################*/

double distZ_X(line roadline, point z);
double probamatch(point z, line road);
double distancetransi(roadNetwork R, point z0, point z1, state xi, state xj);
double probatransi(roadNetwork r,point z0, point z1, state xi, state xj);
double proba_emission_base(point obs, point state);

/*###########################################################################################*/


double normRoadelem(line road);
double normRoadelem(line road, point z, bool dir);
double normRoad(point x0, point x1, line roadi, line roadj);
double normRoadstate(roadNetwork Roads, state i, state j);

/*###########################################################################################*/

vector<state> KNN(vector<road> roads, point z, int k);

/*###########################################################################################*/

int findIndex(const vector<string> &T, string elem);

vector<road> box(roadNetwork R, point z,double seuil);

roadNetwork constructfrominput(vector<string> refnodes, vector<point> nodes, vector<string> refways, vector<vector<string>> ways);



class observation
{
    public:
    point pos;
    int time;
    observation(point z,int t);
};

#endif
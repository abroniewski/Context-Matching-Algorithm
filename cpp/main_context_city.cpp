/*
#include "class.hpp"
#include <vector>
#include <iostream>
#include <fstream>
#include <cmath>
#include "io.hpp"
#include "detection_mode.hpp"
#include <random>
#include <iomanip>
*/

#include "context.hpp"
#include "io.hpp"
#include "class.hpp"
#include <ctime>
#include <random>


int main(){
    srand(time(NULL));
    default_random_engine generator;

    /*
    line road1(vector<point>({point(-20,0), point(-10,0)}));
    line road2(vector<point>({point(-10,0), point(0,0)}));
    line road3(vector<point>({point(0,0), point(10,0)}));
    line road4(vector<point>({point(10,0), point(20,0)}));

    line road5(vector<point>({point(-10,0), point(-10,10)}));
    line road6(vector<point>({point(0,0), point(0,10)}));
    line road7(vector<point>({point(10,0), point(10,10)}));

    line road8(vector<point>({point(-20,10), point(-10,10)}));
    line road9(vector<point>({point(-10,10), point(0,10)}));
    line road10(vector<point>({point(0,10), point(10,10)}));
    line road11(vector<point>({point(10,10), point(20,10)}));

    road Road1("1","0","1",true,10,0,road1);
    road Road2("2","1","2",true,10,0,road2);
    road Road3("3","2","3",true,10,0,road3);
    road Road4("4","3","4",true,10,0,road4);

    road Road5("2","1","6",true,10,0,road5);
    road Road6("6","2","7",true,10,0,road6);
    road Road7("7","3","8",true,10,0,road7);

    road Road8("8","5","6",true,10,0,road8);
    road Road9("9","6","7",true,10,0,road9);
    road Road10("10","7","8",true,10,0,road10);
    road Road11("11","8","9",true,10,0,road11);

    roadNetwork roads(vector<road>({Road1,Road2,Road3,Road4,Road5,Road6,Road7,Road8,Road9,Road10,Road11}));
    */
    
    roadNetwork roads =read("./osm_files/bruxelles_rail.osm");
    vector<point> station=read_stops("./osm_files/bruxelles_trams_stops.osm");
    point ref(4.36,50.84);
    station.push_back(GC_to_meters(point(4.384045,50.814229),ref));


    vector<observation> obs=read_csv("./gpx_files/GPX_points_22_07.csv",1,0,6);
    //vector<observation> obs=read_csv("./gpx_files/GPX_points_05_08.csv",4,5,9);

    /*
    observation A(point(-15.1,0),1);
    observation B(point(-7,-1),2);
    observation C(point(1,5),3);
    observation D(point(10,4),3);
    vector<observation> obs({A,B,C,D});
    */
   cout<<"taille de obs : "<<obs.size()<<endl;

    point init = (1,1);
    line coast(vector<point>({init}));
    
    
    vector<state_c> states=particle_filterv_cityv3(obs,station,100); //100
    vector<int> contexte(0);
    vector<string> colorChoice({"blue","red","yellow","green","pink"});
    vector<point> outputJson(0);

    for(int i=0;i<states.size();++i){
        contexte.push_back(states[i].context);
    }
    //contexte=SavitzkyGolay(contexte,3);
    for(int i=0;i<states.size();++i){
        
        states[i].context=contexte[i];
        
    }
    
    int deb=time(nullptr);
    states=process_tram(states,roads.roads);
    cout<<"temps "<<time(nullptr)-deb<<endl;
    //vector<state> states=particle_filter(obs2,10,roads);
    vector<point> marche;
    vector<point> tram;
    for(int i=0;i<states.size();++i){
        
        if(contexte[i]==2){
            tram.push_back(states[i].pos);
            
        }else{
            marche.push_back(states[i].pos);
        }
        states[i].pos=meters_to_GC(states[i].pos,ref);
        outputJson.push_back(states[i].pos);
    }
    for(int i=0;i<station.size();++i){
        station[i]=meters_to_GC(station[i],ref);
        contexte.push_back(4);
        outputJson.push_back(station[i]);
    }
    vector<point> nul;
    write("output.txt",roads,marche,tram,station);
    write_to_json("outputjson.txt",outputJson,contexte,colorChoice);
    

    return(0);
}
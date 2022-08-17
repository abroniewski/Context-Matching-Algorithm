
#include "context.hpp"
#include "io.hpp"
#include "class.hpp"
#include <ctime>
#include <random>


int main(){
    srand(time(NULL));
    default_random_engine generator;

    roadNetwork roads =read("./osm_files/bruxelles_rail.osm",0);
    vector<point> station=read_stops("./osm_files/bruxelles_trams_stops.osm");
    point ref(4.36,50.84);
    station.push_back(GC_to_meters(point(4.384045,50.814229),ref));

    int lat =1;
    int lon=0;
    int t=6;
    vector<observation> obs=read_csv("./gpx_files/GPX_points_22_07.csv",lat,lon,t,1,0);
    //vector<observation> obs=read_csv("./gpx_files/GPX_points_05_08.csv",4,5,9,1,0);

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
    contexte=SavitzkyGolay(contexte,3);
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
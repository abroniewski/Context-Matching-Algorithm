#include "class.hpp"
#include <vector>
#include <iostream>
#include <fstream>
#include <cmath>
#include "io.hpp"
#include "matching.hpp"
#include <string>

int main(){



    roadNetwork Map =read("osm_files/bruxelles3.osm",1);
    
    vector<point> points=read_gpx("gpx_files/2022-06-15-short.gpx");
    cout<<"end read gpx"<<endl;
    vector<point> match{};
    vector<point> bus{};
    cout<<"obs size : : "<<points.size()<<endl;
    vector<point> pointss;
    for(int i=0;i<points.size();++i){
        pointss.push_back(points[i]);
    }

    vector<vector<state>> candidates =matching1(Map,pointss, 2);
    cout<<"candidates found"<<endl;
    vector<state> result=viterbi(Map,candidates,pointss);
    for(int i=0;i<result.size();++i){
        match.push_back(result[i].x);
    }
    
    write("output.txt",Map,points,match,bus);
    cout<<"fin"<<endl;


    return(0);
}
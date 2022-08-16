#include "class.hpp"
#include "dijkstra.hpp"
#include <vector>
#include <iostream>
#include <fstream>
#include <cmath>
#include "io.hpp"
#include "detection_mode.hpp"
#include "matching.hpp"
#include <string>
//#include "proba.hpp"

int main(){
    
    /*
    point A(0,0);
    point B(2,0);
    point C(2,3);
    line road1(vector<point>({A,B,C}));
    line road2(vector<point>({point(1,1), point(1.5,1.5), point(2,3)}));
    line road3(vector<point>({point(2,3), point (3,4)}));
    road Road1(1,0,1,true,10,0,road1);
    road Road2(2,2,3,false,20,1,road2);
    road Road3(2,2,3,false,20,1,road3);
    roadNetwork roads(vector<road>({Road1,Road2,Road3}));
    point z1(-1,0.5);
    point x1 = findx(road1,z1);
    point z2 (3,4.1);
    point x2 = findx(road3,z2);
    vector<point> points {z1,x1,z2,x2};
    */
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



    point z1(-4,3.5);
    point x1 = findx(road2,z1);
    point z2 (3,4.1);
    point x2 = findx(road3,z2);
    

    /*
    vector<point> obs(0);

    vector<state> C=KNN(roads,z1,0);
    for(int i=0;i<C.size();++i){
        cout<<"point :"<<C[i].x<<" distance : "<<norm(C[i].x-z1)<<endl;
        obs.push_back(C[i].x);
        cout<<"proba : "<<probamatch(z1,C[i].edge.lineString)<<endl<<endl;

    }
    vector<point> match{z1,z2};
    vector<point> bus {point(12,10)};
    */
    /*
    vector<point> obs {z1,z2};
    vector<point> match {x1,x2};
    vector<point> bus {point(12,10)};
    */
   

    



    

    //write("output.txt",roads,obs,match,bus);
    double p = probamatch(z1,road1);
    //cout<<p<<endl;
    //cout<<normRoadelem(road1,point(1.1,0),false)<<endl;
    //cout<<normRoadelem(road3)<<endl;
    //cout<<normRoad(z1,z2,road1,road2);
    state A(point(-15,0), Road1, 0.1, 0.1);
    state B(point(14,0), Road11, 0.1, 0.1);
    
    //dijkstra(roads,2);
    cout<<roads.nodes.size()<<endl;
    for(int i=0;i<roads.Refnodes.size();++i){
        cout<<"ID : "<<roads.Refnodes[i]<<", point : "<<roads.nodes[i]<<endl;
    }

    
    
    cout<<normRoadstate(roads,B,A)<<endl;


    //read("map2.osm");
    
    roadNetwork Map =read("../bruxelles3.osm");
    
    
    //cout<<points.size()<<endl;
    vector<point> points=read_gpx("gpx_files/2022-06-15-short.gpx");
    cout<<"end read gpx"<<endl;
    vector<point> match{};
    vector<point> bus{};
    //cout<<"taille du gpx : "<<points.size()<<endl;
    //cout<<points[205]<<endl;
    //vector<road> rr=matching1(roads, bus);
    vector<vector<state>> candidates =matching1(Map,points);
    cout<<"candidates found"<<endl;
    vector<state> result=viterbi(Map,candidates,points);
    for(int i=0;i<result.size();++i){
        match.push_back(result[i].x);
    }
    /*
    for(int i=0;i<points.size();++i){
        box(Map,points[0],0.001);
        cout <<"step "<<i<<endl;

    }
    */
    //box(Map,points[0],0.005);
    
    //vector<road> rr=matching1(Map,points);

    
    
    write("output.txt",Map,points,match,bus);
    cout<<"fin"<<endl;

    

    return(0);
}
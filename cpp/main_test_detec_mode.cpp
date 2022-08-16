#include "class.hpp"
#include <vector>
#include <iostream>
#include <fstream>
#include <cmath>
#include "io.hpp"
#include "detection_mode.hpp"
#include <random>
#include <iomanip>


int main(){
    srand(time(0));
    cout<<"test_detec_mode"<<endl;
    line road1(vector<point>({point(-20,0), point(-17,-0.5), point(-13,0.5), point(-10,0)}));
    road Road1("1","0","1",true,10,0,road1);
    roadNetwork roads(vector<road>({Road1}));


    point A(-19,0.1);
    point B(-19.7,0.2);
    point C(-19.1,-0.1);
    point x1=findx(road1,A);
    point x2=findx(road1,B);
    point x3=findx(road1,C);
    point z(-17,-0.4);
    state SA(x1,Road1,2,0);
    state SB(x2,Road1,2,0);
    state SC(x3,Road1,2,0);
    vector<state> S({SA,SB,SC});
    
    //vector<point> match {x1,x2,x3};
    //vector<point> bus {point(-18,0)};

    vector<state> S0=update(S,0.,roads);
    vector<double> w=importance_sampling(S0,x1);
    vector<state> S1= resample(S0,w,5);
    vector<state> S2=update(S1,0.,roads);
    w=importance_sampling(S2,z);
    vector<state> S3= resample(S2,w,1);
    



    //cout<<nouv.x<<endl;
    vector<point> obs{z};

    vector<point> up;
    for(int i=0;i<S2.size();++i){
        up.push_back(S2[i].x);
    }
    vector<point> resam;
    for(int i=0;i<S3.size();++i){
        resam.push_back(S3[i].x);

    }
    cout<<resam.size()<<endl;

 




    write("output.txt",roads,obs,up,resam);
    

    //cout<<S2<<endl;
    //cout<<S3<<endl;
    

    //cout<<nouvS<<endl;
    //cout<<findIndx(road1,x1)<<endl;

    return(0);
}
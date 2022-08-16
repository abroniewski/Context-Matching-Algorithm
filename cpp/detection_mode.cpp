#include "class.hpp"
#include "detection_mode.hpp"
#include "dijkstra.hpp"
#include<iostream>
#include<stdlib.h>
#include <vector>
#include <cmath>
#include <random>
#include<cstdlib>
#include <array>
#include <time.h>


using namespace std;
constexpr double EPSILON=0.4;
/*
std::default_random_engine generator;
*/
std::random_device rd;

std::mt19937 gen(rd());

using normal_dist   = std::normal_distribution<>;
using discrete_dist = std::discrete_distribution<std::size_t>;
auto Gwalk = std::array<normal_dist, 2>{
        normal_dist{1.38647,1.02194}, // mean, stddev of G[0]
        normal_dist{1.330367,0.136656}, // mean, stddev of G[1]
    };
auto wwalk = discrete_dist{
        0.52715, // weight of G[0]
        1-0.52715 // weight of G[1]
    };

auto Gtram = std::array<normal_dist, 2>{
        normal_dist{6.3,3.2}, // mean, stddev of G[0]
        normal_dist{2.755783,1.706894}, // mean, stddev of G[1]
    };
auto wtram = discrete_dist{
        1, // weight of G[0]
        0 // weight of G[1]
    };



constexpr int MIN_SPEED = 0;
constexpr int MAX_SPEED = 20;
uniform_int_distribution<> distrK(0,5);
uniform_int_distribution<> boul(0,1);

sample::sample(){
    vector<state> Null({});
    states =Null;
    taille=0; 
}

sample::sample(vector<state> states_init){
    //initialisation d'une ligne de route avec ses points
    states = states_init;
    taille = (int)states.size();
}

sample::sample(int n){
    taille=n;
    vector<state> Null(n,state(road(),0,0,0));
    states=Null;


}


ostream & operator << (ostream & out, state S){
    out <<'(' << S.edge.roadID << ","  <<S.distance << "," << S.velocity << ")";
    return(out);
}

/*#############################################################################################*/

vector<double> importance_sampling(vector<state> S, point z){
    vector<double> w(S.size(),0);
    for(int i =0;i<S.size();++i){
        w.at(i)=10000*proba_emission_base(z ,S.at(i).x);
    }
    return(w);
}


vector<state> resample(vector<state> S, vector<double> w, int n){
    double T=0;
    vector<state> newS(0);
    vector<double> k(w.size(),0);
    for(int i=0;i<w.size();++i){
        T+=w.at(i);
        k.at(i)=T;
    }
    
    for(int j=0;j<n;j++){
        
        double t=rand()*T/RAND_MAX;

        int i=0;
        while(k.at(i)<t){
            i+=1;
        }
        newS.push_back(S.at(i));
    }
    

    return(newS);
}


state translation(state etat, double d,roadNetwork Roads){
    vector<point> chemin(0);
    road edge=etat.edge;
    road newedge;
    int signe=1;
    //int signe =boul(gen)*2-1;
    if(d+etat.distance>etat.edge.Length){
        
       
        for(int k=0;k<edge.lineString.points.size();++k){
            chemin.push_back(edge.lineString.points[k]);
        }
        double Grandereserve=d-signe*edge.Length;
        do{
            int indx=findIndex(Roads.Refroads,edge.roadID);
            if(Roads.VoisinRoad[indx].size()==0){
                return(etat);
            }
            uniform_int_distribution<> distr(0,Roads.VoisinRoad[indx].size()-1);
            //cout<<"avant distrib"<<endl;
            //cout<<"distr "<<distr(gen)<<endl;

            newedge=Roads.roads[Roads.VoisinRoad[indx][distr(gen)]];
            //cout<<"apres distrib"<<endl;
            if(newedge.lineString.points.size()>=2){
                for(int k=1;k<newedge.lineString.points.size();++k){
                    chemin.push_back(newedge.lineString.points[k]);
                }
            }
            Grandereserve=Grandereserve-signe*newedge.Length;
            edge=newedge;
        }while(Grandereserve>0);
            
    }else{
        chemin=etat.edge.lineString.points;
    }
    for(int z=0;z<chemin.size();++z){
        //cout<<z<< " : "<<chemin[z]<<endl;
    }
    //cout<<endl;
   
    int j=0;
    double Reserv=d;
    while(Reserv>0){
        if(Reserv>norm(chemin[j+1]-etat.x)){
            Reserv=Reserv-signe*norm(chemin[j+1]-etat.x);

            j+=1;

        }else{
            point nouveau= findx(edge.lineString, chemin[j]+signe*(Reserv/norm(chemin[j+1]-chemin[j]))*(chemin[j+1]-chemin[j]));
            Reserv=0;
            state etatnouv(nouveau,edge,0.,0.);
            return(etatnouv);
            //return(state(nouveau,edge,0.,0.));

            
            }
        }
    return(etat);

}


state update(state etat,double t){
    

    //double velocity = MIN_SPEED + (double)(rand()) / (((double)(RAND_MAX/(MAX_SPEED - MIN_SPEED))));
    auto index = wtram(gen);
    double newvelocity=Gtram[index](gen);
    
   

    double distance = etat.distance + newvelocity;
    point newdir(etat.dir.x-EPSILON+2*((double)rand()/RAND_MAX)*EPSILON,etat.dir.y-EPSILON+2*((double)rand()/RAND_MAX)*EPSILON);
    newdir=1/(norm(newdir))*newdir;
    point newpos=etat.x+ newvelocity*t*newdir; 
    //state x =KNN(box(Roads,newpos,50),newpos,5)[distrK(gen)];
    etat.x=newpos;
    etat.velocity=newvelocity;
    etat.dir=newdir;
    return(etat);
    
  
    
}

vector<state> update(vector<state> echantillon,double t){
    vector<state> nouveau(0);
    for(int i=0;i<echantillon.size();++i){

        nouveau.push_back(update(echantillon[i],t));
    }
    return(nouveau);
}


vector<state> particle_filter(vector<observation> obs,int n,roadNetwork Roads){
    //initialisation
    vector<state> Sample;
    vector<double> w;
    vector<state> res;
    
    int k;
    for(int i=0;i<n;++i){
        int contexte=(int)((double)rand() / ((double)(RAND_MAX)));
        
        state etat=KNN(box(Roads,obs[0].pos,100),obs[0].pos,5)[distrK(gen)];
        etat.context=contexte;
        cout<<etat.x<<endl;
        Sample.push_back(update(etat,obs[1].time-obs[0].time));
        cout<<Sample[i].x<<endl;       
    }
        w=importance_sampling(Sample,obs[0].pos);
        k=arg_max(w);
        res.push_back(Sample[k]);
    for(int i=0;i<obs.size()-1;++i){
        cout<<"i : "<<i<<endl;
        if(true){
            Sample=update(Sample,obs[i+1].time-obs[i].time);
            w=importance_sampling(Sample,obs[i+1].pos);
            k=arg_max(w);
            /*
            for(int j=0;j<n;++j){
                res.push_back(Sample[j]);
            }
            */
            res.push_back(Sample[k]);
            Sample=resample(Sample,w,n);
            
        }
    }
    return(res);
}
/*
double normRoadstate(roadNetwork Roads, state i, state j){
    double dist=dijkstra(Roads,i.edge.toNodeID)[j.edge.fromNodeID];
    dist=dist+j.distance+(i.edge.Length-i.distance);

    if(i.edge.twoWays==true && j.edge.twoWays==false){
        double dist2=dijkstra(Roads,i.edge.fromNodeID)[j.edge.fromNodeID];
        dist2=dist2+j.distance+i.edge.Length;
        if(dist2<dist){
            dist=dist2;
        }

    }
    if(i.edge.twoWays==false && j.edge.twoWays==true){
        double dist2=dijkstra(Roads,i.edge.toNodeID)[j.edge.toNodeID];
        dist2=dist2+j.edge.Length -j.distance+(i.edge.Length-i.distance);
        if(dist2<dist){
            dist=dist2;
        }

    }
    if(i.edge.twoWays==true && j.edge.twoWays==true){
        double dist2=dijkstra(Roads,i.edge.toNodeID)[j.edge.toNodeID];
        double dist3=dijkstra(Roads,i.edge.fromNodeID)[j.edge.fromNodeID];
        double dist4=dijkstra(Roads,i.edge.fromNodeID)[j.edge.toNodeID];
        dist2=dist2+j.edge.Length -j.distance+(i.edge.Length-i.distance);
        dist3=dist2+j.distance+i.edge.Length;
        dist4=dist4+i.distance+(j.edge.Length-j.distance);
        if(dist2<dist){
            dist=dist2;
        }
        if(dist3<dist){
            dist=dist3;
        }
        if(dist4<dist){
            dist=dist4;
        }

    }
    return(dist);
}
*/
#include <iostream>
#include "class.hpp"
#include <cmath>
#include <random>
#include "io.hpp"
#include <array>
#include "context.hpp"

using namespace std;
std::default_random_engine generator;
std::random_device rd;
std::mt19937 gen(rd());
using normal_dist   = std::normal_distribution<>;
using discrete_dist = std::discrete_distribution<std::size_t>;

auto Gfish = std::array<normal_dist, 2>{
        normal_dist{1.38647,1.02194}, // mean, stddev of G[0]
        normal_dist{1.330367,0.136656}, // mean, stddev of G[1]
    };
auto wfish = discrete_dist{
        0.52715, // weight of G[0]
        1-0.52715 // weight of G[1]
    };

auto Gsail = std::array<normal_dist, 2>{
        normal_dist{3.701302,0.567144}, // mean, stddev of G[0]
        normal_dist{2.755783,1.706894}, // mean, stddev of G[1]
    };
auto wsail = discrete_dist{
        0.610961, // weight of G[0]
        0.389039 // weight of G[1]
    };

auto Gwalk = std::array<normal_dist, 2>{
        //normal_dist{1.38647,1.02194}, // mean, stddev of G[0]
        //normal_dist{1.330367,0.136656}, // mean, stddev of G[1]
        normal_dist{1.491133,0.564024 }, // mean, stddev of G[0]
        normal_dist{5.3134245,3.7900220}, // mean, stddev of G[1]
    };
auto wwalk = discrete_dist{
        0.921517, // weight of G[0]
        0.0784828 // weight of G[1]
    };

auto Gtram = std::array<normal_dist, 2>{
        normal_dist{5.426668,2.361558}, // mean, stddev of G[0]
        normal_dist{12.941190,6.373420}, // mean, stddev of G[1]
    };
auto wtram = discrete_dist{
        0.639265, // weight of G[0]
        0.360735 // weight of G[1]
    };

using uni_distr =std::uniform_real_distribution<double>;
auto prob = uni_distr{
        0, // weight of G[0]
        1 // weight of G[1]
    };



constexpr double MIN_SPEED_FISHING = 0; //2 4
constexpr double MAX_SPEED_FISHING = 2;

constexpr double MIN_SPEED_TRAVEL = 2; //0 3
constexpr double MAX_SPEED_TRAVEL=4;

constexpr double MIN_SPEED_ANCHORED = 0;
constexpr double MAX_SPEED_ANCHORED=1;

constexpr double pi=3.1415;
constexpr double sigma=40; //40 mais avec 6 ça marche aussi ?
constexpr double EPSILON=0.4;

state_fish::state_fish(){
    
}

state_fish::state_fish( point position,double vitesse,point direction,point precdir,int contexte){
    pos=position;
    speed=vitesse;
    dir=direction;
    prec_dir=precdir;
    context=contexte;
}




double proba_emission_fish(line coast, observation z, observation zAnt, state_fish etat){
    double dist=probamatch(z.pos,coast);
    double rapprochement=dist-probamatch(zAnt.pos,coast);
    double speed=norm(z.pos-zAnt.pos)/abs(z.time-zAnt.time);
    if(dist<100 && speed<100){
        return(vector<double>({0.8,0.075,0.075,0.05})[etat.context]);
    }else if(rapprochement>0 && speed>100){
        return(vector<double>({0.1,0.1,0.7,0.1})[etat.context]);
    }else if(rapprochement<0 && speed>100){
        return(vector<double>({0.1,0.7,0.1,0.1})[etat.context]);
    }else{
        return(vector<double>({0.1,0.1,0.1,0.7})[etat.context]);
    }
}

double proba_emission_base(observation obs, state_fish state){
    double denom=(1/sqrt(2*pi*sigma));
    double num=-0.5*pow((norm(obs.pos-state.pos)/sigma),2);
    return(denom*exp(num));
}


double proba_transi_fish(state_fish s1, state_fish s2){
    for(int i=0;i<3;++i){
        if(s1.context==i && (s2.context!=i+1 || s2.context!=i)){
            return(0);
        }
    }
    if(s1.context==3 && (s2.context !=3 || s2.context != 0)){
        return(0);
    }
    if(abs(s1.speed-s2.speed)<10){
        if(s1.context==s2.context){
            return(1);
        }else{
            return(0);
        }
    }
    if(((s1.context==0 &&s2.context==1)||(s1.context==2&&s2.context==3)) && abs(s1.speed-s2.speed)>10){
        return(1);
        
    }
    return(0);

}



/*

double norm_GC(point a, point b){
    double latMid, m_per_deg_lat, m_per_deg_lon, deltaLat, deltaLon,dist_m;

    latMid = (a.y+b.y )/2.0;  // or just use Lat1 for slightly less accurate estimate

    m_per_deg_lat = 111132.954 - 559.822 * cos( 2.0 * latMid ) + 1.175 * cos( 4.0 * latMid);
    m_per_deg_lon = (3.14159265359/180 ) * 6367449 * cos ( latMid );

    deltaLat = fabs(a.y - b.y);
    deltaLon = fabs(a.x - b.x);

    dist_m = sqrt (  pow( deltaLat * m_per_deg_lat,2) + pow( deltaLon * m_per_deg_lon , 2) );
    return(dist_m);
    //return(sqrt(pow((a.y-b.y)*111320,2)+pow(40075000*(a.x-b.x)*cos(a.y-b.y)/360,2)));
}
*/




vector<double> importance_sampling_fish(line coast, vector<state_fish> S, observation obs, observation obsAnt){
    vector<double> w(S.size(),0);
    for(int i =0;i<S.size();++i){
        w.at(i)=100000*proba_emission_base(obs,S[i]);
    }
    return(w);
}


vector<state_fish> resample_fish(vector<state_fish> S, vector<double> w, int n){
    double T=0;
    vector<state_fish> newS(0);
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


state_fish update_fishv1(state_fish etat, int t){
    //modelisation gaussian mixture et seulement 2 etats
    
    //initialisation
    point precd=etat.dir;
    double newvelocity;
    point newpos;
    point newdir(etat.dir.x-EPSILON+2*((double)rand()/RAND_MAX)*EPSILON,etat.dir.y-EPSILON+2*((double)rand()/RAND_MAX)*EPSILON);
    newdir=1/(norm(newdir))*newdir;
    if((etat.context==1 ||etat.context==2) && (double)rand()/RAND_MAX<0.1){
        etat.context+=1;
    }
    if((etat.context==3) && (double)rand()/RAND_MAX<0.1){
        etat.context=2;
    }
    if(etat.context==0){
        //anchored
        if((double)rand()/RAND_MAX<0.8){
            etat.context=1;
        }else{
            return(etat);
        }
        
    }
    if(etat.context==1 || etat.context==3){
        //sail 1

        //newvelocity=distribution_sail(generator);     
        newvelocity=-1.;
        while(newvelocity<0){
            auto index = wsail(gen);
            newvelocity = Gsail[index](gen);
        }
        
        newpos=etat.pos+ newvelocity*t*newdir; // A changer !!
        etat.speed=newvelocity; 

    }
    if(etat.context==2){
        //fishing
        newvelocity=-1.;
        while(newvelocity<0){
            auto index = wfish(gen);
            newvelocity = Gfish[index](gen);
        }
        newpos=etat.pos+ newvelocity*t*newdir; // A changer !!
        etat.speed=newvelocity;

    }

    return(state_fish(newpos,newvelocity,newdir,precd,etat.context));
}

state_fish update_fishv2(state_fish etat, int t,line coast){
    //modelisation gaussian mixture et 3 etats
    
    //initialisation
    point precd=etat.dir;
    double newvelocity;
    point newpos;
    point newdir(etat.dir.x-EPSILON+2*((double)rand()/RAND_MAX)*EPSILON,etat.dir.y-EPSILON+2*((double)rand()/RAND_MAX)*EPSILON);
    newdir=1/(norm(newdir))*newdir;
    if(etat.context==0){
        //anchored
        etat.context=1;
        
    }
    if((double)rand()/RAND_MAX<0.1){
        etat.context=2;
    }
    if((newdir|(etat.pos-coast.points[0]))>0 && ((double)rand()/RAND_MAX<0.1)){
        etat.context=1;
    }
    if((newdir|(etat.pos-coast.points[0]))<0 && ((double)rand()/RAND_MAX<0.1)){
        etat.context=3;
    }
    if(etat.context==1 || etat.context==3){
        //sail 1

        //newvelocity=distribution_sail(generator);     
        newvelocity=-1.;
        while(newvelocity<0){
            auto index = wsail(gen);
            newvelocity = Gsail[index](gen);
        }
        
        newpos=etat.pos+ newvelocity*t*newdir; // A changer !!
        etat.speed=newvelocity;

    }
    if(etat.context==2){
        //fishing
        newvelocity=-1.;
        while(newvelocity<0){
            auto index = wfish(gen);
            newvelocity = Gfish[index](gen);
        }
        newpos=etat.pos+ newvelocity*t*newdir; // A changer !!
        etat.speed=newvelocity;

    }

    return(state_fish(newpos,newvelocity,newdir,precd,etat.context));
}


state_fish update_fish(state_fish etat, int t,line coast){
    //modelisation gaussienne simple et 3 etats
    
    normal_distribution<double> distribution_fish(1.36,0.81); //1.23 0.4
    normal_distribution<double> distribution_sail(3.31,1.19); //2.2 1
    point precd=etat.dir;
    double newvelocity;
    point newpos;
    point newdir(etat.dir.x-EPSILON+2*((double)rand()/RAND_MAX)*EPSILON,etat.dir.y-EPSILON+2*((double)rand()/RAND_MAX)*EPSILON);
    newdir=1/(norm(newdir))*newdir;

    if(etat.context==0){
        //anchored
        etat.context=1;
        
    }
    if((double)rand()/RAND_MAX<0.1){
        etat.context=2;
    }
    if((newdir|(etat.pos-coast.points[0]))>0 && ((double)rand()/RAND_MAX<0.1)){
        etat.context=1;
    }
    if((newdir|(etat.pos-coast.points[0]))<0 && ((double)rand()/RAND_MAX<0.1)){
        etat.context=3;
    }


    if(etat.context==1 || etat.context==3){
        //sail 1 or 2
        newvelocity=distribution_sail(generator);
        newpos=etat.pos+ newvelocity*t*newdir; 
    }
    if(etat.context==2){
        //fishing
        newvelocity=distribution_fish(generator);
        newpos=etat.pos+ newvelocity*t*newdir; // A changer !!
    }
    return(state_fish(newpos,newvelocity,newdir,precd,etat.context));

}

vector<state_fish> update_fish(vector<state_fish> echantillon, int t,line coast){
    vector<state_fish> nouveau(echantillon.size());
    for(int i=0;i<echantillon.size();++i){

        //nouveau[i]=update_fish(echantillon[i],t,coast);
        nouveau[i]=update_fishv1(echantillon[i],t); //ligne à changer pour changer de méthode !
    }
    return(nouveau);
}




vector<state_fish> particle_filter(vector<observation> obs, line coast,int n){
    //initialisation
    vector<state_fish> Sample;
    vector<double> w;
    vector<state_fish> res;
    int k;
    for(int i=0;i<n;++i){
        point direction=point((double)rand() / ((double)(RAND_MAX)),(double)rand() / ((double)(RAND_MAX)));
        int contexte=(int)((double)rand() / ((double)(RAND_MAX)));
        Sample.push_back(update_fish(state_fish(obs[0].pos,0, direction,direction,1),obs[1].time-obs[0].time,coast));
    }
    //Sample=update_fish(Sample);
    for(int i=0;i<obs.size()-1;++i){
        if(true){
        //if(obs[i+1].time-obs[i].time<100){
            Sample=update_fish(Sample,obs[i+1].time-obs[i].time,coast);
            w=importance_sampling_fish(coast,Sample,obs[i+1],obs[i]);
            k=arg_max(w);
            res.push_back(Sample[k]);
            Sample=resample_fish(Sample,w,n);
            
        }
    }
    return(res);
}

vector<state_fish> particle_filterv2(vector<observation> obs, line coast,int n){
        //initialisation
    vector<state_fish> Sample;
    vector<double> w;
    vector<state_fish> res;
    int kk;
    int kontext;
    for(int i=0;i<n;++i){
        point direction=point((double)rand() / ((double)(RAND_MAX)),(double)rand() / ((double)(RAND_MAX)));
        int contexte=(int)((double)rand() / ((double)(RAND_MAX)));
        Sample.push_back(update_fish(state_fish(obs[0].pos,0, direction,direction,1),obs[1].time-obs[0].time,coast));
    }
    //Sample=update_fish(Sample);
    
    for(int i=0;i<obs.size()-1;++i){

        if(true){
        //if(obs[i+1].time-obs[i].time<100){
            Sample=update_fish(Sample,obs[i+1].time-obs[i].time,coast);
            w=importance_sampling_fish(coast,Sample,obs[i+1],obs[i]);
            Sample=resample_fish(Sample,w,n);
            vector<double> context(4,0);
            for(int k=0;k<Sample.size();++k){
                context[Sample[k].context]+=1;
            }
            kontext=arg_max(context);
            kk=arg_max(w);
            Sample[kk].context=kontext;
            res.push_back(Sample[kk]);
            //Sample=resample_fish(Sample,w,n);
            
        }
    }
    return(res);
    
}


vector<state_fish> update_fishv3(state_fish etat, int t,int n,line coast){
    vector<state_fish> newS;
    for(int i=0;i<n;++i){
        newS.push_back(update_fish(etat,t,coast));

    }
    return(newS);
}



vector<state_fish> particle_filterv3(vector<observation> obs,line coast,int n){
        //initialisation
    vector<state_fish> Sample;
    vector<double> w;
    vector<state_fish> res;
    int kk;
    int kontext=0;
    state_fish etat;
    int t=1;

    
    for(int i=0;i<obs.size()-1;++i){
        t=obs[i+1].time-obs[i].time;
        etat.context=kontext;
        etat.pos =obs[i].pos;
        etat.speed=norm(obs[i+1].pos-obs[i].pos)/t; 
        etat.dir=(t/etat.speed)*(obs[i+1].pos-obs[i].pos);
        res.push_back(etat);
        Sample=update_fishv3(etat,t,n,coast);
        w=importance_sampling_city(Sample,obs[i+1].pos);
        Sample=resample_fish(Sample,w,n);
        vector<double> context(4,0);
        for(int k=0;k<Sample.size();++k){
            context[Sample[k].context]+=1;
        }
        kontext=arg_max(context);     
    }
    return(res);
    
}


vector<double> speed(vector<observation> obs){
    vector<double> speed;
    for(int i=0;i<obs.size();++i){
        speed.push_back(norm(obs[i+1].pos-obs[i].pos)/(obs[i+1].time-obs[i].time));
    }
    return(speed);
}

vector<point> metersFromStart(vector<point> gpx){
    vector<point> res;
    point start=gpx[0];
    for(int i=0;i<gpx.size();++i){
        res.push_back(GC_to_meters(gpx[i],gpx[0]));
    }
    return(res);
}

void outputv2(char* arg, vector<state_fish> states,vector<point> pointss,vector<int> label){
    vector<point> match_sail_1;
        vector<point> match_fish_;
        vector<point> match_sail_2;
        vector<point> fish_;
        vector<point> sail_1;
        vector<point> sail_2;
        vector<int> rw(9,0);

        for (int i = 0; i < states.size(); ++i){
            if (states[i].context == 2){
                match_fish_.push_back(states[i].pos);
            }
            else if (states[i].context == 1){
                match_sail_1.push_back(states[i].pos);
            }
            else if (states[i].context == 3){
                match_sail_2.push_back(states[i].pos);
            }
        }
        
        for (int i = 0; i < pointss.size(); ++i){
            if (label[i] == 1){
                sail_1.push_back(pointss[i]);
                if (states[i].context == 1){
                    rw[0]+=1;
                }else if (states[i].context == 2){
                    rw[1]+=1;
                }else{
                    rw[2]+=1;
                }
            }
            else if (label[i] == 2){
                fish_.push_back(pointss[i]);
                if (states[i].context == 1){
                    rw[3]+=1;
                }else if (states[i].context == 2){
                    rw[4]+=1;
                }else{
                    rw[5]+=1;
                }
            }
            else if (label[i] == 3){
                sail_2.push_back(pointss[i]);
                if (states[i].context == 1){
                    rw[6]+=1;
                }else if (states[i].context == 2){
                    rw[7]+=1;
                }else{
                    rw[8]+=1;
                }
            }
        }
        write_boatv2(arg, sail_1, fish_, sail_2, match_sail_1, match_fish_, match_sail_2, rw);
        /*
        cout<<"      |msail1 | mfish | msail2 |"<<endl;
        cout<<"sail1 |  "<<rw[0]<<" | "<<rw[1]<<" | "<<rw[2]<<" | "<<endl;
        cout<<"fish  |  "<<rw[3]<<" | "<<rw[4]<<" | "<<rw[5]<<" | "<<endl;
        cout<<"sail2 |  "<<rw[6]<<" | "<<rw[7]<<" | "<<rw[8]<<" | "<<endl;
        */
       for(int k=0;k<9;++k){
        cout<<rw[k]<<";";
       }
       cout<<endl;
    }


    vector<int> lissage(vector<state_fish> context,int seuil){
        int sail1;
        int fish;
        int sail2;
        vector<int> res(0);
        for(int i=0;i<context.size()/seuil;++i){
            sail1=0;
            fish=0;
            sail2=0;
            for(int j=i*seuil;j<(i+1)*seuil;++j){
                if(context[j].context==1){
                    sail1+=1;
                }else if(context[j].context==2){
                    fish+=1;
                }else if(context[j].context==3){
                    sail2+=1;
                }
            }
            if(sail1>=fish && sail1>=sail2){
                for(int j=i*seuil;j<(i+1)*seuil;++j){
                    res.push_back(1);
                }
            }else if(sail2>=fish && sail2>=sail1){
                for(int j=i*seuil;j<(i+1)*seuil;++j){
                    res.push_back(2);
                }
            }else{
                for(int j=i*seuil;j<(i+1)*seuil;++j){
                    res.push_back(3);
                }
            }


        }
        return(res);

    }

vector<double> importance_sampling_city(vector<state_fish> S, point z){
    vector<double> w(S.size(),0);
    for(int i =0;i<S.size();++i){
        w.at(i)=10000*proba_emission_base(z ,S[i].pos);
    }
    return(w);
}





double distance_from_station(point pos,vector<point> station){
    double dist=norm(pos-station[0]);
    for(int i=1;i<station.size();++i){
        if(norm(pos-station[i])<dist){
            dist=norm(pos-station[i]);
        }
    }
    return(dist);
}


state_fish update_city(state_fish etat, int t,vector<point> station){
    //simplest update version that only take into account speed
    
    //initialisation
    point precd=etat.dir;
    double newvelocity;
    point newpos;
    point newdir(etat.dir.x-EPSILON+2*((double)rand()/RAND_MAX)*EPSILON,etat.dir.y-EPSILON+2*((double)rand()/RAND_MAX)*EPSILON);
    newdir=1/(norm(newdir))*newdir;
    if((etat.context==1)){
    //if((etat.context==0 || etat.context==2) &&( (double)rand()/RAND_MAX<0.1 && distance_from_station(etat.pos,station)<10)){
        if(prob(gen)<0.2){
            etat.context=0;
        }else if(prob(gen)<0.5 && distance_from_station(etat.pos,station)<30){
            etat.context=2;
        }
    }else if((etat.context==0)){
        if(prob(gen)<0.5 && distance_from_station(etat.pos,station)<30 ){
            etat.context=2;
        }else if(prob(gen)<0.5 && distance_from_station(etat.pos,station)<30 ){
            etat.context=1;
        }
    }else if((etat.context==2 && distance_from_station(etat.pos,station)<30)){
        if(prob(gen)<0.5){
            etat.context=1;
        }else if(prob(gen)<0.5){
            etat.context=0;
        }

    //if((etat.context==2) &&( (double)rand()/RAND_MAX<0.1 && distance_from_station(etat.pos,station)<10)){
    }else if(etat.context==3){
        if(prob(gen)<0.2){
            etat.context=1;
        }
    }




    if(etat.context==1){
        return(etat);
    }


    if(etat.context==0 || etat.context==3){
        //walking

        newvelocity=-1.;
        while(newvelocity<0){
            auto index = wwalk(gen);
            newvelocity = Gwalk[index](gen);
        }
        
        newpos=etat.pos+ newvelocity*t*newdir; // A changer !!
        etat.speed=newvelocity;

    }
    if(etat.context==2){
        //tramway
        newvelocity=-1.;
        while(newvelocity<0){
            auto index = wtram(gen);
            newvelocity = Gtram[index](gen);
        }
        newpos=etat.pos+ newvelocity*t*newdir; // A changer !!
        etat.speed=newvelocity;

    }

    return(state_fish(newpos,newvelocity,newdir,precd,etat.context));
}


state_fish update_cityv2(state_fish etat, int t){
    //simplest update version that only take into account speed
    
    //initialisation
    point precd=etat.dir;
    double newvelocity;
    point newpos;
    point newdir(etat.dir.x-EPSILON+2*((double)rand()/RAND_MAX)*EPSILON,etat.dir.y-EPSILON+2*((double)rand()/RAND_MAX)*EPSILON);
    newdir=1/(norm(newdir))*newdir;
    if((etat.context==1 ||etat.context==2) && (double)rand()/RAND_MAX<0.1){
        etat.context+=1;
    }
    if((etat.context==3) && (double)rand()/RAND_MAX<0.1){
        etat.context=2;
    }
    if(etat.context==0){
        //anchored
        if((double)rand()/RAND_MAX<0.8){
            etat.context=1;
        }else{
            return(etat);
        }
        
    }
    if(etat.context==1 || etat.context==3){
        //sail 1

        //newvelocity=distribution_sail(generator);     
        newvelocity=-1.;
        while(newvelocity<0){
            auto index = wtram(gen);
            newvelocity = Gtram[index](gen);
        }
        
        newpos=etat.pos+ newvelocity*t*newdir; // A changer !!
        etat.speed=newvelocity; 

    }
    if(etat.context==2){
        //fishing
        newvelocity=-1.;
        while(newvelocity<0){
            auto index = wwalk(gen);
            newvelocity = Gwalk[index](gen);
        }
        newpos=etat.pos+ newvelocity*t*newdir; // A changer !!
        etat.speed=newvelocity;

    }

    return(state_fish(newpos,newvelocity,newdir,precd,etat.context));
}

vector<state_fish> update_city(vector<state_fish> echantillon, int t,vector<point> station){
    vector<state_fish> nouveau(echantillon.size());
    for(int i=0;i<echantillon.size();++i){

        nouveau[i]=update_city(echantillon[i],t,station); //ligne à changer pour changer de méthode !
        //nouveau[i]=update_cityv2(echantillon[i],t); 
        
    }
    return(nouveau);
}

vector<state_fish> update_cityv3(state_fish etat, int t,int n,vector<point> station){
    vector<state_fish> newS;
    for(int i=0;i<n;++i){
        newS.push_back(update_city(etat,t,station));

    }
    return(newS);
}



vector<state_fish> particle_filter_city(vector<observation> obs, vector<point> station,int n){
        //initialisation
    vector<state_fish> Sample;
    vector<double> w;
    vector<state_fish> res;
    int kk;
    int kontext;
    for(int i=0;i<n;++i){
        point direction=point((double)rand() / ((double)(RAND_MAX)),(double)rand() / ((double)(RAND_MAX)));
        int contexte=1;
        Sample.push_back(update_city(state_fish(obs[0].pos,0, direction,direction,1),obs[1].time-obs[0].time,station));
    }
    //Sample=update_fish(Sample)
    line coast;
    
    for(int i=0;i<obs.size()-1;++i){

        if(true){
        //if(obs[i+1].time-obs[i].time<100){
            //Sample=update_fish(Sample,obs[i+1].time-obs[i].time,coast);
            Sample=update_city(Sample,obs[i+1].time-obs[i].time,station);
            w=importance_sampling_fish(coast,Sample,obs[i+1],obs[i]);
            Sample=resample_fish(Sample,w,n);
            vector<double> context(4,0);
            for(int k=0;k<Sample.size();++k){
                context[Sample[k].context]+=1;
            }
            kontext=arg_max(context);
            kk=arg_max(w);
            Sample[kk].context=kontext;
            res.push_back(Sample[kk]);
            //Sample=resample_fish(Sample,w,n);
            
        }
    }
    return(res);
    
}

vector<state_fish> particle_filterv_cityv3(vector<observation> obs,vector<point> station,int n){
        //initialisation
    vector<state_fish> Sample;
    vector<double> w;
    vector<state_fish> res;
    int kk;
    int kontext=0;
    state_fish etat;
    int t=1;

    
    for(int i=0;i<obs.size()-1;++i){
        t=obs[i+1].time-obs[i].time;
        etat.context=kontext;
        etat.pos =obs[i].pos;
        etat.speed=norm(obs[i+1].pos-obs[i].pos)/t; 
        etat.dir=(t/etat.speed)*(obs[i+1].pos-obs[i].pos);
        res.push_back(etat);
        Sample=update_cityv3(etat,t,n,station);
        w=importance_sampling_city(Sample,obs[i+1].pos);
        Sample=resample_fish(Sample,w,n);
        vector<double> context(4,0);
        for(int k=0;k<Sample.size();++k){
            context[Sample[k].context]+=1;
        }
        kontext=arg_max(context);     
    }
    return(res);
    
}

vector<int> SavitzkyGolay(vector<int> context, int l){
    vector<int> res(context.size(),0);
    vector<double> temp(4,0);
    for(int i=0;i<l;++i){
        res[i]=context[i];
    }
    double m=0;
    for(int i=0;i<context.size()-2*l;++i){
        for(int z=0;z<3;++z){
            temp[z]=0;
        }
        for(int j=l+i;j<2*l+i;++j){
            temp[context[j]]+=1;
        }
        res[i]=arg_max(temp);
    }
    return(res);
}


vector<state_fish> process_tram(vector<state_fish> states,vector<road> rails){
    point x;
    point temp;
    point z;
    vector<state_fish> res=states;
    for(int i=1;i<states.size();++i){
        if(states[i].context==2){
            z=states[i].pos;
            x=findx(rails[0].lineString,z);
            for(int j=1;j<rails.size();++j){
                temp=findx(rails[j].lineString ,z);
                //if(norm(z-temp)<norm(z-x)){
                if(norm(z-temp)+norm(states[i-1].pos-temp)<norm(z-x)+norm(states[i-1].pos-x)){
                    x=temp;
                }
            }
        res[i].pos=x;
        }
    }
    return(res);
}

#include "context.hpp"
#include "io.hpp"
#include "class.hpp"
#include <ctime>
#include <random>

int main(int argc, char *argv[]){
    srand(time(NULL));
    default_random_engine generator;
    vector<observation> pointsfish;
    vector<int> label;


    if(argc>=2){
        //cout<<"input"<<endl;
        pointsfish=read_csv(argv[1],4,3,2);
        label=read_label(argv[1],5);

    }else{
        pointsfish=read_csv("../1traj.csv",4,3,2);
        label=read_label("../1traj.csv",5);
    }
    //vector<observation> pointsfish=read_csv("../1traj.csv",4,3,2);
    
    vector<point> pointss;
    for(int i=0;i<pointsfish.size();++i){
        pointss.push_back(pointsfish[i].pos);
    }
    
    for(int i=0;i<pointsfish.size();++i){
        pointsfish[i].pos=pointss[i];
    }
    
   point init = pointss[0] +(1,1);
    line coast(vector<point>({init}));

    vector<state_c> states=particle_filterv2(pointsfish,coast,100); //100
    /*
    vector<int> processed=lissage(states,10);
    for(int k=0;k<states.size();++k){
        states[k].context=processed[k];
    }
    */
    

    vector<point> match_sail;
    vector<point> match_fish;
    vector<point> fish;
    vector<point> sail;
    int right=0;
    int wrong=0;
    string version = argv[3];
    if(argc==4 && version.compare("1")==0){
    for(int i=0;i<states.size();++i){
        if(states[i].context==2){
            if(label[i]==2){
                right+=1;
            }else{
                wrong+=1;
            }
            match_fish.push_back(states[i].pos);
        }else{
            if(label[i]!=2){
                right+=1;
            }else{
                wrong+=1;
            }
        match_sail.push_back(states[i].pos);
        }
    }
    
    for(int i=0;i<pointss.size();++i){
        if(label[i]==2){
            fish.push_back(pointss[i]);
        }else{
            sail.push_back(pointss[i]);
        }
  
    }
    if(argc>=3){
        write_boat(argv[2],fish,sail,match_sail,match_fish,right,wrong);

    }else{
        write_boat("bateau.txt",fish,sail,match_sail,match_fish,right,wrong);
    }
    
    cout<<""<<right<<","<<wrong<<""<<endl;
    }

    if (argc == 4 && version.compare("2") == 0){
        outputv2(argv[2],states,pointss,label);
    }
    

    return(0);
}


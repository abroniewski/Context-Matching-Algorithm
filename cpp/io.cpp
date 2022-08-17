#include <iostream>
#include <fstream>
#include <cstring>
#include <string>


#include <sstream>
#include <iterator>
#include <vector>
#include <algorithm>
#include "class.hpp"
#include "context.hpp"
using namespace std;


/*
void write_road(const char *filename, line road,point z,point x){
    ofstream fichier;
    fichier.open(filename);
    fichier<<"road"<<endl;
    for(int i=0;i<road.taille;++i){
        fichier<<road.points[i]<<endl;
    }
    fichier<<endl;
    fichier.close();
}

void write_gps(fstream fichier, point z){
    fstream fichier;
    //fichier.open(filename);
    fichier<<"point"<<endl;
    fichier<<z<<endl;
    fichier<<endl;
    //fichier.close();

}

*/

void split(const string &chaine, char delimiteur, vector<string> &elements)
{
 stringstream ss(chaine);
 string sousChaine;
 while (getline(ss, sousChaine, delimiteur))
 {
 elements.push_back(sousChaine);
 }
}
vector<string> split(const string &chaine, char delimiteur) 
{
 vector<string> elements;
 split(chaine, delimiteur, elements);
 return elements;
}


roadNetwork read(const char *filename, int roads){
    cout<<"debut"<<endl;
    ifstream fichier;
    string word; 
    string line;
    string test2;
    

    fichier.open(filename);
    vector<string> refnodes;
    vector<point> nodes;
    
    vector<string> refways;
    vector<vector<string>> ways;

    if (fichier.is_open())
    {
        cout<<"fichier opened"<<endl;
        /*
        for(int j=0;j<20;++j){



        getline(fichier,line);
        vector<string> A=split(line,' ');
        for(int i=0;i<A.size();++i){
            cout<<" i : "<<i<<" A "<<A[i]<<endl;
        }
        cout<<endl;
        }
        */
       int z=0;
       
       
        
        while (fichier.good()){
            z+=1;
            getline(fichier, line);
            
            vector<string> T = split(line, ' ');
            vector<string> I;

            double x;
            double y;

            if (T[2].compare("<node") == 0)
            {
                //import nodes
              
                for (int i = 2; i < T.size(); ++i)
                {
                    
                    
                    I = split(T[i], '=');


                    if (I[0].compare("id") == 0)
                    {
                        I[1].erase(remove(I[1].begin(), I[1].end(), '"'), I[1].end());
                        refnodes.push_back(I[1]);
                        
                    }
                    
                    if (I[0].compare("lat") == 0)
                    {
                        I[1].erase(remove(I[1].begin(), I[1].end(), '"'), I[1].end());
                        y = stod(I[1]);
                        
                    }
                    if (I[0].compare("lon") == 0)
                    {
                        I[1].erase(remove(I[1].begin(), I[1].end(), '"'), I[1].end());
                        x = stod(I[1]);
                    }
                }
                
                //A changer si on veut etre en metres / GC
                //nodes.push_back(point(x, y));
                nodes.push_back(GC_to_meters(point(x,y),point(4.36,50.84)));
            }

            //end of import nodes
            if (T[2].compare("<way") == 0){
                vector<string> refnodeway;
                bool street=false;
                
                string roadid;    
                    I = split(T[3], '=');               
                if (I[0].compare("id") == 0){
                    
                        I[1].erase(remove(I[1].begin(), I[1].end(), '"'), I[1].end());
                        roadid = I[1];
                        //refways.push_back(roadid);
                }
                
                getline(fichier, line);
                vector<string> T = split(line, ' ');
                
                while(line.compare("  </way>")!=0){
                    
                    T=split(line, ' ');
                    if(T[4].compare("<nd")==0){
                        I=split(T[5],'=');
                        I=split(I[1],'"');
                        
                        refnodeway.push_back(I[1]);
                        //getline(fichier, line);
                    }
                    if(T[4].compare("<tag" )==0){
                        I=split(T[3],':');
                        if(T[5].compare("k=\"highway\"")==0 &&  roads==1){
                        
                            street=true;

                        }else if(T[5].compare("k=\"railway\"")==0 && roads==0){
                            street=true;
                        }

                    }
                    getline(fichier, line);

                }
                if(street==true){

                    
                    ways.push_back(refnodeway);
                    refways.push_back(roadid);

                }
                
                /*
                while(T[2].compare("<nd")==0){
                    //cout<<"nd road :"<<endl;
                    //cout<<T[2]<<endl;
                    I=split(T[3],'=');
                    I=split(I[1],'"');
                    //cout<<I[1]<<endl;
                    refnodeway.push_back(I[1]);
                    getline(fichier, line);
                    //cout<<"get"<<endl;
                    //cout<<line<<endl;
                    if(line.compare(" </way>")==0){
                        //cout<<"fin way"<<endl;
                        break;
                    }
                    T = split(line, ' ');
                }
                ways.push_back(refnodeway);
               */ 
            }
            if(T[2].compare("</relation>")==0){
                
                break;
            }
            







             
        }

        
        fichier.close();
        
    }
    roadNetwork R = constructfrominput(refnodes, nodes, refways, ways);
    return(R);
}


vector<point> read_gpx(const char *filename){
    ifstream fichier;
    fichier.open(filename);
    vector<point> gpxpoint(0);
    if(fichier.is_open()){
        cout<<"gpx opened"<<endl;
        string line;
        
        point z;
        double x;
        double y;
        //set space number
        int espaces;
        vector<string> I;

        
        while(fichier.good()){
            getline(fichier,line);

            vector<string> T = split(line, ' ');
            espaces=0;
            while(T[espaces].compare("")==0 && espaces<T.size()-1){
                espaces+=1;
            }
            if(T[espaces].compare("<trkpt")==0){
                for (int i = espaces; i < T.size(); ++i)
                {                                        
                    I = split(T[i], '=');

                    if (I[0].compare("lat") == 0){
                        I[1].erase(remove(I[1].begin(), I[1].end(), '"'), I[1].end());
                        
                        y=stod(I[1]);
                    }
                    if (I[0].compare("lon")==0){
                        I[1].erase(remove(I[1].begin(), I[1].end(), '"'), I[1].end());
                        
                        x=stod(I[1]);

                    }

                }
                //A changer si on veut etre en metres / GC
                //gpxpoint.push_back(point(x, y));
                gpxpoint.push_back(GC_to_meters(point(x,y),point(4.36,50.84)));
            }
        }
    }
    fichier.close();
    return(gpxpoint);

}




void write(const char *filename, roadNetwork roads,vector<point> points,vector<point> match, vector<point> bus){
    ofstream fichier;
    fichier.open(filename);
    for(int i=0;i<roads.taille;++i){
        fichier<<"road"<<endl;
        road road=roads.roads[i];

        for(int j=0;j<road.lineString.taille;++j){
            fichier<<road.lineString.points[j]<<endl;
        }
    }
    fichier<<"obs"<<endl;
    for (int i=0;i<points.size();++i){
        fichier<<points[i]<<endl;
    }
    fichier<<"match"<<endl;
    for (int i=0;i<match.size();++i){
        fichier<<match[i]<<endl;
    }
    fichier<<"bus"<<endl;
    for (int i=0;i<bus.size();++i){
        fichier<<bus[i]<<endl;
    }
    fichier.close();
}

void write_to_json(const char *filename, vector<point> points, vector<int> type,vector<string> colorChoice){
    ofstream fichier;
    fichier.open(filename);
    fichier<<"marker-color,Longitude,Latitude"<<endl;
    for(int i=0;i<type.size();++i){
        fichier<<colorChoice[type[i]]<<","<<points[i].x<<","<<points[i].y<<endl;
        }

    fichier.close();
}

vector<observation> read_csv(const char *filename,int lat, int lon,int time, int GC, int bateau){
    ifstream fichier;
    fichier.open(filename);
    vector<observation> gpxpoint;
    string line;
    double x;
    double y;
    double t;
    if(fichier.is_open()){
        //cout<<"csv opened"<<endl;
        getline(fichier,line);
        vector<string> T = split(line, ',');
        //cout<<T.size()<<endl;
        getline(fichier,line);
        while(fichier.good()){
            
            vector<string> T = split(line, ',');
            x=stod(T[lon]);
            y=stod(T[lat]);
            if(true){
            //if(x>1 && y>1){
                point z(x,y);
                //cout<<"point pushed"<<endl;
                vector<string> I = split(T[time],' ');
                vector<string> K=I;
                if(bateau==1){
                    K=split(I[0],'-'); //a changer pour les gpx

                }else{
                    K=split(I[0],'.'); //a changer pour les gpx

                }                
                vector<string> J=split(I[1],':');
                int t=3600*24*stoi(K[2])+3600*stoi(J[0]) + 60*stoi(J[1]) + stoi(J[2]);
                //A changer si on veut etre en metres / GC
                if(GC==0){
                    gpxpoint.push_back(observation(z,t));
                }else{
                    gpxpoint.push_back(observation(GC_to_meters(point(x,y),point(4.36,50.84)),t));

                }
                
                
            }else{
                cout<<"x : "<<x<<" y :"<<y<<endl;
            }
            getline(fichier,line);
        }
        fichier.close();


    }
    return(gpxpoint);
}

vector<int> read_label(const char *filename,int labelind){
    ifstream fichier;
    fichier.open(filename);
    vector<int> label;
    string line;
    string c;
    if(fichier.is_open()){
        //cout<<"csv opened"<<endl;
        getline(fichier,line);
        vector<string> T = split(line, ',');
        getline(fichier,line);
        while(fichier.good()){          
            vector<string> T = split(line, ',');
            c=(T[labelind]);
            if(c.compare("\"01-sailing\"")==0){
                label.push_back(1);
            }else if(c.compare("\"02-fishing\"")==0){
                label.push_back(2);
            }else if(c.compare("\"03-sailing\"")==0){
                label.push_back(3);
            }else{
                //cout<<c<<endl;
            }

            getline(fichier,line);
        }
        fichier.close();


    }
    return(label);
}


void write_boat(const char *filename,vector<point> sailing ,vector<point> fishing,vector<point> match_sail, vector<point> match_fish,int right, int wrong){
    ofstream fichier;
    fichier.open(filename);
    fichier<<"sail"<<endl;
    for (int i=0;i<sailing.size();++i){
        fichier<<sailing[i]<<endl;
    }
    fichier<<"fish"<<endl;
    for (int i=0;i<fishing.size();++i){
        fichier<<fishing[i]<<endl;
    }
    fichier<<"match_sail"<<endl;
    for (int i=0;i<match_sail.size();++i){
        fichier<<match_sail[i]<<endl;
    }
    fichier<<"match_fish"<<endl;
    for (int i=0;i<match_fish.size();++i){
        fichier<<match_fish[i]<<endl;
    }
    fichier<<"rw"<<endl<<"("<<right<<","<<wrong<<")"<<endl;

    fichier.close();
}

void write_boatv2(const char *filename,vector<point> sail_1 ,vector<point> fish,vector<point> sail_2,vector<point> match_sail_1, vector<point> match_fish,vector<point> match_sail_2, vector<int> rw){
    ofstream fichier;
    fichier.open(filename);
    fichier<<"sail_1"<<endl;
    for (int i=0;i<sail_1.size();++i){
        fichier<<sail_1[i]<<endl;
    }
    fichier<<"fish"<<endl;
    for (int i=0;i<fish.size();++i){
        fichier<<fish[i]<<endl;
    }
    fichier<<"sail_2"<<endl;
    for (int i=0;i<sail_2.size();++i){
        fichier<<sail_2[i]<<endl;
    }
    fichier<<"match_sail_1"<<endl;
    for (int i=0;i<match_sail_1.size();++i){
        fichier<<match_sail_1[i]<<endl;
    }
    fichier<<"match_fish"<<endl;
    for (int i=0;i<match_fish.size();++i){
        fichier<<match_fish[i]<<endl;
    }
    fichier<<"match_sail_2"<<endl;
    for (int i=0;i<match_sail_2.size();++i){
        fichier<<match_sail_2[i]<<endl;
    }
    fichier<<"rwsail_1"<<endl<<"("<<rw[0]<<","<<(rw[1]+rw[2])<<")"<<endl;
    fichier<<"rwfish"<<endl<<"("<<rw[4]<<","<<(rw[3]+rw[5])<<")"<<endl;
    fichier<<"rwsail_2"<<endl<<"("<<rw[8]<<","<<(rw[6]+rw[7])<<")"<<endl;

    fichier.close();

}


vector<point> read_stops(const char *filename){
    cout<<"debut"<<endl;
    ifstream fichier;
    string line;

    

    fichier.open(filename);
    vector<point> nodes;
    


    if (fichier.is_open())
    {
        cout<<"fichier opened"<<endl;
        
       int z=0;
         
        while (fichier.good()){
            z+=1;
            
             
            getline(fichier, line);
            if(line.substr(0,6).compare("</osm>")==0){
                break;
            }
        
            
            vector<string> T = split(line, ' ');
            vector<string> I;

            double x;
            double y;

            if (T[2].compare("<node") == 0)
            {
            
                for (int i = 2; i < T.size(); ++i)
                {
                    
                    
                    I = split(T[i], '=');
                   
                    if (I[0].compare("lat") == 0)
                    {
                        I[1].erase(remove(I[1].begin(), I[1].end(), '"'), I[1].end());
                        y = stod(I[1]);
                        
                    }
                    if (I[0].compare("lon") == 0)
                    {
                        I[1].erase(remove(I[1].begin(), I[1].end(), '"'), I[1].end());
                        x = stod(I[1]);
                    }
                }
                
                nodes.push_back(GC_to_meters(point(x,y),point(4.36,50.84)));
            }
        
        }

        
        fichier.close();
        
    }

    return(nodes);
}
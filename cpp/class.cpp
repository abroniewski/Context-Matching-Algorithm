#include "class.hpp"
#include <iostream>
#include <vector>
#include <cmath>
#include <limits>
#include <numeric>
#include <algorithm>
using namespace std;

/*#############################################################################################*/

double sigma=1.;
double betta= 1.;
double pi = 2*acos(0.0);



int arg_max(vector<double> const& vec) {
  return static_cast<int>(distance(vec.begin(), max_element(vec.begin(), vec.end())));
}





int findIndex(const vector<string> &T, string elem) {
    auto ret = std::find(T.begin(), T.end(), elem);

    if (ret != T.end())
        return ret - T.begin();
    return -1;
}

/*#############################################################################################*/

point::point(double a, double b){
    //constructeur du point 
    x=a;
    y=b;
}



double point::arg(){
    // renvoie l'argument de la représentation complexe du point 
    //cout << x << " " << y << endl;
    if (fabs(x)<1E-14&&fabs(y)<1E-14) return(0);
    if (x<0&&fabs(y)<1E-14) return(3.1415);
    return(2*atan(y/(sqrt(x*x+y*y)+x)));
}


point operator + (point S1, point S2){
    point R(S1.x+S2.x,S1.y+S2.y);
    return(R);
}


point operator - (point S1, point S2){
    point R(S1.x-S2.x,S1.y-S2.y);
    return(R);
}

double operator | (point S1, point S2){
    //produit scalaire
    return(S1.x*S2.x+S1.y*S2.y);
}

bool operator ==(point S1, point S2){
    if(S1.x==S2.x){
        if(S1.y==S2.y){
            return(true);
        }
    }
    return(false);
}

bool operator != (point S1, point S2){

    return(!(S1==S2));
}
point operator *(double lambda, point u){
    return(point(lambda*u.x,lambda*u.y));
}


ostream & operator << (ostream & out, point S){
    //affichage du point
    out << "(" << S.x << "," << S.y << ")";
    return(out);
}

ostream & operator << (ostream & out, match M){
    out << "(" << M.edge.roadID << "," << M.distance << ")";
    return(out);
}


double norm(point u){
    return(sqrt(pow(u.x,2) + pow(u.y,2)));
}

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

point GC_to_meters(point a, point b){
    double latMid, m_per_deg_lat, m_per_deg_lon, deltaLat, deltaLon,dist_m;

    //latMid = (a.y+b.y )/2.0;  // or just use Lat1 for slightly less accurate estimate
    latMid=b.y;
    m_per_deg_lat = 111132.954 - 559.822 * cos( 2.0 * latMid ) + 1.175 * cos( 4.0 * latMid);
    m_per_deg_lon = (3.14159265359/180 ) * 6367449 * cos ( latMid );

    deltaLat = (a.y - b.y);
    deltaLon = (a.x - b.x);

    return(point(deltaLon * m_per_deg_lon,deltaLat * m_per_deg_lat));    
}

point meters_to_GC(point un, point ref){
    double latMid, m_per_deg_lat, m_per_deg_lon, deltaLat, deltaLon,dist_m;
    //point a is in meters, point b is lat long
    latMid=ref.y;
    m_per_deg_lat = 111132.954 - 559.822 * cos( 2.0 * latMid ) + 1.175 * cos( 4.0 * latMid);
    m_per_deg_lon = (3.14159265359/180 ) * 6367449 * cos ( latMid );
    deltaLon=un.x/m_per_deg_lon;
    deltaLat=un.y/m_per_deg_lat;

    return(point(deltaLon+ref.x,deltaLat+ref.y));

}

point intersect(point A, point B, point z){
    point v = B-A;
    point u = z-A;
    point x = ((u|v)/(pow(norm(v),2)))*v;
    if((v|x)<=0){
        return(A);
    }else if(norm(x)>norm(v)){
        return(B);
    }else{
        return(A+x);
    }
};

/*#############################################################################################*/

line::line(){
    vector<point> Null({});
    points =Null;
    taille=0; 
}

line::line(vector<point> Points_init){
    //initialisation d'une ligne de route avec ses points
    points = Points_init;
    taille = (int)points.size();
}


point findx(line roadline, point z){
    // trouve le point x sur la ligne de route roadline le plus proche de z
    point x=intersect(roadline.points[0],roadline.points[1],z);
    int k =0;
    for(int i=1;i<roadline.taille-1;++i){
        point xprim = intersect(roadline.points[i],roadline.points[i+1],z);
        if(norm(xprim-z)<norm(x-z)){
            x=xprim;
            k=i;
        }
    }
    return(x);
}


int findIndx(line roadline, point z){
    // trouve l'indice du point avant x 
    point x=intersect(roadline.points[0],roadline.points[1],z);
    int k =0;
    for(int i=1;i<roadline.taille-1;++i){
        point xprim = intersect(roadline.points[i],roadline.points[i+1],z);
        if(norm(xprim-z)<norm(x-z)){
            x=xprim;
            k=i;
        }
    }
    return(k);
}

double distZ_X(line roadline, point z){
    // retourne la distance entre z et x
    point x =findx(roadline, z);
    return(norm_GC(x,z));
}

/*#############################################################################################*/
match::match(){
    edge=road();
    distance=0;
}

match::match(road e, double d){
    edge=e;
    distance=d;
}

match::match(road e, point z){
    edge=e;
    distance=normRoadelem(e.lineString,z,false);

}


/*#############################################################################################*/

road::road(){
    roadID="0";
    roadID ="0";
    fromNodeID = "0";
    toNodeID = "0";
    twoWays = false;
    speed = 0;
    vertexCount = 0;
    lineString = line();
}

road::road(string roadid,string fromnodeid, string tonodeid, bool twoways, int spid, int vertexcount, line linestring)
{
    //road constructor
    roadID =roadid;
    fromNodeID = fromnodeid;
    toNodeID = tonodeid;
    twoWays = twoways;
    speed = spid;
    vertexCount = vertexcount;
    lineString = linestring;
    double L=0;
    for(int i=0;i<linestring.taille-1;++i){
        L+=norm(linestring.points[i+1]-linestring.points[i]);
    }
    Length=L;
}

/*#############################################################################################*/
roadNetwork::roadNetwork(){
    vector<road> Null({});
    roads =Null;
    taille=0; 
}

roadNetwork::roadNetwork(vector<road> roadz){
    roads=roadz;
    taille=roads.size();
    vector<string> nodez(0);
    vector<point> nodee(0);
    bool isf=false;
    bool ist=false;
    for(int i=0;i<taille;++i){
        isf=false;
        ist=false;
        for (const auto &item : nodez) {
            
            
            if (item == roads[i].fromNodeID) {
                isf=true;
            }
            if (item == roads[i].toNodeID) {
                ist=true;
            }
            
        }
        
        if(isf==false){
            
            nodez.push_back(roads[i].fromNodeID);
            nodee.push_back(roads[i].lineString.points[0]);

        }
        if(ist==false){
        

            nodez.push_back(roads[i].toNodeID);
            nodee.push_back(roads[i].lineString.points[roads[i].lineString.taille-1]);
        }
    Refroads.push_back(roads[i].roadID);
    }
    Refnodes=nodez;
    nodes=nodee;
    /*
    bool is=false;
    vector<string> pointId(0);
    vector<point> pointz(0);
    for(int i=0;i<taille;++i){
        for(int j=0;j<roads[i].lineString.taille;++j){
           is=findIndex(pointId,roads[i].lineString.points[])

    }
    */


    vector<double> ligne;
    for (int i=0; i<Refnodes.size(); ++i){
        ligne = vector<double>(Refnodes.size(),numeric_limits<double>::infinity());
        Adjacence.push_back(ligne);
    }
    for(int i=0;i<taille;++i){
        int fni=findIndex(nodez,roads[i].fromNodeID);
        int tni=findIndex(nodez,roads[i].toNodeID);
        Adjacence[fni][tni]=roads[i].Length;
        if(roads[i].twoWays==true){
            Adjacence[tni][fni]=roads[i].Length;

        }
    }
    
    for(int i=0;i<roads.size();++i){
        vector<int> voisins(0);
        string nt=roads[i].toNodeID;
        string nf=roads[i].fromNodeID;
        for(int j=0;j<roads.size();++j){
            if(roads[j].fromNodeID.compare(nt)==0 || roads[j].toNodeID.compare(nt)==0 || roads[j].fromNodeID.compare(nf)==0 || roads[j].toNodeID.compare(nf)==0){
            //if(roads[j].fromNodeID.compare(nt)==0){
                voisins.push_back(j);
            }
        }
        VoisinRoad.push_back(voisins);
    }



}

double roadNetwork::dist(int i, int j){
    if (i<0||j<0||i>=(int)Refnodes.size()||j>=(int)Refnodes.size()) {cout << "distance : Index out of bound;" << endl; exit(1);}
    return(Adjacence[i][j]);
}


/*#############################################################################################*/


double probamatch(point z, line road){
    double denom=(1/sqrt(2*pi*sigma));
    double num=-0.5*pow((distZ_X(road,z)/sigma),2);
    return(denom*exp(num));
}

double probatransi(roadNetwork r,point z0, point z1, state xi, state xj){
    double distance=distancetransi(r,z0,z1,xi,xj);
    return((1/betta)*exp(-distance/betta));
}


double distancetransi(roadNetwork R, point z0, point z1, state xi, state xj){
    double d=abs(norm(z1-z0)-normRoadstate(R,xi,xj));
    return(d);
}

double proba_emission_base(point obs, point state){
    double denom=(1/sqrt(2*pi*sigma));
    double num=-0.5*pow((norm(obs-state)/sigma),2);
    return(denom*exp(num));
}

/*#############################################################################################*/

state::state(road e, double d,double v,double o)
{

    velocity=v;
    error=o;

    context=0;
    parking_lot=0;
    bus_stop=0 ;
    dir=point(1,0);

}

state::state(point xx, road e, double v, double o){
    x=xx;
    edge = e;
    distance=norm(findx(e.lineString,x)-e.lineString.points[0]);
    velocity=v;
    error=o;
    context=0;
    parking_lot=0;
    bus_stop=0 ;
    dir=point(1,0);
}

state::state(){
    
}


void state::set_bus(line bus){
    point k=bus.points[0];
    for(int i=0;i<bus.taille;++i){
        if(norm(bus.points[i]-x)<norm(k-x)){
            k=bus.points[i];
        }
    }
    bus_stop=norm(k-x);
}

void state::set_park(line park){
        point k=park.points[0];
    for(int i=0;i<park.taille;++i){
        if(norm(park.points[i]-x)<norm(k-x)){
            k=park.points[i];
        }
    }
    parking_lot=norm(k-x);
}




/*###########################################################################################*/




double normRoadelem(line road){
    //calcule la longueur de la route
    double d =0;
    for(int i=0;i<road.taille-1;++i){
        d+=norm(road.points[i+1]-road.points[i]);
    }
    return(d);
}
double normRoadelem(line road, point z, bool dir){
    // si dir est vraie, on va de z au dernier indice
    // sinon on va du premier indice à z
    double d=0;
    point x=findx(road,z);
    int ind = findIndx(road, z);
    for(int i=0; i<=ind-1;++i){
        d+=norm(road.points[i+1]-road.points[i]);
    }
    d+=norm(x-road.points[ind]);
    if(dir==true){
        return(normRoadelem(road)-d);
    }else{
        return(d);
    }

}



double normRoad(point z0, point z1, line roadi, line roadj){
    // calcule la norme route entre 2 x en supposant que les routes soient adjacentes !!

    // si dir est vraie, on va de z au dernier indice
    // sinon on va du premier indice à z
    bool i;
    bool j;
    
    if(roadi.points[0]==roadj.points[0]){
        i=false, j=false;
        
    }else if(roadi.points[0]==roadj.points[roadj.taille-1]){
        i=false, j=true;
       
    }else if(roadi.points[roadi.taille-1]==roadj.points[roadj.taille-1]){
        i=true, j=true;
        
    }else if(roadi.points[roadi.taille-1]==roadj.points[0]){
        i=true, j=false;
        
    }else {
        cout<<"erreur normRoad"<<endl;
        return(-1);
    }
    return(normRoadelem(roadi,z0,i)+ normRoadelem(roadj,z1,j));


}



double normRoadstate(roadNetwork Roads, state i, state j){
    int ifn=findIndex(Roads.Refnodes,i.edge.fromNodeID);
    int jfn=findIndex(Roads.Refnodes,j.edge.fromNodeID);  //cetait un i
    int itn=findIndex(Roads.Refnodes,i.edge.toNodeID);
    int jtn=findIndex(Roads.Refnodes,j.edge.toNodeID);
    double dist=0;
    if(i.edge.roadID.compare(j.edge.roadID)==0){
        dist=0;
    }else if(Roads.Adjacence[itn][jfn]<10000){
        dist=Roads.Adjacence[itn][jfn];
    }else{
        dist=dijkstra(Roads,itn)[jtn];
    }
    dist=dist+j.distance+(i.edge.Length-i.distance);

    if(i.edge.twoWays==true && j.edge.twoWays==false){
        double dist2=dijkstra(Roads,ifn)[jfn];
        dist2=dist2+j.distance+i.edge.Length;
        if(dist2<dist){
            dist=dist2;
        }

    }
    if(i.edge.twoWays==false && j.edge.twoWays==true){
        double dist2=dijkstra(Roads,itn)[jtn];
        dist2=dist2+j.edge.Length -j.distance+(i.edge.Length-i.distance);
        if(dist2<dist){
            dist=dist2;
        }

    }
    if(i.edge.twoWays==true && j.edge.twoWays==true){
        double dist2=dijkstra(Roads,itn)[jtn];
        double dist3=dijkstra(Roads,ifn)[jfn];
        double dist4=dijkstra(Roads,ifn)[jtn];
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


vector<road> box(roadNetwork R, point z,double seuil){
    vector<road> rnew;
    for(int i=0;i<R.roads.size();++i){
        if(norm(R.roads[i].lineString.points[0]-z)<=seuil){
            rnew.push_back(R.roads[i]);
            //cout<<"route ajoutee"<<endl;

        }
    }
    return(rnew);
};

vector<state> KNN(vector<road> roads, point z, int k){
    vector<state> C(0);
    point x;
    for(int i=0;i<roads.size();++i){
        x=findx(roads[i].lineString,z);
        state s(x,roads[i],0.,0.);
        C.push_back(s);

    }
    state p(z,roads[0],0,0);
    int j;
    if(C.size()>2){
    for(int i=1;i<roads.size();++i){
        p=C[i];
        j=i;
        while(j>0 && norm(C[j-1].x-z)>norm(p.x-z)){
            C[j]=C[j-1];
            j=j-1;
        }
        C[j]=p;

    }
    }
    int taille=C.size();
    while(taille>k){
        C.pop_back();
        taille-=1;
    }
    return(C);

}

/*###########################################################################################*/




roadNetwork constructfrominput(vector<string> refnodes, vector<point> nodes, vector<string> refways, vector<vector<string>> ways){

    //road(int roadid,int fromnodeid, int tonodeid, bool twoways, int spid, int vertexcount, line linestring)
    //roadNetwork(vector<road> roadz)
    cout<<"refnode taille :"<<refnodes.size()<<endl;
    cout<<"nb highway :"<<refways.size()<<endl;
    cout<<"nb highway :"<<ways.size()<<endl;
    cout<<" last high :"<<refways[0]<<endl;
    vector<road> roadz;
    int unfound=0;
    for(int i=0;i<refways.size();++i){       
        string roadid=refways[i];
        string fromnodeid=ways[i][0];
        string tonideid=ways[i][ways[i].size()-1];
        bool twoways=false;
        vector<point> points;
        for(int j=0;j<ways[i].size();++j){
            int ind = findIndex(refnodes,ways[i][j]);
            if(ind ==-1){
                unfound+=1;
                //cout<<"NODE NOT FOUND IN CONSTRUCTFROMINPUT"<<endl;
                //cout<<" unfound : "<<unfound<<endl;
            }else{
                point z=nodes[ind];
                points.push_back(z);
            }
            
            

        }
        line linestring(points);
        road r(roadid,fromnodeid,tonideid,twoways,10,0,linestring);
        roadz.push_back(r);
    }
    roadNetwork Roads(roadz);
    return(Roads);

}



observation::observation(point z,int t){
    pos=z;
    time=t;
}


/*###########################################################################################*/



int nearest_vertex(vector<int> vertex, vector<double> d){
    vector<int>::iterator argmin = vertex.begin();
    double min = d[*argmin]; //min est la distance minimale entre le sommet actuel et son plus proche voisin
    for (vector<int>::iterator it = vertex.begin(); it<vertex.end(); it=it+1) {
        if (d[*it]<min){ // si la distance du sommet actuel au sommet it est plus petite que min, min prend la valeur d[*it] et le sommet le plus proche devient it
            argmin = it;
            min = d[*it];
        }
    }
    return(argmin-vertex.begin());
}

vector<double> dijkstra(roadNetwork R,int i){
    vector<int> sommets_traites(0);
    vector<int> sommets_a_traiter(R.nodes.size());
    vector<int> precedents(R.nodes.size()); //vecteur tel que precedents[i] est le sommet precedant i dans le chemin 
    iota(sommets_a_traiter.begin(),sommets_a_traiter.end(),0);
  
    vector<double> d(R.nodes.size(),numeric_limits<double>::infinity()); // d[i] distance du sommet de depart 0 au sommet i
    d[i]=0;
    int it_tmp;
    int sommet_tmp;
    while ((int)sommets_a_traiter.size()!=0) { // tant que tous les sommets n'ont pas ete visites
        it_tmp = nearest_vertex(sommets_a_traiter,d); //recherche du plus proche sommet du depart 0 parmi les sommets non traites
        sommet_tmp = sommets_a_traiter[it_tmp];
        sommets_traites.push_back(sommet_tmp);
        sommets_a_traiter.erase(sommets_a_traiter.begin()+it_tmp);
        for (int k:sommets_a_traiter) { 
            if(d[k]>d[sommet_tmp]+R.dist(sommet_tmp,k)){//si on trouve un chemin plus court, il s'agit du nouveau chemin optimal séparant 0 et k
                d[k]=d[sommet_tmp]+R.dist(sommet_tmp,k);
                precedents[k]=sommet_tmp;
            }
        }
    }
    return(d);
}
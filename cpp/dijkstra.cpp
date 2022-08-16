#include "class.hpp"

#include <iostream>
#include <climits>
#include <numeric>
#include <algorithm>
#include <vector>
#include <limits>

using namespace std;

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
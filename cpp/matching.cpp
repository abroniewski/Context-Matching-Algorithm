#include "class.hpp"
#include "matching.hpp"


vector<state> viterbi(roadNetwork R, vector<vector<state>> candidates, vector<point> traj){
    vector<vector<double>> proba(0);
    vector<vector<int>> path;

    //initialisation
    cout<<"initialisation viterbi"<<endl;
    vector<double> probainit(candidates[0].size());
    for(int i=0;i<candidates[0].size();++i){
        probainit.push_back(probamatch(traj[0],candidates[0][i].edge.lineString));
    }
    proba.push_back(probainit);
    cout<<"candidat size "<<candidates[10].size()<<endl;
    vector<int> pathinit(candidates[0].size(),0);
    path.push_back(pathinit);
    cout<<"fin initialisation"<<endl;

    //pour chaque observation :
    for(int i=1;i<traj.size();++i){
        cout<<"traitement de l'observation "<<i<<endl;
        int n=candidates[i].size();
        vector<double> probai(n);
        vector<int> candii(n);

        //pour chaque candidat :
        for(int j=0;j<n;++j){
            cout<<"traitement du candidat "<<j<<endl;
            double probEmi=probamatch(traj[i],candidates[i][j].edge.lineString);
            cout<<"probamatch"<<endl;
            double bestproba=probatransi(R,traj[i-1],traj[i],candidates[i-1][0],candidates[i][j])*proba[i-1][0];
            cout<<"bestproba"<<endl;
            int bestcandidat=0;

            //max et argmax :
            for(int k=0;k<candidates[i-1].size();++k){
                cout<<k<<endl;
                double probtemp=probatransi(R,traj[i-1],traj[i],candidates[i-1][k],candidates[i][j])*proba[i-1][k];
                if(probtemp>bestproba){
                    bestproba=probtemp;
                    bestcandidat=k;
                    cout<<"nouveau candidat"<<endl;
                }
            }
            probai.push_back(bestproba);
            candii.push_back(bestcandidat);

        }
        proba.push_back(probai);
        path.push_back(candii);
    }

    //construction du path trouvé :
    vector<state> foundpath;

    //argmax
    int maxk=0;
    int m=proba.size();
    double maxprob=proba[m-1][0];
    for(int k=0;k<proba[m-1].size();++k){
        if(proba[m-1][k]>maxprob){
            maxprob=proba[m-1][k];
            maxk=k;
        }
    }
    int mm=path.size();
    //prob[i][j] contient la proba du chemin le plus probable avec ^xi=sj
    //path[i][j] contient ^xi-1 de la séquence la plus probable où ^xi=sj, sj etant l'etat cache j
    foundpath.push_back(candidates[mm-1][maxk]);
    for(int i=mm-2;i>1;--i){
        maxk=path[i][maxk];
        foundpath.push_back(candidates[i][maxk]);


    }
    return(foundpath);




 
}


vector<vector<state>>  matching1(roadNetwork R, vector<point> traj, int k){
    vector<vector<state>> states_graph;
    //vector<state> C;
    for(int i=0;i<traj.size();++i){
        states_graph.push_back(KNN(box(R,traj[i],75),traj[i],k));
        //box(R,traj[i],0.005);
        cout<<"step : "<<i<<" done"<<endl;
    }
    return(states_graph);
}
from matplotlib import pyplot as plt

import sys

from nbformat import read


def readplot(filename):
    fichier = open(filename, 'r')
    contenu=fichier.readlines()
    fichier.close()
    contenu.append("fin")
    road=[]
    obs=[]
    bus=[]
    match=[]

    x=[]
    y=[]


    for i in range(len(contenu)):
        ligne = contenu[i]
        ligne = ligne.strip('\n')
        ligne = ligne.strip(' ')    
        ligne = ligne.strip('(')
        ligne = ligne.strip(')')


        if ligne == '':
            pass

        elif ligne in ["road","obs","fin","bus","match"]:
            if len(x)!=0:
                if type_data == "road":
                    road.append(x)
                    road.append(y)
                if type_data == "obs":
                    obs.append(x)
                    obs.append(y)
                if type_data == "bus" :
                    bus.append(x)
                    bus.append(y)
                if type_data == "match" :
                    match.append(x)
                    match.append(y)
            x=[]
            y=[]
            type_data = ligne
        else:
            ligne = ligne.split(",")
            x.append(float(ligne[0]))
            y.append(float(ligne[1]))
    for i in range(0,len(road)//2):
        plt.plot(road[2*i],road[2*i+1],color="blue")
    for i in range(0,len(obs)//2):
        plt.plot(obs[2*i],obs[2*i+1],color="red",marker="o",linestyle='None', markersize=1,markerfacecolor="red")
    for i in range(0,len(match)//2):
        plt.plot(match[2*i],match[2*i+1],color="yellow",marker="x",linestyle='None', markersize=4,markerfacecolor="yellow")
    for i in range(0,len(bus)//2):
        plt.plot(bus[2*i],bus[2*i+1],color="green",marker="x",linestyle='None', markersize=6,markerfacecolor="green")

    #ax.fill(*ListePolygones)
    plt.axis("equal")
    plt.show()
    print("Reading : Ended ({0})".format(filename))

##############################################################################################################

if(len(sys.argv)==2) :
    readplot(sys.argv[1])
else :
    readplot("bateau.txt")

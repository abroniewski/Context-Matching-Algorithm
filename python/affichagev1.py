from matplotlib import pyplot as plt

import sys

from nbformat import read


def readplot(filename):
    fichier = open(filename, 'r')
    contenu=fichier.readlines()
    fichier.close()
    contenu.append("fin")
    sail=[]
    fish=[]
    match_sail=[]
    match=[]
    right = []
    wrong = []

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

        elif ligne in ["sail","fish","match_sail","match_fish","fin","rw"]:
            if len(x)!=0:
                if type_data == "sail":
                    sail.append(x)
                    sail.append(y)
                if type_data == "fish":
                    fish.append(x)
                    fish.append(y)
                if type_data == "match_sail" :
                    match_sail.append(x)
                    match_sail.append(y)
                if type_data == "match_fish" :
                    match.append(x)
                    match.append(y)
                if type_data == "rw" :
                    right.append(x)
                    wrong.append(y)

            x=[]
            y=[]
            type_data = ligne
        else:
            ligne = ligne.split(",")
            x.append(float(ligne[0]))
            y.append(float(ligne[1]))
    for i in range(0,len(sail)//2):
        plt.plot(sail[2*i],sail[2*i+1],color="blue",marker="o",linestyle='None', markersize=4,markerfacecolor="blue")
    for i in range(0,len(fish)//2):
        plt.plot(fish[2*i],fish[2*i+1],color="red",marker="o",linestyle='None', markersize=4,markerfacecolor="red")
    for i in range(0,len(match)//2):
        plt.plot(match[2*i],match[2*i+1],color="green",marker="x",linestyle='None', markersize=2,markerfacecolor="green")
    for i in range(0,len(match_sail)//2):
        plt.plot(match_sail[2*i],match_sail[2*i+1],color="grey",marker="x",linestyle='None', markersize=2,markerfacecolor="grey")
   
    #ax.fill(*ListePolygones)
    plt.axis("equal")
    plt.title("success : %f "%(right[0][0]/(right[0][0]+wrong[0][0])))
    #plt.show()
    plt.savefig("../resultGaussDoubleMixt_png/%s"%(filename[25:-4])+".png")
    print("Reading : Ended ({0})".format(filename))

##############################################################################################################

if(len(sys.argv)==2) :
    readplot(sys.argv[1])
else :
    readplot("bateau.txt")

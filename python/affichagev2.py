from matplotlib import pyplot as plt

import sys

from nbformat import read


def readplot(filename):
    fichier = open(filename, 'r')
    contenu=fichier.readlines()
    fichier.close()
    contenu.append("fin")
    sail_1=[]
    sail_2=[]
    fish=[]
    match_sail_1=[]
    match_sail_2=[]
    match_fish=[]
    rwsail_1=[]
    rwfish=[]
    rwsail_2=[]

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

        elif ligne in ["sail_1","fish","sail_2","match_sail_1","match_fish","match_sail_2","fin","rwsail_1","rwfish","rwsail_2"]:
            if len(x)!=0:
                if type_data == "sail_1":
                    sail_1.append(x)
                    sail_1.append(y)  
                if type_data == "fish":
                    fish.append(x)
                    fish.append(y)
                if type_data == "sail_2":
                    sail_2.append(x)
                    sail_2.append(y)  
                if type_data == "match_sail_1" :
                    match_sail_1.append(x)
                    match_sail_1.append(y)
                if type_data == "match_fish" :
                    match_fish.append(x)
                    match_fish.append(y)
                if type_data == "match_sail_2" :
                    match_sail_2.append(x)
                    match_sail_2.append(y)
                if type_data == "rwsail_1" :
                    rwsail_1.append(x)
                    rwsail_1.append(y)
                if type_data == "rwfish" :
                    rwfish.append(x)
                    rwfish.append(y)
                if type_data == "rwsail_2" :
                    rwsail_2.append(x)
                    rwsail_2.append(y)

            x=[]
            y=[]
            type_data = ligne
        else:
            ligne = ligne.split(",")
            x.append(float(ligne[0]))
            y.append(float(ligne[1]))
    for i in range(0,len(sail_1)//2):
        plt.plot(sail_1[2*i],sail_1[2*i+1],color="blue",marker="o",linestyle='None', markersize=4,markerfacecolor="blue")
    for i in range(0,len(sail_2)//2):
        plt.plot(sail_2[2*i],sail_2[2*i+1],color="violet",marker="o",linestyle='None', markersize=4,markerfacecolor="violet")
    for i in range(0,len(fish)//2):
        plt.plot(fish[2*i],fish[2*i+1],color="red",marker="o",linestyle='None', markersize=4,markerfacecolor="red")
    for i in range(0,len(match_fish)//2):
        plt.plot(match_fish[2*i],match_fish[2*i+1],color="green",marker="x",linestyle='None', markersize=2,markerfacecolor="green")
    for i in range(0,len(match_sail_1)//2):
        plt.plot(match_sail_1[2*i],match_sail_1[2*i+1],color="grey",marker="x",linestyle='None', markersize=2,markerfacecolor="grey")
    for i in range(0,len(match_sail_2)//2):
        plt.plot(match_sail_2[2*i],match_sail_2[2*i+1],color="yellow",marker="x",linestyle='None', markersize=2,markerfacecolor="yellow")
   
    #ax.fill(*ListePolygones)
    plt.axis("equal")
    plt.title("success : %f "%((rwsail_1[0][0]+rwsail_2[0][0]+rwfish[0][0])/(rwsail_1[0][0]+rwsail_2[0][0]+rwfish[0][0]+rwsail_1[1][0]+rwsail_2[1][0]+rwfish[1][0])))
    plt.legend(["sail_1","sail_2","fish","match_fish : %f"%(rwfish[0][0]/(rwfish[0][0]+rwfish[1][0])),"match_sail_1 : %f"%(rwsail_1[0][0]/(rwsail_1[0][0]+rwsail_1[1][0])) ,"match_sail_2 : %f"%(rwsail_2[0][0]/(rwsail_2[0][0]+rwsail_2[1][0]))])
    plt.show()
    #plt.savefig("../filterv3-update-png/%s"%(filename)[2:-4]+".png")
    #plt.savefig("result.png")
    print("Reading : Ended ({0})".format(filename))

##############################################################################################################

if(len(sys.argv)==2) :
    readplot(sys.argv[1])
else :
    readplot("bateau.txt")

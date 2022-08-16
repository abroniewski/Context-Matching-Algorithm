setwd("~/../Bureau/PRE/Map-matching/AIS_traj/")
library(dplyr)
table=read.table("1traj.csv",header = TRUE,sep=',')
table[,1]<-as.POSIXct(table[,1],format="%Y-%m-%d %H:%M:%S")

tableSail1=table %>% filter(label=="01-sailing")

tableSail2=table %>% filter(label=="02-fishing")

tableSail3=table %>% filter(label=="03-sailing")




result=c()

for(i in 1:dim(tableSail1)[1]){
  result=rbind(result,GC_to_meters(tableSail1[i,2],tableSail1[i,3],tableSail1[1,2],tableSail1[1,3]))
  
}
plot(result[,1]~result[,2])
results=GC_to_meters(tableSail1[,2],tableSail1[,3],tableSail1[1,2],tableSail1[1,3])
plot(table)

result=c()
for(i in 1:dim(table)[1]){
  result=rbind(result,GC_to_meters(table[i,2],table[i,3],table[1,2],table[1,3]))
  
}

table[,2]=result[,1]
table[,3]=result[,2]
plot(table[,3]~table[,2])





vitesse=speed(table[,2],table[,3],table[,1])


vitessesail1=vitesse[c(1:390)]
vitessefish=vitesse[c(391:5064)]
vitessesail2=vitesse[c(5065:5609)]
hist(vitesse)
hist(vitessefish)
hist(vitessesail1)
hist(vitessesail2)

#####################################################################################
#####################################################################################

GC_to_meters=function(ax,ay,xx,yy){
  
  
  latMid = (ay+yy )/2
  
  m_per_deg_lat = 111132.954 - 559.822 * cos( 2 * latMid ) + 1.175 * cos( 4* latMid);
  m_per_deg_lon = (3.14159265359/180 ) * 6367449 * cos ( latMid );
  
  deltaLat = (ay - yy);
  deltaLon = (ax - xx);
  
  return(cbind(deltaLon * m_per_deg_lon , deltaLat * m_per_deg_lat))    
}

speed=function(x,y,t){
  speedy=c()
  for(i in 1:length(x)-1){
    z=sqrt((x[i]-x[i+1])**2 +(y[i]-y[i+1])**2)/(as.numeric(difftime(t[i+1],t[i])))
    speedy=c(speedy,z)
  }
  return(speedy)
}


transformLatToM=function(x,y){
  result=c()
  for(i in 1:length(x)){
    result=rbind(result,GC_to_meters(x[i],y[i],x[1],y[1]))
    
  }
  return(result)
}

preprocess=function(tabble){
  jj=c() 
  jt=FALSE
  for(j in 2:dim(tabble[1])){
    if(GC_to_meters(tabble[j,3],tabble[j,4],tabble[j-1,3],tabble[j-1,4])>1000){
      jj=cbind(jj,j)
      jt=TRUE 
    }
  }
  
  if(jt==TRUE){
    tabble=tabble[-jj,]
  }
  return(tabble)
  
}


tablecomplete=read.csv("../../128_fishing_trajs.csv",header=TRUE,sep=',')
tablecomplete =tablecomplete[,c(1,2,3,4,13)]
tablecomplete[,2]<-as.POSIXct(tablecomplete[,2],format="%Y-%m-%d %H:%M:%S")

meansail1=c()
meanfishing=c()
meansail2=c()

tableid= tablecomplete %>% distinct(id)
for(i in 1:1){
#for(i in 1:(length(tableid[,1]))){
  tabble=tablecomplete %>% filter(id==tableid[i,])
  #tabble=preprocess(tabble)

  

  resultss=transformLatToM(tabble[,3],tabble[,4])
  tabble[,3]=resultss[,1]
  tabble[,4]=resultss[,2]
  vitesse=speed(tabble[,3],tabble[,4],tabble[,2])
  sail1 = tabble %>% filter(label=="01-sailing")
  vitessesail1=speed(sail1[,3],sail1[,4],sail1[,2])
  #hist(vitessesail1)
  meansail1=cbind(meansail1,mean(vitessesail1))

  fishing = tabble %>% filter(label=="02-fishing")
  vitessefishing=speed(fishing[,3],fishing[,4],fishing[,2])
  #hist(vitessefishing)
  meanfishing=cbind(meanfishing,mean(vitessefishing))

  sail2 = tabble %>% filter(label=="03-sailing")
  vitessesail2=speed(sail2[,3],sail2[,4],sail2[,2])
  #hist(vitessesail2)
  meansail2=cbind(meansail2,mean(vitessesail2))
}

hist(vitessesail1,breaks=20)
hist(vitessefishing,breaks=20)
hist(vitessesail2,breaks=20)
plot(vitessesail1)
hist(meansail1,breaks = 20)
hist(meanfishing,breaks = 20)
hist(meansail2, breaks=20)
plot(vitessesail1)
plot(vitessefishing)
plot(vitessesail2)

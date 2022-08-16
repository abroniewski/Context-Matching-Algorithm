setwd("~/Documents/Map-matching/gpx_files")
setwd(dir = "~/../Bureau/PRE/Projet code/Map-matching/gpx_files")
library(dplyr)
tablecomplete=read.csv("./GPX_points_22_07.csv",header=TRUE,sep=',')
tablecomplete =tablecomplete[,c(1,2,7)]
tablecomplete[,3]<-as.POSIXct(tablecomplete[,3],format="%Y-%m-%d %H:%M:%S")


##########################################################
GC_to_meters=function(ax,ay,xx,yy){
  
  
  latMid = (ay+yy )/2
  
  m_per_deg_lat = 111132.954 - 559.822 * cos( 2 * latMid ) + 1.175 * cos( 4* latMid);
  m_per_deg_lon = (3.14159265359/180 ) * 6367449 * cos ( latMid );
  
  deltaLat = (ay - yy);
  deltaLon = (ax - xx);
  
  return(cbind(deltaLon * m_per_deg_lon , deltaLat * m_per_deg_lat))    
}
transformLatToM=function(x,y){
  result=c()
  for(i in 1:length(x)){
    result=rbind(result,GC_to_meters(x[i],y[i],4.36,50.84))
    
  }
  return(result)
}
speed=function(x,y,t){
  speedy=c()
  for(i in 1:length(x)-1){
    z=sqrt((x[i]-x[i+1])**2 +(y[i]-y[i+1])**2)/(as.numeric(difftime(t[i+1],t[i])))
    speedy=c(speedy,z)
  }
  return(speedy)
}
##########################################################

resultss=transformLatToM(tablecomplete[,1],tablecomplete[,2])
tablecomplete[,1]=resultss[,1]
tablecomplete[,2]=resultss[,2]
vitesse=speed(tablecomplete[,1],tablecomplete[,2],tablecomplete[,3])

for(i in 1:450){
  if(vitesse[i]>100){
    vitesse[i]=0
  }
}
hist(vitesse,breaks = 200)
plot(tablecomplete[,1],tablecomplete[,2])




library(mixtools)
walk<-normalmixEM(vitesse[20:390], lambda = .5, mu = c(7,15), sigma = c(2.2,4.2))

summary(walk)
x=seq(0,40,0.05)
ymix=0
for(i in 1:2){
  ymix=ymix+walk$lambda[i]*dnorm(x,walk$mu[i],walk$sigma[i])
}
hist(vitesse[20:390],breaks = 200,freq = FALSE,main="")
title(main="tramway speed distribution")
points(x,ymix, col="lightslateblue",type="l", lwd=2)

points(x,dnorm(x,2,0.1),type="l")


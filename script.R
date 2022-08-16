#setwd("~/Documents/")
setwd(dir = "~/../Bureau/PRE/Projet code/Map-matching")
library(dplyr)
tablecomplete=read.csv("./128_fishing_trajs.csv",header=TRUE,sep=',')
tablecomplete =tablecomplete[,c(1,2,5,6,13)]
tablecomplete[,2]<-as.POSIXct(tablecomplete[,2],format="%Y-%m-%d %H:%M:%S")


speed=function(x,y,t){
  speedy=c()
  for(i in 1:length(x)-1){
    z=sqrt((x[i]-x[i+1])**2 +(y[i]-y[i+1])**2)/(as.numeric(difftime(t[i+1],t[i])))
    speedy=c(speedy,z)
  }
  return(speedy)
}

tableid= tablecomplete %>% distinct(id)


meansail1=c()
meanfishing=c()
meansail2=c()
sd_fishing=c()
#for(i in 4:4){
for(i in 1:dim(tableid)[1]){
  #for(i in 1:(length(tableid[,1]))){
  tabble=tablecomplete %>% filter(id==tableid[i,])
  #tabble=preprocess(tabble)
  
  
  vitesse=speed(tabble[,3],tabble[,4],tabble[,2])
  sail1 = tabble %>% filter(label=="01-sailing")
  vitessesail1=speed(sail1[,3],sail1[,4],sail1[,2])
  name<-paste(i,"vitessesail1",sep=" ")
  #hist(vitessesail1,main=name,breaks = 20)
  meansail1=cbind(meansail1,mean(vitessesail1))
  
  fishing = tabble %>% filter(label=="02-fishing")
  vitessefishing=speed(fishing[,3],fishing[,4],fishing[,2])
  name<-paste(i,"vitessefishing",sep=" ")
  hist(vitessefishing,freq=FALSE,main=name,breaks=seq(0,10000,0.5),xlim=c(0,5))
  hat_mu=mean(vitessefishing)
  print(hat_mu)
  hat_sigma=sd(vitessefishing)
  print(hat_sigma)
  #x=0:10
  #y=dnorm(x, hat_mu,hat_sigma)
  #points(x,y, col="lightslateblue",type="l", lwd=2)
  meanfishing=cbind(meanfishing,mean(vitessefishing))
  sd_fishing=cbind(sd_fishing,hat_sigma)
  
  sail2 = tabble %>% filter(label=="03-sailing")
  vitessesail2=speed(sail2[,3],sail2[,4],sail2[,2])
  name<-paste(i,"vitessesail2",sep=" ")
  #hist(vitessesail2,main=name,breaks=20)
  meansail2=cbind(meansail2,mean(vitessesail2))
}



vitessesail=c()
vitessefish=c()
for(i in 1:dim(tableid)[1]){
  tabble=tablecomplete %>% filter(id==tableid[i,])
  
  sail1 = tabble %>% filter(label=="01-sailing")
  vitessesail11=speed(sail1[,3],sail1[,4],sail1[,2])
  vitessesail=c(vitessesail,vitessesail11)
  
  fishing = tabble %>% filter(label=="02-fishing")
  vitessefishingg=speed(fishing[,3],fishing[,4],fishing[,2])
  vitessefish=c(vitessefish,vitessefishingg)
  
  sail2 = tabble %>% filter(label=="03-sailing")
  vitessesail22=speed(sail2[,3],sail2[,4],sail2[,2])
  vitessesail=c(vitessesail,vitessesail22)
  
}
vitessesailproc=data.frame(vitessesail)
vitessesailproc= vitessesailproc %>% filter(vitessesail<20)
ms=mean(vitessesailproc$vitessesail)
sds=sd(vitessesailproc$vitessesail)

vitessefishproc=data.frame(vitessefish)
vitessefishproc= vitessefishproc %>% filter(vitessefish<6)
mf=mean(vitessefishproc$vitessefish)
sdf=sd(vitessefishproc$vitessefish)

x=seq(0,8,0.05)

ys=dnorm(x, ms,sds)
yf=dnorm(x,mf,sdf)

sailinghist=hist(vitessesailproc$vitessesail,freq=FALSE,main="sailing speed distribution",breaks=seq(0,10000,0.05),xlim=c(0,8))
hist(vitessesailproc$vitessesail,freq=FALSE,main="sailing speed distribution",breaks=seq(0,10000,0.05),xlim=c(0,8))
points(x,ys, col="lightslateblue",type="l", lwd=2)
ms
sds


coucou=hist(vitessefishproc$vitessefish,freq=FALSE,main="fishing speed distribution",breaks=seq(0,10000,0.05),xlim=c(0,8))
hist(vitessefishproc$vitessefish,freq=FALSE,main="fishing speed distribution",breaks=seq(0,10000,0.05),xlim=c(0,8))
points(x,yf, col="lightslateblue",type="l", lwd=2)
mf
sdf



tableidd=as.data.frame(tableid)
tableiddd=tableidd[,]
for(i in 1:length(tableiddd)){
  tabble=tablecomplete %>% filter(id==tableid[i,])
  name<-paste(i,".csv",sep="")
  write.table(tabble,name,sep=',')
}



library(nor1mix)

dinguerie=(ytarace-yf)

for(i in 1:length(x)){
  if (dinguerie[i]<=0){
    dinguerie[i]=0
  }
}
plot(x,dinguerie)
mfd=1.2
sdfd=0.1
yfd=dnorm(x, mfd,sdfd)/4
points(x,yfd, col="lightslateblue",type="l", lwd=2)

hist(vitessefishproc$vitessefish,freq=FALSE,main="fishing speed distribution",breaks=seq(0,10000,0.05),xlim=c(0,8))
points(x,yf+yfd, col="lightslateblue",type="l", lwd=2)
points(x,yf, col="red",type="l", lwd=2)

library(nor1mix)
v=norMix(vitessefishproc$vitessefish)
dnorMix(x,v)
ydnormx=dnorMix(x,v)
plot(ydnormx)




library(mixtools)
wait1<-normalmixEM(vitessefishproc$vitessefish, lambda = .5, mu = c(mf,2), sigma = c(sdf,0.1))
plot(wait1, density=TRUE, cex.axis=1.4, cex.lab=1.4, cex.main=1.8,main2="Time between Old Faithful eruptions", xlab2="Minutes")

summary(wait1)

ymix=0
for(i in 1:2){
  ymix=ymix+wait1$lambda[i]*dnorm(x,wait1$mu[i],wait1$sigma[i])
}

hist(vitessefishproc$vitessefish,freq=FALSE,main="fishing speed distribution",breaks=seq(0,10000,0.05),xlim=c(0,8))
points(x,ymix, col="lightslateblue",type="l", lwd=2)


wait2<-normalmixEM(vitessesailproc$vitessesail, lambda = .5, mu = c(3.9,ms), sigma = c(0.4,sds))

summary(wait2)

ymixs=0
for(i in 1:2){
  ymixs=ymixs+wait2$lambda[i]*dnorm(x,wait2$mu[i],wait2$sigma[i])
}

hist(vitessesailproc$vitessesail,freq=FALSE,main="sailing speed distribution",breaks=seq(0,10000,0.05),xlim=c(0,8))
points(x,ymixs, col="lightslateblue",type="l", lwd=2)
points(x,ys, col="red",type="l", lwd=2)











vitessesail=c()
vitessefish=c()
for(i in 43:43){
  tabble=tablecomplete %>% filter(id==tableid[i,])
  
  sail1 = tabble %>% filter(label=="01-sailing")
  vitessesail11=speed(sail1[,3],sail1[,4],sail1[,2])
  vitessesail=c(vitessesail,vitessesail11)
  
  fishing = tabble %>% filter(label=="02-fishing")
  vitessefishingg=speed(fishing[,3],fishing[,4],fishing[,2])
  vitessefish=c(vitessefish,vitessefishingg)
  
  sail2 = tabble %>% filter(label=="03-sailing")
  vitessesail22=speed(sail2[,3],sail2[,4],sail2[,2])
  vitessesail=c(vitessesail,vitessesail22)
  
}

#include "class.hpp"
#include <vector>
#include <iostream>
#include <fstream>
#include <cmath>
#include "io.hpp"

double sigma=1.;
double beta= 1.;
double pi = 2*acos(0.0);


double probamatch(point z, line road){
    double denom=(1/sqrt(2*pi*sigma));
    double num=-0.5*pow((distRoad(road,z)/sigma),2);
    return(denom*exp(num));
}
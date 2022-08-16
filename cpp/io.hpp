#ifndef IO_HPP
#define IO_HPP
#include "class.hpp"

//void write_road(fstream fichier, line road);
roadNetwork read(const char *filename);

vector<point> read_gpx(const char *filename);
vector<point> read_stops(const char *filename);

void write(const char *filename, roadNetwork roads,vector<point> points,vector<point> match, vector<point> bus);
void write_to_json(const char *filename, vector<point> points, vector<int> type,vector<string> colorChoice);
void write_boat(const char *filename,vector<point> sailing ,vector<point> fishing,vector<point> match_sail, vector<point> match_fish, int right, int wrong);
void write_boatv2(const char *filename,vector<point> sailing_1 ,vector<point> fishing,vector<point> sailing_2,vector<point> match_sail, vector<point> match_fish,vector<point> match_sailing_2, vector<int> rightwrong);
//void write_gps(fstream fichier, point z);

vector<observation> read_csv(const char *filename,int lat, int lon,int time);
vector<int> read_label(const char *filename,int label);

#endif
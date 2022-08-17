# Required packages in C++
- iostream 
-  vector 
-  cmath 
-  limits 
-  numeric 
-  algorithm 
-  fstream 
-  string 
-  iostream 
-  ctime 
-  random 
# Required packages in python
- matplotlib
- sys
- nbformat
# Map Matching

## Set up 
The trajectory must be a GPX file, the road data are extracted from a OSM file
All the set up can be done in the main file :
- To change the OSM file : change path line 14
- To change the GPX file : change path line 16
- To change the number of points to match : change int line 22
- To change the output file name : chznge path line 33
The GPX and OSM files must not be modified from the standard format
## Compiling 
```bash
g++ ./cpp/class.cpp ./cpp/context.cpp ./cpp/io.cpp ./cpp/main_context_city.cpp 
```
## Execution
```bash
./a.out 
```
## Display
```bash
python3 python/affichage.py output.txt 
```
# Context Matching : fishing vessels
## Set up 
The input file must be a csv file with the following structure
```
"n","id","t","x","y","label"
"1","219001125-3",2021-11-15 02:54:18,594767.946816762,6398600.51909148,"01-sailing"
```
The date is in format yyyy-mm-dd HH:MM:SS
The coordinates "x" and "y" are in meters (from any reference point). 
The label can be "01-sailing", "02-fishing" or "03-sailing".
The id columns is ignored.


## Compiling
```bash
g++ ./cpp/class.cpp ./cpp/context.cpp ./cpp/io.cpp ./cpp/main_context_fishing.cpp 
```
## Execution : with 2 labels
```bash
./a.out  ./AIS_traj/traj/1.csv output.txt 1
```
In the console is displayed the tuple $(r,f)$ with $r$ the number of correctly labeled points, $f$ the number of false labeled points.
## Display
```bash
python3 python/affichagev1.py output.txt 
```
## Execution : with 3 labels
```bash
./a.out  ./AIS_traj/traj/1.csv output.txt 2
```
In the console are displayed the 9 numbers that corresponds to the boxes in the table of the report. The 1st, 4th, 9th numbers must be the bigger and the other the lower.
## Display
```bash
python3 python/affichagev2.py output.txt 
```
# Context Matching : city user
## Compiling 
```bash
g++ ./cpp/class.cpp ./cpp/context.cpp ./cpp/io.cpp ./cpp/main_context_city.cpp 
```
## Execution
```bash
./a.out 
```
## commandes à executer sous windows :

```bash
for($i=1; $i -lt 30; $i=$i+1){ .\a.exe ..\traj\${i}.csv ..\results\oui${i}.txt 2 }

for($i=1; $i -lt 30; $i=$i+1){ python3 affichagev2.py ..\results\oui${i}.txt }
```
## commandes à executer sous linux :
```bash
for ((i=1;i =30;i++)); do ./a.out ../traj/${i}.csv ../resultGaussMixt/${i}.txt 1 ; done
```

## Origine :
(4.36,50.84)

## Récupérer une zone avec un document pbf :
```bash
osmosis --read-pbf belgium-latest.osm.pbf --tf accept-ways highway=* --used-node --bounding-box top=50.8562 left=4.3525 bottom=50.8126 right=4.3869 --write-xml file="bruxelles_trams.osm"
osmosis --read-pbf belgium-latest.osm.pbf --node-key-value keyValueList="public_transport.station" --bounding-box top=50.8562 left=4.3525 bottom=50.8126 right=4.3869 --write-xml file="bruxelles_trams_stops.osm"
osmosis --read-pbf belgium-latest.osm.pbf --node-key-value keyValueList="public_transport.station,public_transport.stop_position" --bounding-box top=50.8718 left=4.2967 bottom=50.7973 right=4.4096 --write-xml file="bruxelles_stops.osm"
osmosis --read-pbf belgium-latest.osm.pbf --way-key-value keyValueList="railway.tram" --used-node --bounding-box top=50.8718 left=4.2967 bottom=50.7973 right=4.4096 --write-xml file="bruxelles_rail.osm"


```

## liens utiles
https://www.convertcsv.com/csv-to-geojson.htm
https://mygeodata.cloud/converter/gpx-to-csv
https://geojson.io/#map=2/20.0/0.0
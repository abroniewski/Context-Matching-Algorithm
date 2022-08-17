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


# OSM files
The osm files can be generated from a osm.pbf, foundable at :

https://download.geofabrik.de/

Then, the tool osmosis is needed, foundable at :

https://github.com/openstreetmap/osmosis/releases/tag/0.48.3

Now, to extract the road network needed for Map Matching, execute :
```bash
osmosis --read-pbf belgium-latest.osm.pbf --tf accept-ways highway=* --used-node --bounding-box top=50.8562 left=4.3525 bottom=50.8126 right=4.3869 --write-xml file="brussels.osm"
```
The bounding box can be changed.

To extract the rail network for the tramway, execute :
```bash
osmosis --read-pbf belgium-latest.osm.pbf --way-key-value keyValueList="railway.tram" --used-node --bounding-box top=50.8718 left=4.2967 bottom=50.7973 right=4.4096 --write-xml file="brussels_rail.osm"
```

To extract the public transport stations, execute :
```bash
osmosis --read-pbf belgium-latest.osm.pbf --node-key-value keyValueList="public_transport.station,public_transport.stop_position" --bounding-box top=50.8718 left=4.2967 bottom=50.7973 right=4.4096 --write-xml file="bruxelles_stops.osm"
```





# Map Matching

## Set up 
The trajectory must be a GPX file, the road data are extracted from a OSM file

All the set up can be done in the ```main.cpp``` file :
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
In the console is displayed the tuple $(r,f)$ with $r$ the number of correctly labeled points, $f$ the number of mislabeled points.
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
## Set up
All the set up can be done in the ```main_context_city.cpp`` file :

To compute or not the smoothing process, comment or not the line 38

To compute or not the light Map Matching post process, comment or not the line 46
### input files:
#### Trajectory
A csv file, with the following structure
```
lon,lat,c1,c2,c3,date
4.36856078,50.85514968,0,0,0,129.59,2022.07.22 07:00:37+00,
```
The date is in format yyyy.mm.dd HH:MM:SS+00. The columns c1,c2,c3 are ignored.

If the file is not organised in the same way, the numbering of the columns can be changed from line  18 to 20.

The path can be changed line 21
#### Stations
The stations are in a osm file, see below how to generate it.

The path can be changed line 14
#### Railways
The railways are in a osm file, see below how to generate it.

The path can be changed line 13
## Compiling 
```bash
g++ ./cpp/class.cpp ./cpp/context.cpp ./cpp/io.cpp ./cpp/main_context_city.cpp 
```
## Execution
```bash
./a.out 
```
## Output
The program write two txt files, by default ```output.txt``` and ```outputjson.txt```. To display ```output.txt```, execute :
```bash
python3 python/affichage.py output.txt 
```
```outputjson.txt``` is a file that allows to see the result with geojson. To do so, copy-paste the file on the input console at :

https://www.convertcsv.com/csv-to-geojson.htm

Then, copy the output and paste at :

https://geojson.io/#map=2/20.0/0.0

The result will be similar as the figures in the report, with in addition the pink points representing the stations.



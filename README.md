# PRE

## commandes à executer sous windows :

```bash
for($i=1; $i -lt 30; $i=$i+1){ .\a.exe ..\traj\${i}.csv ..\results\oui${i}.txt 2 }

for($i=1; $i -lt 30; $i=$i+1){ python3 affichagev2.py ..\results\oui${i}.txt }
```
## commandes à executer sous linux :
```bash
for ((i=1;i<=30;i++)); do ./a.out ../traj/${i}.csv ../resultGaussMixt/${i}.txt 1 ; done
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
#!/bin/bash
for Nk in 10 20 40 80 100 200 400
do
  /usr/bin/time -v python sparsehc-dm_load.py $Nk 2>&1
done | grep 'finished\|Maximum' > sparsehc-dm_load.txt 

for Nk in 10 20 40 80
do
  /usr/bin/time -v python fastcluster_load.py $Nk 2>&1
done | grep 'finished\|Maximum' > fastcluster_load.txt 

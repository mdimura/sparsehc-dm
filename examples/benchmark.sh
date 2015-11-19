#!/bin/bash
touch fastcluster_load.txt  sparsehc-dm_load.txt sparsehc-dm_load_pyRMSD.txt
for Nk in 10 20 40 80
do
  /usr/bin/time -v python fastcluster_load.py $Nk 2>&1 | grep 'finished\|Maximum' >> fastcluster_load.txt 
done

for Nk in 10 20 40 80 100 200 400
do
  /usr/bin/time -v python sparsehc-dm_load.py $Nk 2>&1 | grep 'finished\|Maximum' >> sparsehc-dm_load.txt 
done

for Nk in 10 20 40 80 100 200 400
do
  /usr/bin/time -v python sparsehc-dm_load_pyRMSD.py $Nk 2>&1 | grep 'finished\|Maximum' >> sparsehc-dm_load_pyRMSD.txt 
done
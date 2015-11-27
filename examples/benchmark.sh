#!/bin/bash

for Nk in 10 20 40 80
do
  /usr/bin/time -v python fastcluster_load.py $Nk 2>&1 | grep 'finished\|Maximum' >> fastcluster_perf.txt 
done

for Nk in 10 20 40
do
 ./cpptraj_cluster.sh $Nk 2>&1 >> cpptraj_perf.txt 
done

for Nk in 10 20 40 80 100 200 #400 700
do
  /usr/bin/time -v python sparsehc-dm_load.py $Nk 2>&1 | grep 'finished\|Maximum' >> sparsehc-dm_perf.txt 
done


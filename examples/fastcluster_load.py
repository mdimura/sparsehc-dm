#!/usr/bin/env python
import sys
import time
import scipy.cluster as cluster
from scipy.spatial.distance import squareform
from fastcluster import linkage
import mdtraj as md
import numpy as np
import json

Nk=1
if len(sys.argv)>1:
  Nk=int(sys.argv[1])
traj_filename='aMD-148l-first{}k.nc'.format(Nk)
top_filename='aMD-148l-all_1.pdb'

first_frame = md.load_frame(traj_filename, 0,top=top_filename)
atoms_to_keep = [a.index for a in first_frame.topology.atoms if a.name == 'CA']

start=time.time()
traj=md.load(traj_filename,top=top_filename, atom_indices=atoms_to_keep)
finishedLoad=time.time()
print ("finished loading ({}k): {}".format(Nk,finishedLoad-start))

Nframes=traj.n_frames
D=np.empty((Nframes*(Nframes-1)/2,))
j=0
for i in range(0,Nframes-1):
  rmsds=md.rmsd(traj, traj, i)[i+1:]
  D[j:j+Nframes-i-1]=rmsds
  j+=Nframes-1-i
  
#D = np.empty((traj.n_frames, traj.n_frames))
#for i in range(traj.n_frames):
    #D[i] = md.rmsd(traj, traj, i)
#D=squareform(D,checks=False) #rounding errors make D appear asymetric

finishedRMSD=time.time()
print ("finished rmsd: {}".format(finishedRMSD-finishedLoad))
#Z-matrix contains the linkage history, see more at http://docs.scipy.org/doc/scipy/reference/generated/scipy.cluster.hierarchy.linkage.html#scipy.cluster.hierarchy.linkage
Z=linkage(D, method='complete',preserve_input=False)
finishedClust=time.time()

#One would probably like to save the Z-matrix for the future, so that there is no need to redo the clustering
open('z_sparsehc-dm_load_{}k.json'.format(Nk), 'w').write(json.dumps(Z.tolist()))
print ("finished clustering: {}".format(finishedClust-finishedRMSD))
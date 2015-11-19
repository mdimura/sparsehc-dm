#!/usr/bin/env python
import sys
import time
import scipy.cluster as cluster
from scipy.spatial.distance import squareform
from fastcluster import linkage
import mdtraj as md
import numpy as np

N=int(sys.argv[1])
traj_filename='aMD-148l-first{}k.nc'.format(N)
top_filename='aMD-148l-all_1.pdb'

first_frame = md.load_frame(traj_filename, 0,top=top_filename)
atoms_to_keep = [a.index for a in first_frame.topology.atoms if a.name == 'CA']

start=time.time()
traj=md.load(traj_filename,top=top_filename, atom_indices=atoms_to_keep)
finishedLoad=time.time()
print ("finished loading ({}k): {}".format(N,finishedLoad-start))

N=traj.n_frames
D=np.empty((N*(N-1)/2,))
j=0
for i in range(0,N-1):
  rmsds=md.rmsd(traj, traj, i)[i+1:]
  D[j:j+N-i-1]=rmsds
  j+=N-1-i
  
#D = np.empty((traj.n_frames, traj.n_frames))
#for i in range(traj.n_frames):
    #D[i] = md.rmsd(traj, traj, i)
#D=squareform(D,checks=False) #rounding errors make D appear asymetric

finishedRMSD=time.time()
print ("finished rmsd: {}".format(finishedRMSD-finishedLoad))
Z=linkage(D, method='complete',preserve_input=False)
finishedClust=time.time()
print ("finished clustering: {}".format(finishedClust-finishedRMSD))

#for r in Z:
  #print("{}\t{}\t{}\t{}".format(int(r[0]),int(r[1]),r[2],int(r[3])))

#plt.figure(101)
#plt.subplot(1, 2, 1)
#plt.title("ascending")
#cluster.hierarchy.dendrogram(Z)
#plt.show()

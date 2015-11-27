import sys
import gc
from sparsehc_dm import sparsehc_dm
import random
import time
import mdtraj as md
import numpy
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

m=sparsehc_dm.InMatrix()
finishedLoad=time.time()
print ("finished loading ({}k): {}".format(Nk,finishedLoad-start))

Nframes=traj.n_frames
rmsds=list()
for i in range(0,Nframes-1):
  rmsds=md.rmsd(traj, traj, i)[i+1:].tolist()
  sparsehc_dm.push(m,rmsds,i)


#for i in range(0,Nframes-1):
  #rmsds=md.rmsd(traj, traj, i)
  #for j in range(i+1,Nframes):
    #m.push(i,j,float(rmsds[j]))
    
finishedRMSD=time.time()
print ("finished rmsd: {}".format(finishedRMSD-finishedLoad))

#Z-matrix contains the linkage history, see more at http://docs.scipy.org/doc/scipy/reference/generated/scipy.cluster.hierarchy.linkage.html#scipy.cluster.hierarchy.linkage
Z=sparsehc_dm.linkage(m,"complete")
finishedClust=time.time()

#One would probably like to save the Z-matrix for the future, so that there is no need to redo the clustering
open('z_sparsehc-dm_load_{}.json'.format(Nk), 'w').write(json.dumps(Z))
print ("finished clustering: {}".format(finishedClust-finishedRMSD))
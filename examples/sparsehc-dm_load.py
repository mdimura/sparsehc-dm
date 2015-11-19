import sys
import gc
import sparsehc_dm
import random
import time
import mdtraj as md

N=int(sys.argv[1])
traj_filename='aMD-148l-first{}k.nc'.format(N)
top_filename='aMD-148l-all_1.pdb'

first_frame = md.load_frame(traj_filename, 0,top=top_filename)
atoms_to_keep = [a.index for a in first_frame.topology.atoms if a.name == 'CA']

start=time.time()
traj=md.load(traj_filename,top=top_filename, atom_indices=atoms_to_keep)
finishedLoad=time.time()
print ("finished loading ({}k): {}".format(N,finishedLoad-start))

m=sparsehc_dm.InMatrix()
N=traj.n_frames
for i in range(0,N-1):
  rmsds=md.rmsd(traj, traj, i)[i+1:].tolist()
  sparsehc_dm.push(m,rmsds,i)
    
#for i in range(0,N-1):
  #rmsds=md.rmsd(traj, traj, i)
  #for j in range(i+1,N):
    #m.push(i,j,float(rmsds[j]))
    
finishedRMSD=time.time()
print ("finished rmsd: {}".format(finishedRMSD-finishedLoad))

Z=sparsehc_dm.linkage(m,"complete")
finishedClust=time.time()
print ("finished clustering: {}".format(finishedClust-finishedRMSD))
#for r in Z:
  #print("{}\t{}\t{}\t{}".format(r[0],r[1],r[2],r[3]))

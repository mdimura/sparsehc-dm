import sparsehc_dm
import random
import time
import numpy as np
import mdtraj as md

N=10000
traj_filename='aMD-148l-first10k.nc'
top_filename='aMD-148l-all_1.pdb'

first_frame = md.load_frame(traj_filename, 0,top=top_filename)
atoms_to_keep = [a.index for a in first_frame.topology.atoms if a.name == 'CA']

m=sparsehc_dm.InMatrix()
print ("start: "+str(time.clock()))
for i in range(0,N-1):
  ref = md.load_frame(traj_filename, i,top=top_filename, atom_indices=atoms_to_keep)
  j=i+1
  for chunk in md.iterload(traj_filename, top=top_filename, chunk=100, skip=j, atom_indices=atoms_to_keep):
    if j==N:
	break
    rmsds=md.rmsd(chunk, ref)
    for d in rmsds:
      if j==N:
	break
      m.push(i,j,float(d))
      j+=1
      
print ("finished rmsd: "+str(time.clock()))
Z=sparsehc_dm.linkage(m,"complete")
print ("finished clustering: "+str(time.clock()))
#print Z

Sparsehc-dm is a python wrapper for sparcehc distance matrix clustering algorithm, integrated with STXXL for on-disk sorting. SparseHC [![DOI for Citing SparseHC](https://img.shields.io/badge/DOI-10.1016%2Fj.procs.2014.05.001-blue.svg)](http://dx.doi.org/10.1016/j.procs.2014.05.001) is a memory-efficient hierarchical agglomerative clustering implementation. It has close to linear memory complexity, enabling clustering of ~900000 structures/points on 32GB RAM.

Usage example:
```python
import mdtraj as md
from sparsehc_dm import sparsehc_dm

traj_filename='traj.nc'
top_filename='top.pdb'

first_frame = md.load_frame(traj_filename, 0,top=top_filename)
atoms_to_keep = [a.index for a in first_frame.topology.atoms if a.name == 'CA']
traj=md.load(traj_filename,top=top_filename, atom_indices=atoms_to_keep)

m=sparsehc_dm.InMatrix()
N=traj.n_frames
for i in range(0,N-1):
  rmsds=md.rmsd(traj, traj, i)[i+1:].astype(float)
  sparsehc_dm.push(m,rmsds,i)

Z=linkage(D, method='complete',preserve_input=False)
```
###Instalation
####Prerequisites: boost graph and stxxl library
```
sudo apt-get install libboost-graph-dev libstxxl-dev libstxxl1
```
####Building:
```
git clone https://github.com/Burning-Daylight/sparsehc-dm.git sparcehc-dm
cd sparsehc-dm
mkdir build
cd build
cmake ..
make
sudo make install
```
Sparsehc-dm is a python wrapper for sparcehc distance matrix clustering algorithm, integrated with STXXL[![DOI for Citing STXXL](https://img.shields.io/badge/DOI-10.1002%2Fspe.v38%3A6-blue.svg)](http://dx.doi.org/10.1002/spe.v38:6) for on-disk sorting. SparseHC [![DOI for Citing SparseHC](https://img.shields.io/badge/DOI-10.1016%2Fj.procs.2014.05.001-blue.svg)](http://dx.doi.org/10.1016/j.procs.2014.05.001) is a memory-efficient hierarchical agglomerative clustering implementation. It has close to linear memory complexity, enabling clustering of ~900000 structures/points on 32GB RAM.

#### Usage example:
```python
import mdtraj as md
from sparsehc_dm import sparsehc_dm

traj_filename='traj.nc'
top_filename='top.pdb'

traj=md.load(traj_filename,top=top_filename)

m=sparsehc_dm.InMatrix()
N=traj.n_frames
for i in range(0,Nframes-1):
  rmsds=md.rmsd(traj, traj, i)
  for j in range(i+1,Nframes):
    m.push(i,j,float(rmsds[j]))

Z=sparsehc_dm.linkage(m,"complete")
```
### Instalation
#### Prerequisites: boost graph and stxxl library
```
sudo apt-get install libboost-graph-dev libstxxl-dev libstxxl1
```
#### Building:
```
git clone https://github.com/Burning-Daylight/sparsehc-dm.git sparsehc-dm
cd sparsehc-dm
mkdir build
cd build
cmake ..
make
sudo make install
```

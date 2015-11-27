#!/bin/bash
Nk=$1
PARMFILE=aMD-148l-all_1.pdb
TRAJINFILE=aMD-148l-first${Nk}k.nc

EPSILON=30.0

cat <<EOF | /usr/bin/time -v cpptraj.OMP
parm ${PARMFILE}
trajin ${TRAJINFILE}

cluster hieragglo epsilon $EPSILON complete rms :1-9999@CA out frame_clstr.dat summary clstr-summary.dat \
info info.dat

go
EOF
cd ..
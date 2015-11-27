from distutils.core import setup, Extension
import os
setup (name = 'sparsehc_dm',
       version = '',
       description = 'Python wrapper for sparsehc_dm with STXXL sorting integrated',
       packages=['sparsehc_dm'],
       package_dir={'sparsehc_dm':''},
       package_data={'sparsehc_dm':['sparsehc_dm.so',]},
       )
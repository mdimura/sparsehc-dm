from distutils.core import setup
from distutils.extension import Extension
setup(name="sparsehc-dm",    
      ext_modules=[
                    Extension("sparsehc-dm",["sparsehc-dm_python.cpp",],
                    library_dirs=["/usr/local/lib",],
                    libraries=["",],
                    include_dirs=[".","../src"],
                    depends=[]),
                    ]
     )
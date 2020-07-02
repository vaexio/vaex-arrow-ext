import os
from setuptools import setup, Extension

# from Cython.Build import cythonize
import numpy as np
import pyarrow as pa
import pybind11

dirname = os.path.dirname(__file__)

# ext_modules = cythonize("example.pyx")

ext_vaex_arrow = Extension("vaex_arrow_ext.ext",[os.path.relpath(os.path.join(dirname, "src/ext.cpp"))])


ext_modules = [ext_vaex_arrow]

for ext in ext_modules:
    # The Numpy C headers are currently required
    ext.include_dirs.append(np.get_include())
    ext.include_dirs.append(pa.get_include())
    ext.include_dirs.append(pybind11.get_include())

    ext.libraries.extend([k for k in pa.get_libraries() if k != "arrow"])
    ext.library_dirs.extend(pa.get_library_dirs())

    if os.name == 'posix':
        ext.extra_compile_args.append('-std=c++11')

    # Try uncommenting the following line on Linux
    # if you get weird linker errors or runtime crashes
    # ext.define_macros.append(("_GLIBCXX_USE_CXX11_ABI", "0"))


setup(
    name="vaex_arrow_ext",
    ext_modules=ext_modules
)

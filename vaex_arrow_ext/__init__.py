from pathlib import Path
import ctypes
import pyarrow as pa

def _set_arrow_symbol_resolution(flag):
    for dir in map(Path, pa.get_library_dirs()):
        arrow_path = dir / 'libarrow.so'
        arrow_python_path = dir / 'libarrow_python.so'
        if arrow_path.exists() and arrow_python_path.exists():
            arrow_python = ctypes.CDLL(arrow_path, flag)
            libarrow_python = ctypes.CDLL(arrow_python_path, flag)
            break
_set_arrow_symbol_resolution(ctypes.RTLD_GLOBAL)
from . import ext

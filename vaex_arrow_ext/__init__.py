from pathlib import Path
import ctypes
import pyarrow as pa

print("pa.get_library_dirs()", pa.get_library_dirs())
for dir in map(Path, pa.get_library_dirs()):
    arrow_path = dir / 'libarrow.so'
    arrow_python_path = dir / 'libarrow_python.so'
    print(f"Testing {arrow_path} and {arrow_python_path}")
    if arrow_path.exists() and arrow_python_path.exists():
        arrow_python = ctypes.CDLL(arrow_path, ctypes.RTLD_GLOBAL)
        libarrow_python = ctypes.CDLL(arrow_python_path, ctypes.RTLD_GLOBAL)
        print("loaded...")
        break

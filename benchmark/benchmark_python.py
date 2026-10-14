import subprocess
import json

exec_path = "./build/kvstore_benchmark"
ops_num = 100000
threads_num = [1, 2, 4, 10, 12, 16]

def run_cpp_benchmark(threads: int, ops: int) -> dict:
    cmd = [exec_path, str(ops), str(threads)]
    try:
        result = subprocess.run(cmd, capture_output=True, text=True, check=True)
        data = json.loads(result.stdout)
        return data
    except FileNotFoundError:
        print(f"Error. Make sure that the executable file is found at the path: {exec_path}.")
        print("Make sure that the CMake file was compiled.")
        exit(1)
    except json.JSONDecodeError:
        print(f"Error in parsing JSON file from C++.")
        print(result.stdout)
        exit(1)

def main():
    results = []
    for t in threads_num:
        print(f"Running {t} threads.")
        data = run_cpp_benchmark(t, ops_num)
        results.append(data)
        print(f"  -> Result: {data['ops_per_sec']:,.0f} op/sec (Time: {data['duration_ms']:.2f} ms)")
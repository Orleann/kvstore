import subprocess
import json
import matplotlib.pyplot as plt

exec_path = "./build/kvstore_bench"
ops_num = 100000
threads_num = [1, 2, 4, 10, 12, 16]


def run_cpp_benchmark(threads: int, ops: int) -> dict:

    cmd = [exec_path, str(threads), str(ops)]
    try:
        result = subprocess.run(cmd, capture_output=True, text=True, check=True)
        return json.loads(result.stdout)
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

    threads = [r['threads'] for r in results]
    ops_per_sec = [r['ops_per_sec'] for r in results]

    plt.figure(figsize=(10, 5))
    plt.plot(threads, ops_per_sec, marker="o", label="C++ benchmark")

    plt.xlabel('Number of threads', fontsize=12)
    plt.ylabel('Operations per second', fontsize=12)
    plt.xticks(threads_num)
    plt.legend(fontsize=11)

    output_image = "result.png"
    plt.savefig(output_image)
    plt.show()
    print(f"Result: {output_image}")

if __name__ == "__main__":
    main()
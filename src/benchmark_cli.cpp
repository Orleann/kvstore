//
// Created by Sebastian Sobczyński on 14/10/2026.
//

#include <iostream>
#include <thread>
#include <chrono>
#include <vector>
#include "kvstore/kvstore.h"

int main(int argc, char **argv) {
    int num_threads = (argc > 1) ? std::stoi(argv[1]) : 4;
    int operations = (argc > 2) ? std::stoi(argv[2]) : 100000;

    kv::KVStore store;

    auto start_time = std::chrono::high_resolution_clock::now();
    std::vector<std::thread> threads;
    threads.reserve(num_threads);

    for (int i = 0; i < num_threads; i++) {
        threads.emplace_back([&store, i, operations]() {
            for (int j = 0; j < operations; j++) {
                std::string key = "key:" + std::to_string(i) + ":" + std::to_string(j);
                if (j % 2 == 0) {
                    store.set(key, "Benchmark value.");
                }
                else {
                    auto val = store.get(key);
                    (void)val;
                }
            }
        });
    }

    for (auto& th : threads) {
        if (th.joinable()) {
            th.join();
        }
    }
    threads.clear();

    auto end_time = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double, std::milli> time_duration = end_time - start_time;

    long long total_operations = static_cast<long long>(num_threads) * operations;

    double operations_per_second = 0.0;
    if (time_duration.count() > 0.0) {
        operations_per_second = (static_cast<double>(total_operations) / time_duration.count()) * 1000.0;
    }

    std::cout << "{\n"
              << "  \"threads\": " << num_threads << ",\n"
              << "  \"total_ops\": " << total_operations << ",\n"
              << "  \"duration_ms\": " << time_duration.count() << ",\n"
              << "  \"ops_per_sec\": " << operations_per_second << "\n"
              << "}\n";
}
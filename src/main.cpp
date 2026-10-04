#include <chrono>
#include <iostream>
#include <optional>

#include "kvstore/kvstore.h"
#include "kvstore/ttl_worker.h"

using namespace std::chrono_literals;

static void result(const std::string& key, const std::optional<std::string>& value) {
    if (value.has_value()) {
        std::cout << "Found " << key << " -> \"" << *value << "\"" << std::endl;
    }
    else {
        std::cout << "Missing " << key << std::endl;
    }
}

int main() {
    kv::KVStore store;

    {
        //Inserting keys
        store.set("user:1001", "Jan");
        store.set("app:version", "0.0.1");
        store.set("app:version2", "0.1.1");

        result("user:1001", store.get("user:1001"));
        result("app:version", store.get("app:version"));
        result("app:version2", store.get("app:version2"));

        //Deleting a key
        bool deleted = store.del("app:version");
        std::cout << "Deleted app:version: " << std::boolalpha << deleted << std::endl;
        result("app:version", store.get("app:version"));

        //Short lived key
        store.set("temp_token", "temp_value", std::chrono::milliseconds(500));
        std::cout << "Immediately after insert: ";
        result("temp_token", store.get("temp_token"));
        std::cout << "Sleeping for 500ms..." << std::endl;
        std::this_thread::sleep_for(std::chrono::milliseconds(500));
        result("temp_token", store.get("temp_token"));

        std::cout << std::endl;
    } //Basic in-memory KV Operations
    {
        kv::TTLWorker worker(store, 100ms); //worker starts automatically via RAII
        store.set("token_q", "quick", 150ms);
        store.set("token_s", "slow", 500ms);

        std::cout << "Immediately after insert: " << std::endl;
        result("token_q", store.get("token_q"));
        result("token_s", store.get("token_s"));

        std::cout << "Waiting for 250ms..." << std::endl;
        std::this_thread::sleep_for(std::chrono::milliseconds(250));
        result("token_q", store.get("token_q"));
        result("token_s", store.get("token_s"));
        std::cout << "Waiting for additional 300ms..." << std::endl;
        std::this_thread::sleep_for(std::chrono::milliseconds(300));
        result("token_q", store.get("token_q"));
        result("token_s", store.get("token_s"));
    } //Background worker (running periodic purge_expired sweeps)
    {
        kv::TTLWorker stress_worker(store, 50ms);
        constexpr int number_of_threads = 4;
        constexpr int operations_per_thread = 1000;
        std::vector<std::jthread> threads;

        for (int t = 0; t < number_of_threads; ++t) {
            threads.emplace_back([&store, t]() {
                for (int j = 0; j < operations_per_thread; ++j) {
                    std::string k = "k: " + std::to_string(t) + ":" + std::to_string(t);
                    store.set(k, "value", 50ms);
                }
            });
        }

        for (int t = 0; t < number_of_threads; ++t) {
            threads.emplace_back([&store, t]() {
                for (int i = 0; i < operations_per_thread; ++i) {
                    std::string k = "k:" + std::to_string(t) + ":" + std::to_string(i);
                    volatile auto res = store.get(k);
                    (void)res;
                }
            });
        }

        for (auto& th : threads) {
            th.join();
        }
        std::cout << "Finished " << (number_of_threads * operations_per_thread * 2) << " concurrent operations.";
    } //Multithread stress test
}
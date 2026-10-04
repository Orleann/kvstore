//
// Created by Sebastian Sobczyński on 21/09/2026.
//

#pragma once

#include "kvstore/kvstore.h"
#include <chrono>
#include <thread>

namespace kv {
    class TTLWorker {
    public:
        explicit TTLWorker(KVStore& store, std::chrono::milliseconds interval = std::chrono::milliseconds(1000))
        : store_(store), interval_(interval) {
            worker_ = std::jthread([this](const std::stop_token &stop_token) {
                run(stop_token);
            });
        }
    private:
        void run(const std::stop_token& stop_token) const {
            while (!stop_token.stop_requested()) {
                store_.purge_expired();
                for (int i = 0; i < 10 && !stop_token.stop_requested(); ++i) {
                    std::this_thread::sleep_for(interval_ / 10);
                }
            }
        }
        KVStore& store_;
        std::chrono::milliseconds interval_;
        std::jthread worker_;
    };
}
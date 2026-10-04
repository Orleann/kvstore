//
// Created by Sebastian Sobczyński on 21/09/2026.
//

#pragma once //Instead of the whole def. using #ifdef etc.

#include <string>
#include <optional>
#include <unordered_map>
#include <shared_mutex>
#include <chrono>

namespace kv {
    using Clock = std::chrono::steady_clock;
    using TimePoint = Clock::time_point;

    struct Entry {
        std::string value;
        std::optional<TimePoint> expiry;

        [[nodiscard]] bool is_expired(TimePoint now) const noexcept {
            return expiry.has_value() && *expiry <= now;
        }
    };

    class KVStore {
    public:
        KVStore() = default;

        void set(std::string key, std::string value, std::optional<std::chrono::milliseconds> ttl = std::nullopt);
        [[nodiscard]] std::optional<std::string> get(const std::string& key);
        bool del(const std::string& key);

        size_t purge_expired();

    private:
        mutable std::shared_mutex mutex_;
        std::unordered_map<std::string, Entry> table_;
    };
};
//
// Created by Sebastian Sobczyński on 21/09/2026.
//

#include "kvstore/kvstore.h"
#include <mutex>

namespace kv {
    void KVStore::set(std::string key, std::string value, std::optional<std::chrono::milliseconds> ttl) {
        std::unique_lock lock(mutex_);

        std::optional<TimePoint> expiry;
        if (ttl) {
            expiry = Clock::now() + *ttl;
        }
        table_[std::move(key)] = Entry{std::move(value), expiry};
    }

    std::optional<std::string> KVStore::get(const std::string& key) {
        auto now = Clock::now();

        {
            std::shared_lock lock(mutex_);
            auto it = table_.find(key);
            if (it == table_.end()) {
                return std::nullopt;
            }
            if (!it->second.is_expired(now)) {
                return it->second.key;
            }
        }

        std::unique_lock lock(mutex_);
        auto it = table_.find(key);
    }
}
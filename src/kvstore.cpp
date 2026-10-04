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
                return it->second.value;
            }
        }

        std::unique_lock lock(mutex_);
        auto it = table_.find(key);
        if (it != table_.end() && it->second.is_expired(now)) {
            table_.erase(it);
            return std::nullopt;
        }
        return (it != table_.end() ? std::optional(it->second.value) : std::nullopt);
    }

    bool KVStore::del(const std::string& key) {
        std::unique_lock lock(mutex_);
        return table_.erase(key);
    }

    size_t KVStore::purge_expired() {
        auto now = Clock::now();
        std::unique_lock lock(mutex_);
        size_t count = 0;

        for (auto it = table_.begin(); it != table_.end();) {
            if (it->second.is_expired(now)) {
                it = table_.erase(it);
                ++count;
            }
            else {
                ++it;
            }
        }
        return count;
    }
}

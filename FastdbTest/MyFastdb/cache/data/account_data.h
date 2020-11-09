#pragma once

namespace sdbus { class VariantMap; }
class AccountCache;

class AccountData {
public:
    static void SetMap(const AccountCache* cache, sdbus::VariantMap *mp, bool isFull = true);
    static void SetMapMore(const AccountCache* cache, sdbus::VariantMap *mp);

    static void SetCache(const sdbus::VariantMap *mp, AccountCache *cache);
};

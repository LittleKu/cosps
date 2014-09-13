using System;

namespace CommonLib.Cache
{
    public enum CacheItemRemovedReason
    {
        REMOVED,

        EXPIRED
    }

    public delegate object CacheItemRefreshAction(string removedKey, object expiredValue, CacheItemRemovedReason removalReason);
}

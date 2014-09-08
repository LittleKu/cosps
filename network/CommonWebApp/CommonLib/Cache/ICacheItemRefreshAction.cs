using System;

namespace CommonLib.Cache
{
    public enum CacheItemRemovedReason
    {
        REMOVED,

        EXPIRED
    }

    public interface ICacheItemRefreshAction
    {
        object Refresh(string removedKey, object expiredValue, CacheItemRemovedReason removalReason);
    }
}

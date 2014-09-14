using System;

namespace CommonLib.Cache
{
    public delegate object CacheItemRefreshAction(string removedKey, object expiredValue);
}

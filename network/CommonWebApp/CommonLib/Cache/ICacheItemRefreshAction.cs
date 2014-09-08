using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace CommonLib.Cache
{
    public interface ICacheItemRefreshAction
    {
        object Refresh(string removedKey, object expiredValue, CacheItemRemovedReason removalReason);
    }
}

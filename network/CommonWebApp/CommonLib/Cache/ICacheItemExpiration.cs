using System;

namespace CommonLib.Cache
{
    public class LastExpirationInfo
    {
        public DateTime LastExpiration { get; set; }

        public DateTime LastBeginRefresh { get; set; }

        public DateTime LastEndRefresh { get; set; }

        public LastExpirationInfo()
        {
            DateTime now = DateTime.Now;

            this.LastExpiration = this.LastBeginRefresh = this.LastEndRefresh = now;
        }
    }

    public interface ICacheItemExpiration
    {
        DateTime NextExpiration(LastExpirationInfo lastInfo);
    }
}

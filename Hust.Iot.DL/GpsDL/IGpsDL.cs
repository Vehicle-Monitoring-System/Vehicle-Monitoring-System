using Hust.Iot.Common;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Hust.Iot.DL
{
    public interface IGpsDL : IBaseDL<GPSRecord>
    {
        public Task<List<Tuple<GPSRecord,GPSRecord>>> GetPairs(string deviceId, DateTime time);

        public Task<List<GPSRecord>> GetTripAsync(string deviceId, DateTime start, DateTime end);

        public Task<GPSRecord> GetRecentLocationAsync(string deviceId);
    }
}

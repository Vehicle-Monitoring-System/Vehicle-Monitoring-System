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
        public Task<List<Tuple<GPSRecord,GPSRecord>>> GetPairs(DateTime time);

        public Task<List<GPSRecord>> GetTripAsync(DateTime start, DateTime end);
    }
}

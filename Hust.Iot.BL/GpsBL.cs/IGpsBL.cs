using Hust.Iot.Common;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Hust.Iot.BL.GpsBL.cs
{
    public interface IGpsBL
    {
        public Task CreateRecordAsync(GPSRecord record);
        public Task<List<Tuple<GPSRecord, GPSRecord>>> GetPairsAsync(DateTime time);

        public Task<List<GPSRecord>> GetTripAsync(DateTime start, DateTime end);
    }
}

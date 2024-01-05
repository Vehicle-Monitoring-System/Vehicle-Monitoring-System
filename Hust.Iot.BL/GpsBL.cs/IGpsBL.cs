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
        public Task<List<Tuple<GPSRecord, GPSRecord>>> GetPairsAsync(string deviceId, DateTime time);

        public Task<List<GPSRecord>> GetTripAsync(string deviceId, DateTime start, DateTime end);

        public Task JoinChanel(string deviceId);
    }
}

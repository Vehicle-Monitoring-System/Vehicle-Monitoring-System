using Hust.Iot.Common;
using Hust.Iot.DL;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Hust.Iot.BL.GpsBL.cs
{
    public class GpsBL : IGpsBL
    {
        private readonly IGpsDL _gpsDL;

        public GpsBL(IGpsDL gpsDL)
        {
            _gpsDL = gpsDL;
        }

        public async Task CreateRecordAsync(GPSRecord record)
        {
            await _gpsDL.CreateRecordAsync(record);
            return;
        }

        public async Task<List<Tuple<GPSRecord, GPSRecord>>> GetPairsAsync(DateTime time)
        {
            var result = await _gpsDL.GetPairs(time);
            return result;
        }

        public async Task<List<GPSRecord>> GetTripAsync(DateTime start, DateTime end)
        {
            var result = await _gpsDL.GetTripAsync(start, end);
            return result;
        }
    }
}

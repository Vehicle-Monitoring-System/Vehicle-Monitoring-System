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
        private readonly IDriverDL _driverDL;
        private readonly IDeviceDL _deviceDL;
        private readonly IHiveMQ _hiveMQ;

        public GpsBL(IGpsDL gpsDL, IDeviceDL deviceDL, IDriverDL driverDL, IHiveMQ hiveMQ)
        {
            _gpsDL = gpsDL;
            _deviceDL = deviceDL;
            _driverDL = driverDL;
            _hiveMQ = hiveMQ;
        }

        public async Task CreateRecordAsync(GPSRecord record)
        {
            if (record.DriverId != null && record.DriverId != "")
            {
                var driver = await _driverDL.GetRecordByIdAsync(record.DriverId);
                record.DriverName = driver.DriverName;
                record.DriverLicense = driver.DriverLicense;
            }
            if (record.DeviceId != null && record.DeviceId != "")
            {
                var device = await _deviceDL.GetRecordByIdAsync(record.DeviceId);
                record.DeviceName = device.DeviceName;
                record.DeviceLicensePlates = device.DeviceLicensePlates;
            }
            if (Math.Abs(Double.Parse(record.Latitude)) > 0)
            {
                _hiveMQ.PublishTopic(record.DeviceId, record);
            } else
            {
                var recent = await _gpsDL.GetRecentLocationAsync(record.DeviceId);
                object data =   new
                                {
                                    Error = true,
                                    ErrorMessage = "Mất kết nối GPS",
                                    RecentLocation = recent
                                };
                _hiveMQ.PublishTopic(record.DeviceId, data);
            }
            await _gpsDL.CreateRecordAsync(record);
            return;
        }

        public async Task<object> GetPairsAsync(string deviceId, DateTime time)
        {
            var result = await _gpsDL.GetPairs(deviceId, time);
            return result;
        }

        public async Task<List<GPSRecord>> GetTripAsync(string deviceId, DateTime start, DateTime end)
        {
            var result = await _gpsDL.GetTripAsync(deviceId, start, end);
            return result;
        }
    }
}

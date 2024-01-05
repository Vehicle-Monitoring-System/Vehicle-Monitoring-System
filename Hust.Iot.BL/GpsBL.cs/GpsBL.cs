using Hust.Iot.Common;
using Hust.Iot.DL;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using Microsoft.AspNetCore.SignalR;

namespace Hust.Iot.BL.GpsBL.cs
{
    public class GpsBL : Hub, IGpsBL
    {
        private readonly IGpsDL _gpsDL;
        private readonly IDriverDL _driverDL;
        private readonly IDeviceDL _deviceDL;

        public GpsBL(IGpsDL gpsDL, IDeviceDL deviceDL, IDriverDL driverDL)
        {
            _gpsDL = gpsDL;
            _deviceDL = deviceDL;
            _driverDL = driverDL;
        }

        public async Task CreateRecordAsync(GPSRecord record)
        {
            var driver = await _driverDL.GetRecordByIdAsync(record.DriverId);
            var device = await _deviceDL.GetRecordByIdAsync(record.DeviceId);
            record.DeviceName = device.DeviceName;
            record.DeviceLicensePlates = device.DeviceLicensePlates;
            record.DriverName = driver.DriverName;
            record.DriverLicense = driver.DriverLicense;
            if (Clients != null)
            {
                object data = Math.Abs(Double.Parse(record.Latitude))>0?record :
                                new
                                {
                                    Error = true,
                                    ErrorMessage = "Mất kết nối GPS"
                                };
                await Clients.Group(record.DeviceId).SendAsync("ReceiveMessage", data);
            }
            await _gpsDL.CreateRecordAsync(record);
            return;
        }

        public async Task<List<Tuple<GPSRecord, GPSRecord>>> GetPairsAsync(string deviceId, DateTime time)
        {
            var result = await _gpsDL.GetPairs(deviceId, time);
            return result;
        }

        public async Task<List<GPSRecord>> GetTripAsync(string deviceId, DateTime start, DateTime end)
        {
            var result = await _gpsDL.GetTripAsync(deviceId, start, end);
            return result;
        }

        public async Task JoinChanel(string deviceId)
        {
            await Groups.AddToGroupAsync(Context.ConnectionId, deviceId);
            if (Clients != null)
            {
                var recent = await _gpsDL.GetRecentLocationAsync(deviceId);
                await Clients.Group(deviceId).SendAsync("ReceiveMessage", recent);
            }
        }
    }
}

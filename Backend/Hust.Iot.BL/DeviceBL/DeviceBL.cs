using Hust.Iot.Common;
using Hust.Iot.DL;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Hust.Iot.BL
{
    public class DeviceBL : IDeviceBL
    {
        private readonly IDeviceDL _deviceDL;

        public DeviceBL(IDeviceDL deviceDL)
        {
            _deviceDL = deviceDL;
        }
        public async Task<IEnumerable<Device>> GetAllAsync()
        {
            var result = await _deviceDL.GetRecordsAsync();
            return result;

        }

        public async Task<Device> GetByIdAsync(string id)
        {
            var result = await _deviceDL.GetRecordByIdAsync(id);
            return result;
        }
    }
}

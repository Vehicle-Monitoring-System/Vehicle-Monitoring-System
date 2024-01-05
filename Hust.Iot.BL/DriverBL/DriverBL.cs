using Hust.Iot.Common;
using Hust.Iot.DL;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Hust.Iot.BL
{
    public class DriverBL : IDriverBL
    {
        private readonly IDriverDL _driverDL;

        public DriverBL(IDriverDL driverDL)
        {
            _driverDL = driverDL;
        }
        public async Task<IEnumerable<Driver>> GetAllAsync()
        {
            var result = await _driverDL.GetRecordsAsync();
            return result;

        }

        public async Task<Driver> GetByIdAsync(string id)
        {
            var result = await _driverDL.GetRecordByIdAsync(id);
            return result;
        }
    }
}

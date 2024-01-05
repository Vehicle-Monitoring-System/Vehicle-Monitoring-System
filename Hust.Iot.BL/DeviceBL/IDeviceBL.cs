using Hust.Iot.Common;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Hust.Iot.BL
{
    public interface IDeviceBL
    {
        public Task<IEnumerable<Device>> GetAllAsync();
        public Task<Device> GetByIdAsync(string id);
    }
}

using Hust.Iot.Common;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Hust.Iot.BL
{
    public interface IDriverBL
    {
        public Task<IEnumerable<Driver>> GetAllAsync();
        public Task<Driver> GetByIdAsync(string id);
    }
}

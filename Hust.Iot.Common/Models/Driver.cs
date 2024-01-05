using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Hust.Iot.Common
{
    public class Driver : IModel
    {
        public string DriverId { get; set; }
        public string DriverLicense {  get; set; }
        public string DriverName { get; set; }

        public string GetId()
        {
            return DriverId;
        }
    }
}

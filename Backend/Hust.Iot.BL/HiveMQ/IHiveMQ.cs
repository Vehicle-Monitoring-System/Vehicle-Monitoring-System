using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Hust.Iot.BL
{
    public interface IHiveMQ
    {
        public void PublishTopic(string id, object data);
    }
}

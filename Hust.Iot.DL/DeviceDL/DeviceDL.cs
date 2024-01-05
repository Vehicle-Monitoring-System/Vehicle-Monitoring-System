﻿using Hust.Iot.Common;
using MongoDB.Driver;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Hust.Iot.DL
{
    public class DeviceDL : BaseDL<Device>, IDeviceDL
    {
        public DeviceDL(IMongoDatabase database, IClientSessionHandle session) : base(database, session)
        {
        }
    }
}

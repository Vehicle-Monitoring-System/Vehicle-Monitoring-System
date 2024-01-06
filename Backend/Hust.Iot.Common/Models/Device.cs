using MongoDB.Bson.Serialization.Attributes;
using MongoDB.Bson;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Hust.Iot.Common
{
    public class Device : IModel
    {
        [BsonId]
        [BsonRepresentation(BsonType.ObjectId)]
        public string _id;
        public string DeviceId { get; set; }
        public string DeviceLicensePlates { get; set; }
        public string DeviceName { get; set; }

        public string GetId()
        {
            return DeviceId;
        }
    }
}

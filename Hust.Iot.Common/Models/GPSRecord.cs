using MongoDB.Bson;
using MongoDB.Bson.Serialization.Attributes;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Hust.Iot.Common
{
    public class GPSRecord : IModel
    {
        [BsonId]
        [BsonRepresentation(BsonType.ObjectId)]
        public string _id;
        public string DeviceId { get; set; }
        public string? DriverId {  get; set; }
        public DateTime Time { get; set; }
        public string Latitude { get; set; }
        public string Longitude { get; set; }
        public double LocationInfo { get; set; }
        public int SatelliteNum { get; set; }
        public double Speed { get; set; }
        public double Distance { get; set; }
        public string? DriverLicense { get; set; }
        public string? DriverName { get; set; }

        public string? DeviceLicensePlates { get; set; }
        public string? DeviceName { get; set; }

        public Status Status { get; set; }

        public string GetId()
        {
            return DeviceId;
        }
    }
}

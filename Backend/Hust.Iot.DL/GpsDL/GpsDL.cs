using Hust.Iot.Common;
using MongoDB.Bson;
using MongoDB.Driver;
using System;
using System.Collections.Generic;
using System.Data;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Hust.Iot.DL
{
    public class GpsDL : BaseDL<GPSRecord>, IGpsDL
    {
        public GpsDL(IMongoDatabase database, IClientSessionHandle session) : base(database, session)
        {
        }

        public async Task<object> GetPairs(string deviceId, DateTime time)
        {

            // Lấy tất cả các bản ghi trong một ngày
            var x = DateTime.Now;
            var y = DateTime.UtcNow;
            var dbTime = time; // Thời gian lấy từ cơ sở dữ liệu
            var vietnamTimeZone = TimeZoneInfo.FindSystemTimeZoneById("SE Asia Standard Time"); // Múi giờ Việt Nam
            var startDate = TimeZoneInfo.ConvertTimeToUtc(dbTime, vietnamTimeZone);
            var endDate = TimeZoneInfo.ConvertTimeToUtc(dbTime.AddDays(1), vietnamTimeZone);

            var filter = Builders<GPSRecord>.Filter.And(
                Builders<GPSRecord>.Filter.Gte("Time", startDate),
                Builders<GPSRecord>.Filter.Lt("Time", endDate),
                Builders<GPSRecord>.Filter.Eq("DeviceId", deviceId)
            );

            var records = await _collection.Find(filter).SortBy(record => record.Time).ToListAsync();

            // Xử lý bản ghi trong C#
            var pairedRecords = new List<Tuple<GPSRecord, GPSRecord>>();
            GPSRecord currentActiveRecord = null;
            GPSRecord prev = null;

            foreach (var record in records)
            {
                if (record.Status == Status.Active)
                {
                    // Nếu có một bản ghi Active mới, cập nhật currentActiveRecord
                    if (currentActiveRecord == null && record.Latitude != "0.000000")
                    {
                        currentActiveRecord = record;
                    }
                }
                else if ( (record.Status == Status.Parking || record.Latitude == "0.000000") && currentActiveRecord != null)
                {
                    // Nếu có một bản ghi Parking và có một bản ghi Active trước đó,
                    // thì chúng ta có một cặp và thêm vào danh sách
                    pairedRecords.Add(new Tuple<GPSRecord, GPSRecord>(currentActiveRecord, prev));
                    currentActiveRecord = null; // Reset currentActiveRecord
                }
                prev = record;
            }
            if (records.Any() && records[records.Count-1].Status == Status.Active)
            {
                pairedRecords.Add(new Tuple<GPSRecord, GPSRecord>(records[records.Count - 1], null));
            }

            // Bây giờ pairedRecords chứa các cặp bản ghi với bản ghi đầu tiên là Active và bản ghi thứ hai là Parking



            return new
            {
                Start = startDate,
                End = endDate,
                x = x,
                y = y,
                Data = pairedRecords,
            } ;
        }

        public async Task<GPSRecord> GetRecentLocationAsync(string deviceId)
        {
            TimeZoneInfo vietnamTimeZone = TimeZoneInfo.FindSystemTimeZoneById("SE Asia Standard Time");

            // Lấy thời gian hiện tại theo múi giờ Việt Nam
            DateTime currentTime = TimeZoneInfo.ConvertTimeFromUtc(DateTime.UtcNow, vietnamTimeZone);


            // Xây dựng bộ lọc
            var filter = Builders<GPSRecord>.Filter.And(
                Builders<GPSRecord>.Filter.Eq("DeviceId", deviceId),
                Builders<GPSRecord>.Filter.Lte("Time", currentTime),
                Builders<GPSRecord>.Filter.Ne("Latitude", "0.000000")
            );

            // Sắp xếp theo trường Time giảm dần để lấy thời điểm gần nhất
            var sort = Builders<GPSRecord>.Sort.Descending("Time");

            // Thực hiện truy vấn và lấy bản ghi đầu tiên
            var result = await _collection.Find(filter).Sort(sort).FirstOrDefaultAsync();

            return result;
        }

        public async Task<List<GPSRecord>> GetTripAsync(string deviceId, DateTime start, DateTime end)
        {
            var result = await _collection.FindAsync<GPSRecord>(x=> x.DeviceId == deviceId && x.Time >= start && x.Time <= end && x.Latitude != "0.000000");
            return result.ToList();
        }
    }
}

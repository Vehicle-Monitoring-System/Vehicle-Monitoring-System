using Hust.Iot.Common;
using MongoDB.Bson;
using MongoDB.Driver;
using System;
using System.Collections.Generic;
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

        public async Task<List<Tuple<GPSRecord, GPSRecord>>> GetPairs(string deviceId, DateTime time)
        {

            // Lấy tất cả các bản ghi trong một ngày
            var startDate = time;
            var endDate = startDate.AddDays(1);

            var filter = Builders<GPSRecord>.Filter.And(
                Builders<GPSRecord>.Filter.Gte("Time", startDate),
                Builders<GPSRecord>.Filter.Lt("Time", endDate),
                Builders<GPSRecord>.Filter.Eq("DeviceId", deviceId)
            );

            var records = await _collection.Find(filter).SortBy(record => record.Time).ToListAsync();

            // Xử lý bản ghi trong C#
            var pairedRecords = new List<Tuple<GPSRecord, GPSRecord>>();
            GPSRecord currentActiveRecord = null;

            foreach (var record in records)
            {
                if (record.Status == Status.Active)
                {
                    // Nếu có một bản ghi Active mới, cập nhật currentActiveRecord
                    if (currentActiveRecord == null)
                    {
                        currentActiveRecord = record;
                    }
                }
                else if (record.Status == Status.Parking && currentActiveRecord != null)
                {
                    // Nếu có một bản ghi Parking và có một bản ghi Active trước đó,
                    // thì chúng ta có một cặp và thêm vào danh sách
                    pairedRecords.Add(new Tuple<GPSRecord, GPSRecord>(currentActiveRecord, record));
                    currentActiveRecord = null; // Reset currentActiveRecord
                }
            }
            if (records.Any() && records[records.Count-1].Status == Status.Active)
            {
                pairedRecords.Add(new Tuple<GPSRecord, GPSRecord>(records[records.Count - 1], null));
            }

            // Bây giờ pairedRecords chứa các cặp bản ghi với bản ghi đầu tiên là Active và bản ghi thứ hai là Parking



            return pairedRecords ;
        }

        public async Task<GPSRecord> GetRecentLocationAsync(string deviceId)
        {
            DateTime currentTime = DateTime.UtcNow;

            // Xây dựng bộ lọc
            var filter = Builders<GPSRecord>.Filter.And(
                Builders<GPSRecord>.Filter.Eq("DeviceId", deviceId),
                Builders<GPSRecord>.Filter.Lte("Time", currentTime)
            );

            // Sắp xếp theo trường Time giảm dần để lấy thời điểm gần nhất
            var sort = Builders<GPSRecord>.Sort.Descending("Time");

            // Thực hiện truy vấn và lấy bản ghi đầu tiên
            var result = await _collection.Find(filter).Sort(sort).FirstOrDefaultAsync();

            return result;
        }

        public async Task<List<GPSRecord>> GetTripAsync(string deviceId, DateTime start, DateTime end)
        {
            var result = await _collection.FindAsync<GPSRecord>(x=> x.DeviceId == deviceId && x.Time >= start && x.Time <= end);
            return result.ToList();
        }
    }
}

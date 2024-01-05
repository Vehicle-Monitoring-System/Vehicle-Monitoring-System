using Hust.Iot.Common;
using MongoDB.Driver;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Hust.Iot.DL
{
    public abstract class BaseDL<T> : IBaseDL<T> where T : IModel
    {
        public virtual string Collection { get; set; } = typeof(T).Name;
        protected readonly IMongoCollection<T> _collection;
        protected readonly IClientSessionHandle _session;

        protected BaseDL(IMongoDatabase database, IClientSessionHandle session)
        {
            _collection = database.GetCollection<T>(Collection);
            _session = session;
        }

        public async Task CreateRecordAsync(T record)
        {
            await _collection.InsertOneAsync(record);
            return;
        }

        public Task<int> DeleteRecordAsync(string id)
        {
            throw new NotImplementedException();
        }

        public async Task<IEnumerable<T>> GetRecordsAsync()
        {
            var result = await _collection.FindAsync<T>(_ => true);
            return result.ToList();
        }

        public async Task<T> GetRecordByIdAsync(string id)
        {
            var filter = Builders<T>.Filter.And(
                Builders<T>.Filter.Eq(Collection+"Id", id)
            );

            var records = await _collection.Find<T>(filter).ToListAsync();
            return records[0];
        }

        public Task<int> UpdateRecordAsync(string id, T record)
        {
            throw new NotImplementedException();
        }
    }
}

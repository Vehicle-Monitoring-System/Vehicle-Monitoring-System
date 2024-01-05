using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Hust.Iot.DL
{
    public interface IBaseDL<T>
    {
        public Task<T> GetRecordByIdAsync(string id);
        public Task<IEnumerable<T>> GetRecordsAsync();
        public Task CreateRecordAsync(T record);
        public Task<int> UpdateRecordAsync(string id, T record);
        public Task<int> DeleteRecordAsync(string id);
    }
}

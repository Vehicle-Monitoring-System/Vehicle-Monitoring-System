using Hust.Iot.BL.GpsBL.cs;
using Hust.Iot.Common;
using Microsoft.AspNetCore.Http;
using Microsoft.AspNetCore.Mvc;

namespace Hust.Iot.Controllers
{
    [Route("api/[controller]")]
    [ApiController]
    public class GpsController : ControllerBase
    {
        private readonly IGpsBL _gpsBL;

        public GpsController(IGpsBL gpsBL)
        {
            _gpsBL = gpsBL;
        }

        [HttpPost]
        public async Task<IActionResult> CreateRecordAsync(GPSRecord gpsRecord)
        {
            await _gpsBL.CreateRecordAsync(gpsRecord);
            return Ok(gpsRecord);
        }

        [HttpGet]
        public async Task<IActionResult> GetPairs([FromQuery] DateTime time)
        {
            var result = await _gpsBL.GetPairsAsync(time);
            return Ok(result);
        }

        [HttpGet]
        [Route("trip")]
        public async Task<IActionResult> GetTripAsync([FromQuery] DateTime start, [FromQuery] DateTime end)
        {
            var result = await _gpsBL.GetTripAsync(start, end);
            return Ok(result);
        }
    }
}

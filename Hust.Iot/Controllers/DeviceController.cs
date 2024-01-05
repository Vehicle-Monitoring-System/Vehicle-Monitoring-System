using Hust.Iot.BL;
using Hust.Iot.Common;
using Microsoft.AspNetCore.Http;
using Microsoft.AspNetCore.Mvc;

namespace Hust.Iot.Controllers
{
    [Route("api/[controller]")]
    [ApiController]
    public class DeviceController : ControllerBase
    {
        private readonly IDeviceBL _deviceBL;

        public DeviceController(IDeviceBL deviceBL)
        {
            _deviceBL = deviceBL;
        }

        [HttpGet]
        public async Task<IActionResult> GetAllAsync()
        {
            var result = await _deviceBL.GetAllAsync();
            return Ok(result);
        }

        [HttpGet]
        [Route("{id}")]
        public async Task<IActionResult> GetByIdAsync(string id)
        {
            var result = await _deviceBL.GetByIdAsync(id);
            return Ok(result);
        }
    }
}

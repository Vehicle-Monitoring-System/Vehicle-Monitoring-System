using Hust.Iot.BL;
using Microsoft.AspNetCore.Http;
using Microsoft.AspNetCore.Mvc;

namespace Hust.Iot.Controllers
{
    [Route("api/[controller]")]
    [ApiController]
    public class DriverController : ControllerBase
    {
        private readonly IDriverBL _driverBL;

        public DriverController(IDriverBL driverBL)
        {
            _driverBL = driverBL;
        }

        [HttpGet]
        public async Task<IActionResult> GetAllAsync()
        {
            var result = await _driverBL.GetAllAsync();
            return Ok(result);
        }

        [HttpGet]
        [Route("{id}")]
        public async Task<IActionResult> GetByIdAsync(string id)
        {
            var result = await _driverBL.GetByIdAsync(id);
            return Ok(result);
        }
    }
}

using Hust.Iot.BL;
using Hust.Iot.BL.GpsBL.cs;
using Hust.Iot.Common;
using Hust.Iot.DL;
using MongoDB.Driver;

var builder = WebApplication.CreateBuilder(args);

// Add services to the container.

builder.Services.AddControllers();
// Learn more about configuring Swagger/OpenAPI at https://aka.ms/aspnetcore/swashbuckle
builder.Services.AddEndpointsApiExplorer();
builder.Services.AddSwaggerGen();
var configuration = new ConfigurationBuilder()
    .SetBasePath(builder.Environment.ContentRootPath)
    .AddJsonFile("appsettings.json")
    .Build();
var connectionString = configuration["ConnectionString"];
var databaseName = configuration["DatabaseName"];
builder.Services.AddSingleton<IMongoDatabase>(provider =>
{
    var mongoClient = new MongoClient(connectionString);
    return mongoClient.GetDatabase(databaseName);
});
builder.Services.AddScoped<IClientSessionHandle>(provider =>
{
    var mongoClient = new MongoClient(connectionString);
    return mongoClient.StartSession();
});
builder.Services.AddScoped<IGpsDL, GpsDL>();
builder.Services.AddScoped<IGpsBL, GpsBL>();
builder.Services.AddScoped<IDeviceDL, DeviceDL>();
builder.Services.AddScoped<IDeviceBL, DeviceBL>();
builder.Services.AddScoped<IDriverDL, DriverDL>();
builder.Services.AddScoped<IDriverBL, DriverBL>();

builder.Services.AddSignalR();

builder.Services.AddCors(p => p.AddPolicy("MyCors", build =>
{
    build.WithOrigins("*").AllowAnyMethod().AllowAnyHeader();
}));

var app = builder.Build();

// Configure the HTTP request pipeline.
if (app.Environment.IsDevelopment())
{
    app.UseSwagger();
    app.UseSwaggerUI();
}

app.UseSwagger();
app.UseSwaggerUI();

app.UseHttpsRedirection();


app.UseCors("MyCors");
app.UseRouting();
app.UseAuthorization();

app.UseEndpoints(endpoints =>
{
    endpoints.MapControllers();
    endpoints.MapHub<GpsBL>("/current-location");
});

app.Run();

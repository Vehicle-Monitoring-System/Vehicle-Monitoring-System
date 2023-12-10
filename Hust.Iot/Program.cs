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

var app = builder.Build();

// Configure the HTTP request pipeline.
if (app.Environment.IsDevelopment())
{
    app.UseSwagger();
    app.UseSwaggerUI();
}

app.UseHttpsRedirection();

app.UseAuthorization();

app.MapControllers();

app.Run();

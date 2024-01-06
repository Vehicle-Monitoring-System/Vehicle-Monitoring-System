using HiveMQtt.Client;
using HiveMQtt.Client.Options;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Text.Json;
using System.Threading.Tasks;

namespace Hust.Iot.BL
{
    public class HiveMQ : IHiveMQ
    {
        public readonly HiveMQClient client;

        public HiveMQ()
        {
            var options = new HiveMQClientOptions
            {
                Host = "fca1150a13fd455eb1d37dadd06e203e.s2.eu.hivemq.cloud",
                Port = 8883,
                UseTLS = true,
                UserName = "marvelboy",
                Password = "Qqqqqqq1",
            };

            client = new HiveMQClient(options);
            client.ConnectAsync().Wait();
        }

        public async void PublishTopic(string id, object data)
        {
            var msg = JsonSerializer.Serialize(data);
            var result = await client.PublishAsync($"device/{id}", msg);
        }
    }
}

document.getElementById('city-form').addEventListener('submit', function(event) {
    event.preventDefault();

    const citySelect = document.getElementById('city');
    const selectedCity = citySelect.options[citySelect.selectedIndex].text;
    let valueToSend = 0;
    let averageTemp = "";

    // Set the value and message based on the selected city
    if (selectedCity === 'Timisoara') {
        valueToSend = 12;
        averageTemp = "The Average Temperature in Timisoara in the last 15 years was 12.75 degrees Celsius -- computed from the NASA internal weather database.";
    } else if (selectedCity === 'Cluj-Napoca') {
        valueToSend = 10;
        averageTemp = "The Average Temperature in Cluj-Napoca in the last 15 years was 10.35 degrees Celsius -- computed from the NASA internal weather database.";
    }

    // Display the selected city
    document.getElementById('selected-city').textContent = `You have selected: ${selectedCity}`;

    // Send the value to ThingSpeak
    sendToThingSpeak(valueToSend);

    // Fetch ThingSpeak data and open the new page with a graph
    fetchThingSpeakDataAndOpenPage(averageTemp);
});

// Function to send the data to ThingSpeak
function sendToThingSpeak(value) {
    const apiKey = 'V9UVII7WQOFOUNJ6'; // Replace with your ThingSpeak Write API Key
    const fieldNumber = 3; // Replace with the field number you're sending the data to

    const url = `https://api.thingspeak.com/update?api_key=${apiKey}&field${fieldNumber}=${value}`;

    fetch(url)
        .then(response => {
            if (response.ok) {
                console.log('Data sent successfully');
            } else {
                console.log('Failed to send data');
            }
        })
        .catch(error => {
            console.error('Error sending data:', error);
        });
}

// Fetch ThingSpeak data and open a new page with a graph
function fetchThingSpeakDataAndOpenPage(averageTemp) {
    const channelId = '2683325'; // Replace with your ThingSpeak Channel ID
    const readApiKey = '0LP4TI8QBZFM9ZQ4'; // Replace with your ThingSpeak Read API Key

    const url = `https://api.thingspeak.com/channels/${channelId}/feeds.json?api_key=${readApiKey}&results=20`;

    fetch(url)
        .then(response => response.json())
        .then(data => {
            const feeds = data.feeds;
            const sensorData = {
                temperature: feeds.map(feed => feed.field1),
                humidity: feeds.map(feed => feed.field2),
                luminosity: feeds.map(feed => feed.field3),
                soilMoisture: feeds.map(feed => feed.field4),
                waterLevel: feeds.map(feed => feed.field5),
                time: feeds.map(feed => feed.created_at)
            };

            openNewPageWithChart(averageTemp, sensorData);
        })
        .catch(error => {
            console.error('Error fetching data from ThingSpeak:', error);
        });
}

// Open a new page with the chart and the message
function openNewPageWithChart(averageTemp, sensorData) {
    const newWindow = window.open("", "_blank");
    newWindow.document.write(`
<html lang="en">
<head>
  <meta charset="UTF-8">
  <meta name="viewport" content="width=device-width, initial-scale=1.0">
  <title>Sensor Data Visualization</title>
  <style>
    body {
      font-family: Arial, sans-serif;
      display: flex;
      justify-content: center;
      align-items: center;
      flex-direction: column;
      height: 100vh;
      margin: 0;
      background-color: #f4f4f4;
    }
    .content {
      text-align: center;
      background-color: white;
      padding: 20px;
      border-radius: 10px;
      box-shadow: 0 2px 10px rgba(0, 0, 0, 0.1);
    }
    canvas {
      max-width: 100%; /* Adjust the width to make the graph bigger */
      height: 600px;  /* Adjust the height to make the graph bigger */
    }
    h2 {
      font-size: 24px;
      margin-bottom: 20px;
    }
    p {
      font-size: 18px;
      margin-top: 20px;
      font-weight: bold;
    }
  </style>
  <script src="https://cdn.jsdelivr.net/npm/chart.js"></script> <!-- Include Chart.js -->
</head>
<body>
  <div class="content">
    <p>${averageTemp}</p>
    <h2>Sensor Data Visualization</h2>
    <canvas id="sensorChart"></canvas>
  </div>

  <script>
    function updateChart(time, temperature, humidity, luminosity, soilMoisture, waterLevel) {
      const ctx = document.getElementById('sensorChart').getContext('2d');
      const sensorChart = new Chart(ctx, {
        type: 'line',
        data: {
          labels: time,
          datasets: [
            { label: 'Temperature', data: temperature, borderColor: 'red', fill: false },
            { label: 'Humidity', data: humidity, borderColor: 'blue', fill: false },
            { label: 'Luminosity', data: luminosity, borderColor: 'yellow', fill: false },
            { label: 'Soil Moisture', data: soilMoisture, borderColor: 'green', fill: false },
            { label: 'Water Level', data: waterLevel, borderColor: 'purple', fill: false }
          ]
        },
        options: {
          responsive: true,
          scales: {
            x: { display: true, title: { display: true, text: 'Time' } },
            y: { display: true, title: { display: true, text: 'Value' } }
          }
        }
      });
    }

    const sensorData = ${JSON.stringify(sensorData)};
    updateChart(sensorData.time, sensorData.temperature, sensorData.humidity, sensorData.luminosity, sensorData.soilMoisture, sensorData.waterLevel);
  </script>
</body>
</html>
    `);
}

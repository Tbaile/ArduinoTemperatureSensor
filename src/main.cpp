#include <Arduino.h>
#include <Arduino_FreeRTOS.h>
#include <DHT_U.h>

#define DHTPIN 2
#define DHTTYPE DHT11

void TaskReadSensor(void *pvParameters);

void setup() {
  xTaskCreate(
    TaskReadSensor,
    "DHT Control",
    544,
    NULL,
    2,
    NULL
  );
}

void loop() {
  // FreeRTOS taking care of everything
}

void TaskReadSensor(void *pvParameters __attribute__((unused))) {
  // Set up DHT sensor
  DHT_Unified dht(DHTPIN, DHTTYPE);
  dht.begin();
  // Read sensor and set up tick accordingly
  sensor_t sensor;
  dht.temperature().getSensor(&sensor);
  // Open serial to print data
  Serial.begin(9600);
  for(;;) {
    vTaskDelay(1000 / portTICK_PERIOD_MS);
    sensors_event_t event;
    // Get temperature and print it's value
    dht.temperature().getEvent(&event);
    if (isnan(event.temperature)) {
      Serial.println(F("Error reading temperature!"));
    }
    else {
      Serial.print(F("Temperature: "));
      Serial.print(event.temperature);
      Serial.println(F("°C"));
    }
    // Get humidity event and print its value
    dht.humidity().getEvent(&event);
    if (isnan(event.relative_humidity)) {
      Serial.println(F("Error reading humidity!"));
    }
    else {
      Serial.print(F("Humidity: "));
      Serial.print(event.relative_humidity);
      Serial.println(F("%"));
    }
  }
}
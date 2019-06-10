
#define PUB_MSG_LEN 16

#include <stdio.h>
#include <stdlib.h>
#include "espressif/esp_common.h"
#include "esp/uart.h"
#include "FreeRTOS.h"
#include "task.h"
#include <dht/dht.h>
#include "esp8266.h"

#include <semphr.h>


uint8_t const dht_gpio = 14;
const dht_sensor_type_t sensor_type = DHT_TYPE_DHT22;

extern QueueHandle_t publish_queue;

void dhtMeasurementTask(void *pvParameters)
{
    int16_t temperature = 0;
    int16_t humidity = 0;
    int converted_temperature = 0;
    int converted_humidity = 0;

    char msg[PUB_MSG_LEN];
    int count = 0;

    // DHT sensors that come mounted on a PCB generally have
    // pull-up resistors on the data pin.  It is recommended
    // to provide an external pull-up resistor otherwise...
    gpio_set_pullup(dht_gpio, false, false);

    while(1) {
        if (dht_read_data(sensor_type, dht_gpio, &humidity, &temperature)) {
            converted_temperature = temperature / 10;
            converted_humidity = humidity / 10;

            printf("%s:Humidity: %d%% Temp: %dC\n", __func__,
                    humidity / 10,
                    temperature / 10);
        } else {
            printf("Could not read data from sensor\n");
        }

        snprintf(msg, PUB_MSG_LEN, "t:%d,h:%d\r\n", converted_temperature, converted_humidity);
        if (xQueueSend(publish_queue, (void *)msg, 0) == pdFALSE) {
            printf("Publish queue overflow.\r\n");
        }

        // Three second delay...
        vTaskDelay(3000 / portTICK_PERIOD_MS);
    }
}

void init_dht(void)
{
    xTaskCreate(dhtMeasurementTask, "dhtMeasurementTask", 256, NULL, 2, NULL);
}

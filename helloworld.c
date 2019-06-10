/* The classic "blink" example
 *
 * This sample code is in the public domain.
 */

#define PUB_MSG_LEN 16

#define MQTT_HOST ("test.mosquitto.org")
#define MQTT_PORT 1883

#define MQTT_USER NULL
#define MQTT_PASS NULL


#include <stdlib.h>


#include "espressif/esp_common.h"
#include "esp/uart.h"
#include "FreeRTOS.h"
#include "task.h"
#include "esp8266.h"

#include "blink_task.c"
#include "dht_task.c"
#include "wifi_task.c"
#include "mqtt.c"

#include <semphr.h>

SemaphoreHandle_t wifi_alive;
QueueHandle_t publish_queue;

void user_init(void)
{
    uart_set_baud(0, 115200);
    vSemaphoreCreateBinary(wifi_alive);
    publish_queue = xQueueCreate(3, PUB_MSG_LEN);

    printf("init tasks\n");
    xTaskCreate(blink_task, "blink_task", 256, NULL, 2, NULL);

    xTaskCreate(&wifi_task, "wifi_task",  256, NULL, 2, NULL);

    xTaskCreate(dhtMeasurementTask, "dhtMeasurementTask", 256, NULL, 2, NULL);

    xTaskCreate(&mqtt_task, "mqtt_task", 1024, NULL, 4, NULL);
}

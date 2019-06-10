#include <stdlib.h>

#include "espressif/esp_common.h"
#include "esp/uart.h"
#include "FreeRTOS.h"
#include "task.h"
#include "esp8266.h"

const int gpio = 2;

void blink_task(void *pvParameters)
{
    gpio_enable(gpio, GPIO_OUTPUT);
    while(1) {
        gpio_write(gpio, 1);
        vTaskDelay(500 / portTICK_PERIOD_MS);
        gpio_write(gpio, 0);
        vTaskDelay(500 / portTICK_PERIOD_MS);
    }
}

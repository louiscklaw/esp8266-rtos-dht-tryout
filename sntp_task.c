/*
 * Test code for SNTP on esp-open-rtos.
 *
 * Jesus Alonso (doragasu)
 */
#include <espressif/esp_common.h>
#include <esp/uart.h>

#include <string.h>
#include <stdio.h>

#include <FreeRTOS.h>
#include <task.h>

#include <lwip/err.h>
#include <lwip/sockets.h>
#include <lwip/sys.h>
#include <lwip/netdb.h>
#include <lwip/dns.h>

#include <ssid_config.h>

/* Add extras/sntp component to makefile for this include to work */
#include <sntp.h>
#include <time.h>

#define SNTP_SERVERS 	"stdtime.gov.hk"

#define vTaskDelayMs(ms)	vTaskDelay((ms)/portTICK_PERIOD_MS)
#define UNUSED_ARG(x)	(void)x

extern SemaphoreHandle_t wifi_alive;

extern uint8_t wifi_status;

void sntp_tsk(void *pvParameters)
{
	const char *servers[] = {SNTP_SERVERS};
	UNUSED_ARG(pvParameters);

	/* Start SNTP */
	printf("Starting SNTP... \r\n");
	/* SNTP will request an update each 5 minutes */
	sntp_set_update_delay(30*60000);
	/* Set GMT+1 zone, daylight savings off */
	const struct timezone tz = {8*60, 0};
	/* SNTP initialization */
	sntp_initialize(&tz);
	/* Servers must be configured right after initialization */
	sntp_set_servers(servers, sizeof(servers) / sizeof(char*));
	printf("DONE!\n");

	/* Print date and time each 5 seconds */
	while(1) {
		vTaskDelayMs(1000);
		time_t ts = time(NULL);
		printf("TIME: %s", ctime(&ts));
	}


}

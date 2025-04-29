#include <stdio.h>
#include "pico/stdlib.h"
#include "pico/cyw43_arch.h"
#include "lcd.h"
#include "wifi.h"
#include "buzzer.h"
#include "ultrasonic.h"
#include "datetime.h"
#include <math.h>
#include "led.h"
#include "lwip/apps/mqtt.h"
#include "lwip/ip_addr.h"
#include "lwip/err.h"

#define WIFI_SSID "XXXX"
#define WIFI_PASSWORD "XXXX"

mqtt_client_t *mqtt_client;
volatile bool mqtt_connected = false;

static const struct mqtt_connect_client_info_t mqtt_client_info = {
    .client_id   = "pico_client",
    .client_user = NULL,
    .client_pass = NULL,
    .keep_alive  = 60,
    .will_topic  = NULL,
    .will_msg    = NULL,
    .will_qos    = 0,
    .will_retain = 0
};
static void mqtt_connection_cb(mqtt_client_t *client, void *arg, mqtt_connection_status_t status) {
    if (status == MQTT_CONNECT_ACCEPTED) {
        printf("MQTT connected.\n");
        mqtt_connected = true;
    } else {
        printf("MQTT connection failed with status %d\n", status);
        mqtt_connected = false;
    }
}

static void mqtt_publish_cb(void *arg, err_t result) {
    if (result == ERR_OK) {
        printf("MQTT publish successful.\n");
    } else {
        printf("MQTT publish failed: %d\n", result);
    }
}


int main(){
	

    stdio_init_all();

    lcd_init();
    lcd_clear();
    buzzer_init();

    lcd_set_cursor(0, 0);
    lcd_print("Ansluter till");
    lcd_set_cursor(0, 1);
    lcd_print("WiFi....");

    if (connect_to_wifi(WIFI_SSID, WIFI_PASSWORD)){
	    lcd_clear();
	    lcd_set_cursor(0, 0);
	    lcd_print("Ansluten till");
	    lcd_set_cursor(0, 1);
	    lcd_print("WiFi!");
	    sleep_ms(2000);
	    const ip_addr_t *ip=netif_ip_addr4(netif_default);
	    printf("Min IP-adress: %s\n", ipaddr_ntoa(ip));
    } else{
	    lcd_clear();
	    lcd_set_cursor(0, 0);
	    lcd_print("WiFi FEL!");
	    sleep_ms(2000);
    }

    ip_addr_t broker_ip;
   ipaddr_aton("XXX.XXX.X.XXX", &broker_ip);
   printf("Forsoker ansluta till mqtt....");

mqtt_client = mqtt_client_new();
if (mqtt_client != NULL) {
    mqtt_client_connect(mqtt_client, &broker_ip, 1883, mqtt_connection_cb, NULL, &mqtt_client_info);
    printf("Försöker ansluta till MQTT...\n");
} else {
    printf("Kunde inte skapa MQTT-klient\n");
}



    ultrasonic_init(17, 16);
    datetime_init();
    led_init();


    while(true){
	    double distance = ultrasonic_read_cm();
	    if (mqtt_connected) {
		    char payload[32];
		    snprintf(payload, sizeof(payload), "%.2f", distance);
		    mqtt_publish(mqtt_client, "sensor/distance", payload, strlen(payload), 0, 0, mqtt_publish_cb, NULL);
}

	    char buffer[32] = {0};
	    struct tm *now = datetime_get_time();
	    lcd_clear(); 
	    lcd_set_cursor(0, 0);
	    snprintf(buffer, sizeof(buffer), "Avstand: %1f cm", distance);
	    lcd_print(buffer);


	    if (now){
		    char time_buf[32] = {0};
		    snprintf(time_buf, sizeof(time_buf), "%02d/%02d/%04d %02d:%02d:%02d",
				   now->tm_mday, now->tm_mon +1, now->tm_year + 1900,
				   now->tm_hour, now->tm_min, now->tm_sec);
		    lcd_set_cursor(0, 1);
		    lcd_print(time_buf);
	    } else {
		    lcd_set_cursor(0, 1);
		    lcd_print("Ingen tid");
	    }

	    if (distance < 100){
		    buzzer_beep(200);
		    led_on();
	    } else{
		    led_off();}
	   
	    sleep_ms(1000);
    }


    return 0;
}


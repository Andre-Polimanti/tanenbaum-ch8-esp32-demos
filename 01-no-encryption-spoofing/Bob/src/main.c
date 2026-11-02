#include "wifi/wifi.h"
#include "led/led.h"
#include "espnow_rx/espnow_rx.h"

void app_main(void)
{
    led_mysetup();
    wifi_mysetup();
    espnow_rx_mysetup();
}
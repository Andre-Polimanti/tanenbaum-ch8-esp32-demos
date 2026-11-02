#include "wifi/wifi.h"
#include "led/led.h"
#include "espnow_tx/espnow_tx.h"

void app_main(void)
{
    led_mysetup();
    wifi_mysetup();
    espnow_tx_mysetup();
}
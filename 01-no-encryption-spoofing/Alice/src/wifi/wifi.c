#include "wifi.h"

#include "esp_event.h"
#include "esp_netif.h"
#include "esp_wifi.h"
#include "nvs_flash.h"

void nvs_flash_mysetup() {
    esp_err_t ret = nvs_flash_init();
    if (ret == ESP_ERR_NVS_NO_FREE_PAGES || ret == ESP_ERR_NVS_NEW_VERSION_FOUND) {
        nvs_flash_erase();
        nvs_flash_init();
    }
}

void wifi_mysetup() {
    nvs_flash_mysetup(); // WiFi usa de dados gravados na flash
    esp_netif_init(); // WiFi precisa da ponte entre hardware de rede e software TCP/IP pra funcionar
    
    esp_event_loop_create_default();

    wifi_init_config_t cnf = WIFI_INIT_CONFIG_DEFAULT();
    esp_wifi_init(&cnf);

    esp_wifi_set_storage(WIFI_STORAGE_RAM);
    esp_wifi_set_mode(WIFI_MODE_STA);
    esp_wifi_start();
}
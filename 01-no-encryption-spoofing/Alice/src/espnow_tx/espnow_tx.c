#include "espnow_tx.h"

#include <string.h>

#include "esp_now.h"
#include "esp_timer.h"
#include "esp_wifi.h"

#include "led/led.h"

#define SEND_PERIOD_US (1000 * 1000)
#define LED_ON_SUCCESS_MS 50
#define LED_ON_FAILURE_MS 200

static const uint8_t peer_mac[6] = {0x8C, 0x94, 0xDF, 0x4C, 0x77, 0x00}; // 8c:94:df:4c:77:00
static const char *msg = "Hallooooo!";

static esp_timer_handle_t send_timer;

static void on_send(const esp_now_send_info_t *tx_info, esp_now_send_status_t status) {
    led_pulse(status == ESP_NOW_SEND_SUCCESS ? LED_ON_SUCCESS_MS : LED_ON_FAILURE_MS);
}

static void send_timer_cb(void *arg) {
    esp_now_send(peer_mac, (const uint8_t *)msg, strlen(msg));
}

static void add_mypeer (void) {
    esp_now_peer_info_t peer_info;

    memset(&peer_info, 0, sizeof(peer_info));
    memcpy(peer_info.peer_addr, peer_mac, 6);
    peer_info.channel = 0;
    peer_info.ifidx = WIFI_IF_STA;
    peer_info.encrypt = false;

    esp_now_add_peer(&peer_info);
}

void espnow_tx_mysetup(void) {
    esp_now_init();
    esp_now_register_send_cb(on_send); // Registra o callback chamado a cada tentativa de envio de mensagem
    add_mypeer();

    const esp_timer_create_args_t send_args = {
        .callback = send_timer_cb,
        .name = "send"
    };

    esp_timer_create(&send_args, &send_timer); // Cria o timer de envio, associando o send_timer_cb
    esp_timer_start_periodic(send_timer, SEND_PERIOD_US); // Inicia o timer de envio, disparando periodicamente (tarefa cíclica)
}
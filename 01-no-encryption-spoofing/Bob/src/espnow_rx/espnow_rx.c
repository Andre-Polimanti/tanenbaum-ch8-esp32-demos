#include "espnow_rx.h"

#include <stdio.h>
#include <string.h>

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"

#include "esp_now.h"

#include "led/led.h"

#define LED_ON_MS 1000

typedef struct { // Pacote de dados recebido
    uint8_t src[6]; // Endereço MAC da origem da mensagem
    uint8_t data[ESP_NOW_MAX_DATA_LEN]; // Mensagem recebida
    int len; // Tamanho
} rx_msg_t;

static QueueHandle_t rx_queue; // Fila do FreeRTOS

static void on_receive(const esp_now_recv_info_t *info, const uint8_t *data, int data_len) {
    led_pulse(LED_ON_MS);

    rx_msg_t m;
    memcpy(m.src, info->src_addr, 6);
    m.len = (data_len > ESP_NOW_MAX_DATA_LEN) ? ESP_NOW_MAX_DATA_LEN : data_len; // Checa por buffer overflow e limita o tamanho do buffer antes de copiar a informação
    memcpy(m.data, data, m.len);

    xQueueSend(rx_queue, &m, 0); // Envia o pacote de informações copiado para a fila do FreeRTOS
}

static void print_task(void *arg) {
    rx_msg_t m;

    // Espera por pacotes na fila, sem bloquear CPU e imprime cada um
    while (true) {
        if (xQueueReceive(rx_queue, &m, portMAX_DELAY) == pdTRUE) {
            printf("Alice [%02X:%02X:%02X:%02X:%02X:%02X] enviou: %.*s\n",
                m.src[0], m.src[1], m.src[2], m.src[3], m.src[4], m.src[5],
                m.len, m.data);
        printf("Stack livre (mínimo): %u bytes\n",
            (unsigned)uxTaskGetStackHighWaterMark(NULL));
        }
    }
}

void espnow_rx_mysetup(void) {
    rx_queue = xQueueCreate(8, sizeof(rx_msg_t));
    xTaskCreate(print_task, "print", 3072, NULL, 4, NULL);

    esp_now_init();
    esp_now_register_recv_cb(on_receive); // Registra o callback chamado a cada mensagem recebida
}
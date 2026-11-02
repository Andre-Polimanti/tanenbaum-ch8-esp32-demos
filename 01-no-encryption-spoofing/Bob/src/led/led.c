#include "led.h"

#include "driver/gpio.h"
#include "esp_timer.h"

#define BUILTIN_LED GPIO_NUM_2

static esp_timer_handle_t led_off_timer; // Timer para desligar o led, iniciado a partir do momento que o mesmo é ligado

static void led_off_cb(void *arg) { // Função callback que desliga o led, chamada no expirar do timer
    gpio_set_level(BUILTIN_LED, 0);
}

void led_mysetup(void) {
    gpio_reset_pin(BUILTIN_LED); // Limpa lixo do pino
    gpio_set_direction(BUILTIN_LED, GPIO_MODE_OUTPUT); // Definindo o GPIO como output, não haverá input por meio desse pino

    const esp_timer_create_args_t off_args = { // Registrando os argumentos do timer de desligamento: Callback e Nome
        .callback = led_off_cb,
        .name = "led_off"
    };
    esp_timer_create(&off_args, &led_off_timer); // Cria o timer e guarda o handle do mesmo em led_off_timer
}

void led_pulse(uint32_t duration_ms) {
    gpio_set_level(BUILTIN_LED, 1);
    esp_timer_stop(led_off_timer); // Caso já tivesse um timer ativo, ele é desarmado. Se isso ocorrer, o pulso é estendido, pois na próxima linha o timer é rearmado novamente.

    esp_timer_start_once(led_off_timer, (uint64_t)duration_ms * 1000ULL); // Timer armado
}
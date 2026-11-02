#pragma once

#include <stdint.h>

void led_mysetup(void); // Definição e configuração do GPIO
void led_pulse(uint32_t duration_ms); // Acende o LED por determinado intervalo de tempo
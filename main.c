/*
 * main.c
 * Налаштування генерації випадкових імпульсів на GP2.
 */

#include <xc.h>
#include <stdint.h>

#define _XTAL_FREQ 4000000UL

#pragma config OSC = IntRC      // внутрішній генератор IntRC
#pragma config WDTE = OFF        // додаємо сторожовий таймер для перезапуску, якщо зависає високий рівень
#pragma config MCLRE = OFF      // GP3 працює як GPIO
#pragma config CP = OFF         // відключаємо захист коду

/* Простий LFSR для псевдовипадкових значень */
static uint8_t lfsr_state = 0xA5;

static uint8_t lfsr_next(void) {
    uint8_t feedback = (lfsr_state ^ (lfsr_state >> 2)) & 0x01;
    lfsr_state = (uint8_t)((lfsr_state >> 1) | (feedback << 7));
    return lfsr_state;
}

static void delay_ms(uint16_t ms) {
    while (ms--) {
        __delay_ms(1);
        CLRWDT(); // скидаємо сторожовий таймер під час довгих пауз
    }
}

static void init_ports(void) {
    OPTION = 0b00000000; 
    GPIO = 0;          // початковий стан на всіх лініях — 0
    TRIS = 0b1011;     // GP2 вихід, інші — входи
}

static uint16_t cycle_high_ms;
static uint16_t cycle_low_ms;

int main(void) {
    init_ports();

    for (;;) {

        uint8_t span = lfsr_next() & 0x07;
        cycle_high_ms = 1000u + (uint16_t)span * 250u;
        if (cycle_high_ms > 3000u) {
            cycle_high_ms = 3000u;
        }

        cycle_low_ms = 4000u - cycle_high_ms;
        GPIObits.GP2 = 1;
        delay_ms(cycle_low_ms);
        GPIObits.GP2 = 0;
        delay_ms(cycle_low_ms);
    }
}

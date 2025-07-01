/* Atualizado para o protocolo do nobreak SMS Gamer */

#include <stdint.h>
#include <string.h>
#include "sms_ser.h"

#define ENDCHAR '\r'

static uint8_t checksum(uint8_t cmd, uint8_t p1, uint8_t p2, uint8_t p3, uint8_t p4) {
    return (uint8_t)((-1 * (cmd + p1 + p2 + p3 + p4)) & 0xFF);
}

#define PREP_CMD(NAME, CMD, P1, P2, P3, P4) \
uint8_t sms_prepare_##NAME(uint8_t *buffer) { \
    buffer[0] = CMD; \
    buffer[1] = P1; \
    buffer[2] = P2; \
    buffer[3] = P3; \
    buffer[4] = P4; \
    buffer[5] = checksum(buffer[0], buffer[1], buffer[2], buffer[3], buffer[4]); \
    buffer[6] = ENDCHAR; \
    return 7; \
}

uint8_t sms_prepare_test_battery_nsec(uint8_t *buffer, uint16_t delay) {
    buffer[0] = 0x54; // 'T'
    buffer[1] = (delay >> 8) & 0xFF;
    buffer[2] = delay & 0xFF;
    buffer[3] = 0x00;
    buffer[4] = 0x00;
    buffer[5] = checksum(buffer[0], buffer[1], buffer[2], buffer[3], buffer[4]);
    buffer[6] = ENDCHAR;
    return 7;
}

uint8_t sms_prepare_shutdown_nsec(uint8_t *buffer, uint16_t delay) {
    buffer[0] = 0x52; // 'R' (shutdown + restore)
    buffer[1] = (delay >> 8) & 0xFF;
    buffer[2] = delay & 0xFF;
    buffer[3] = 0x27;
    buffer[4] = 0x0F;
    buffer[5] = checksum(buffer[0], buffer[1], buffer[2], buffer[3], buffer[4]);
    buffer[6] = ENDCHAR;
    return 7;
}

// Comandos padronizados
PREP_CMD(get_status,  0x51, 0xFF, 0xFF, 0xFF, 0xFF) // 'Q'
PREP_CMD(get_information, 0x49, 0xFF, 0xFF, 0xFF, 0xFF) // 'I'
PREP_CMD(get_features,   0x46, 0xFF, 0xFF, 0xFF, 0xFF) // 'F'
PREP_CMD(set_beep,       0x4D, 0xFF, 0xFF, 0xFF, 0xFF) // 'M'
PREP_CMD(cancel_test,    0x43, 0xFF, 0xFF, 0xFF, 0xFF) // 'C'
PREP_CMD(cancel_shutdown,0x43, 0xFF, 0xFF, 0xFF, 0xFF) // 'C' (mesmo do cancel_test)
PREP_CMD(test_battery_low,0x54, 0x00, 0x10, 0x00, 0x00) // T com 16s fixos
PREP_CMD(shutdown_restore,0x52, 0x00, 0xC8, 0x27, 0x0F) // 10s padrão

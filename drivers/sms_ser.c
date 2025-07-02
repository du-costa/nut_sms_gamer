/* Atualizado para o protocolo real do nobreak SMS Gamer */

#include <stdint.h>
#include <string.h>
#include "sms_ser.h"
#include "serial.h"
#include "common.h"
#include "main.h"

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
    buffer[0] = 0x52; // 'R'
    buffer[1] = (delay >> 8) & 0xFF;
    buffer[2] = delay & 0xFF;
    buffer[3] = 0x27;
    buffer[4] = 0x0F;
    buffer[5] = checksum(buffer[0], buffer[1], buffer[2], buffer[3], buffer[4]);
    buffer[6] = ENDCHAR;
    return 7;
}

// Comandos padronizados
PREP_CMD(get_status,        0x51, 0xFF, 0xFF, 0xFF, 0xFF) // 'Q'
PREP_CMD(get_information,   0x49, 0xFF, 0xFF, 0xFF, 0xFF) // 'I'
PREP_CMD(get_features,      0x46, 0xFF, 0xFF, 0xFF, 0xFF) // 'F'
PREP_CMD(set_beep,          0x4D, 0xFF, 0xFF, 0xFF, 0xFF) // 'M'
PREP_CMD(cancel_test,       0x43, 0xFF, 0xFF, 0xFF, 0xFF) // 'C'
PREP_CMD(cancel_shutdown,   0x43, 0xFF, 0xFF, 0xFF, 0xFF) // 'C' (mesmo comando)
PREP_CMD(test_battery_low,  0x54, 0x00, 0x10, 0x00, 0x00) // 16s
PREP_CMD(shutdown_restore,  0x52, 0x00, 0xC8, 0x27, 0x0F) // padrão

void sms_parse_results(uint8_t *rawvalues, SmsData *results) {
    uint8_t flags;

    if (!rawvalues || rawvalues[0] != 0x51) return; // comando 'Q'

    results->lastinputVac = (float)((rawvalues[1] << 24) | (rawvalues[2] << 16) | (rawvalues[3] << 8) | rawvalues[4]) / 10.0f;
    results->inputVac     = (float)((rawvalues[5] << 8) | rawvalues[6]) / 10.0f;
    results->outputVac    = (float)((rawvalues[7] << 8) | rawvalues[8]) / 10.0f;
    results->outputpower  = (float)((rawvalues[9] << 8) | rawvalues[10]) / 10.0f;
    results->outputHz     = (float)((rawvalues[11] << 8) | rawvalues[12]) / 10.0f;
    results->batterylevel = (float)((rawvalues[13] << 8) | rawvalues[14]) / 10.0f;
    results->temperatureC = (float)((rawvalues[15] << 8) | rawvalues[16]) / 10.0f;

    flags = rawvalues[17];
    results->onbattery     = (flags & (1 << 7)) != 0;
    results->lowbattery    = (flags & (1 << 6)) != 0;
    results->bypass        = (flags & (1 << 5)) != 0;
    results->boost         = (flags & (1 << 4)) != 0;
    results->upsok         = (flags & (1 << 3)) != 0;
    results->test          = (flags & (1 << 2)) != 0;
    results->shutdown      = (flags & (1 << 1)) != 0;
    results->beepon        = (flags & (1 << 0)) != 0;
} 
extern int upsfd;
extern char device_path[];

void upsdrv_cleanup(void) {
    if (upsfd >= 0) {
        ser_close(upsfd, device_path);
        upsdebugx(LOG_DEBUG, "upsdrv_cleanup: porta serial fechada");
    } else {
        upsdebugx(LOG_DEBUG, "upsdrv_cleanup: upsfd inválido");
    }
}
void upsdrv_help(void) {}
void upsdrv_makevartable(void) {}

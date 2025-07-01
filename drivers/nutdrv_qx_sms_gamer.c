/* nutdrv_qx_sms_gamer.c - Subdriver for SMS Gamer UPS (protocol binário)
 *
 *
 * Released under MIT License
 */

#include "main.h"
#include "nutdrv_qx.h"
#include "nutdrv_qx_blazer-common.h"

#define SMSGAMER_VERSION "SMS Gamer 0.01"

/* Mapeamento das variáveis do comando 'Q' (resposta binária, 17 bytes + \r)
 * Exemplo: 0x51 ... [vin 4B] [vout 2B] [load 2B] [freq 2B] [batt 2B] [temp 2B] [flags 1B] \r
 */
static item_t sms_gamer_qx2nut[] = {
    { "input.voltage",     0, NULL, "Q", "", 17, 0x51, "",  1,  5, "%.1f", 0, NULL, NULL, qx_decode_u32div10 },
    { "output.voltage",    0, NULL, "Q", "", 17, 0x51, "",  5,  7, "%.1f", 0, NULL, NULL, qx_decode_u16div10 },
    { "ups.load",          0, NULL, "Q", "", 17, 0x51, "",  7,  9, "%.1f", 0, NULL, NULL, qx_decode_u16div10 },
    { "input.frequency",   0, NULL, "Q", "", 17, 0x51, "",  9, 11, "%.1f", 0, NULL, NULL, qx_decode_u16div10 },
    { "battery.charge",    0, NULL, "Q", "", 17, 0x51, "", 11, 13, "%.1f", 0, NULL, NULL, qx_decode_u16div10 },
    { "ups.temperature",   0, NULL, "Q", "", 17, 0x51, "", 13, 15, "%.1f", 0, NULL, NULL, qx_decode_u16div10 },
    { "ups.status",        0, NULL, "Q", "", 17, 0x51, "", 15, 15, NULL, QX_FLAG_QUICK_POLL, NULL, NULL, blazer_process_status_bits },

    { "beeper.toggle",         0, NULL, "M", "", 0, 0, "", 0, 0, NULL, QX_FLAG_CMD, NULL, NULL, NULL },
    { "test.battery.start",    0, NULL, "T00\x10\x00\x00", "", 0, 0, "", 0, 0, NULL, QX_FLAG_CMD, NULL, NULL, NULL },
    { "test.battery.start.deep", 0, NULL, "T01\x2C\x00\x00", "", 0, 0, "", 0, 0, NULL, QX_FLAG_CMD, NULL, NULL, NULL },
    { "test.battery.start.custom", 0, NULL, "T%02d\x00\x00\x00", "", 0, 0, "", 0, 0, NULL, QX_FLAG_CMD, NULL, NULL, blazer_process_command },
    { "test.battery.stop",     0, NULL, "C", "", 0, 0, "", 0, 0, NULL, QX_FLAG_CMD, NULL, NULL, NULL },
    { "shutdown.return",       0, NULL, "R", "", 0, 0, "", 0, 0, NULL, QX_FLAG_CMD, NULL, NULL, NULL },
    { "shutdown.stayoff",      0, NULL, "S", "", 0, 0, "", 0, 0, NULL, QX_FLAG_CMD, NULL, NULL, NULL },
    { "shutdown.stop",         0, NULL, "C", "", 0, 0, "", 0, 0, NULL, QX_FLAG_CMD, NULL, NULL, NULL },
    { "battery.discharge",     0, NULL, "D", "", 0, 0, "", 0, 0, NULL, QX_FLAG_CMD, NULL, NULL, NULL },

    { NULL, 0, NULL, NULL, "", 0, 0, "", 0, 0, NULL, 0, NULL, NULL, NULL }
};

#ifdef TESTING
static testing_t sms_gamer_testing[] = {
    { "Q", "\x51\x00...<resposta binária>", -1 },
    { NULL }
};
#endif

static void sms_gamer_initups(void) {
    blazer_initups(sms_gamer_qx2nut);
}

subdriver_t sms_gamer_subdriver = {
    SMSGAMER_VERSION,
    blazer_claim,
    sms_gamer_qx2nut,
    sms_gamer_initups,
    NULL,
    blazer_makevartable,
    "\xAA", // byte de resposta esperada opcional (pode ser ignorado)
    NULL,
#ifdef TESTING
    sms_gamer_testing,
#endif
};

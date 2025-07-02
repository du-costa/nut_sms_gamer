#ifndef NUT_SMS_SER_H_SEEN
#define NUT_SMS_SER_H_SEEN 1

#include <stdbool.h>
#include "nut_stdint.h"

#define DEFAULT_BOOTDELAY 64 /* segundos (máximo 0xFFFF com protocolo real) */
#define MAXTRIES 3

typedef struct {
    char model[25];        /* ups.model */
    char version[7];       /* ups.firmware */
    char voltageRange[15]; /* legado: pode ser ignorado */
    char currentRange[7];  /* legado: pode ser ignorado */
    uint8_t voltageBattery;
    uint8_t frequency;

    bool beepon;
    bool shutdown;
    bool test;
    bool upsok;
    bool boost;
    bool bypass;
    bool lowbattery;
    bool onbattery;

    float lastinputVac;   /* input.voltage (4 bytes - vin) */
    float inputVac;       /* input.voltage */
    float outputVac;      /* output.voltage */
    float outputpower;    /* ups.load */
    float outputHz;       /* output.frequency */
    float batterylevel;   /* battery.charge */
    float temperatureC;   /* ups.temperature */
} SmsData;

/* Funções de parsing */
void sms_parse_features(uint8_t *rawvalues, SmsData *results);
void sms_parse_information(uint8_t *rawvalues, SmsData *results);
void sms_parse_results(uint8_t *rawvalues, SmsData *results);

/* Funções de montagem de comandos (7 bytes: CMD + P1–P4 + CHK + \r) */
uint8_t sms_prepare_get_status(uint8_t *buffer);             // 'Q'
uint8_t sms_prepare_get_information(uint8_t *buffer);        // 'I'
uint8_t sms_prepare_get_features(uint8_t *buffer);           // 'F'
uint8_t sms_prepare_set_beep(uint8_t *buffer);               // 'M'
uint8_t sms_prepare_test_battery_low(uint8_t *buffer);       // 'T' (16s)
uint8_t sms_prepare_test_battery_nsec(uint8_t *buffer, uint16_t delay); // 'T' custom
uint8_t sms_prepare_shutdown_nsec(uint8_t *buffer, uint16_t delay);     // 'R'
uint8_t sms_prepare_shutdown_restore(uint8_t *buffer);       // 'R' fixo
uint8_t sms_prepare_cancel_test(uint8_t *buffer);            // 'C'
uint8_t sms_prepare_cancel_shutdown(uint8_t *buffer);        // 'C'

#endif /* NUT_SMS_SER_H_SEEN */

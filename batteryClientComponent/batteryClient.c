/**
 * @file
 *
 * Blinks the user controlled LED at 1Hz. If the push-button is pressed, the LED
 * will remain on until the push-button is released.
 *
 * <HR>
 *
 * Copyright (C) Sierra Wireless, Inc. Use of this work is subject to license.
 */

#include "legato.h"
#include "interfaces.h"


void percentageHandler
(
    uint8_t percentage,
    uint8_t triggerPercentage,
    bool isHigh,
    void *context
)
{
    LE_INFO(
        "Received a %s battery level event.  Current level is %d%% and trigger percentage is %d%% and context=0x%p",
        isHigh ? "high" : "low", percentage, triggerPercentage, context);
}

void chargeHandler
(
    ma_battery_ChargeCondition_t chargecondition,
    void *context
)
{
    LE_INFO(
        "Received a charge alarm level event.  Charge Alarm  is %d and context=0x%p",
        chargecondition, context);
}

void healthHandler
(
    ma_battery_HealthCondition_t healthcondition,
    void *context
)
{
    LE_INFO(
        "Received a health alarm level event.  Health Alarm is %d and context=0x%p",
        healthcondition, context);
}

COMPONENT_INIT
{
    ma_adminbattery_SetTechnology("LIPO", 2000, 3700);


    char batteryType[256];
    uint16_t maH;
    uint16_t voltage; // millivolts
    uint16_t energy;  // mWh
    uint16_t percentremaining;
    uint16_t energyremaining;
    double currentvoltage, currenttemp;
    bool present;

    le_result_t result =
        ma_battery_GetTechnology(batteryType, sizeof(batteryType), &maH, &voltage, &energy);
    if (result == LE_OK)
    {
        LE_INFO(
            " battery type = %s, capacity = %hd mAh,  voltage = %hd mV, energy = %hd mWh",
            batteryType, maH, voltage, energy);
    }
    else
    {
        LE_ERROR("Unable to get battery technology info (%s)", LE_RESULT_TXT(result));
    }

    ma_battery_HealthCondition_t health = ma_battery_GetHealthStatus();
    LE_INFO(" health = %d", health);

    ma_battery_ChargeCondition_t charge = ma_battery_GetChargeStatus();
    LE_INFO("charge = %d", charge);

    le_result_t result1 = ma_battery_GetVoltage(&currentvoltage);
    if (result1 == LE_OK)
    {
        LE_INFO(" current voltage = %lf in millivolts", currentvoltage);
    }


    le_result_t result2 = ma_battery_GetTemp(&currenttemp);
    if (result2 == LE_OK)
    {
        LE_INFO(" current temperature = %lf in Celcius", currenttemp);
    }

    le_result_t result3 = ma_battery_GetPercentRemaining(&percentremaining);
    if (result3 == LE_OK)
    {
        LE_INFO(" current percent = %d", percentremaining);
    }


    le_result_t result4 = ma_battery_GetChargeRemaining(&energyremaining);
    if (result4 == LE_OK)
    {
        LE_INFO(" current energy = %d", energyremaining);
    }

    present = ma_battery_Present();
    LE_INFO("Present or not %d", present);


    ma_battery_AddLevelPercentageHandler(80, 90, percentageHandler, (void *)0xdeadbeef);

    ma_battery_AddAlarmChargeHandler(chargeHandler, NULL);

    ma_battery_AddAlarmHealthHandler(healthHandler, NULL);
}

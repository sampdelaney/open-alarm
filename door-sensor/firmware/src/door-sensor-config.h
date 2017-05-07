#ifndef DOOR_SENSOR_CONFIG_H
#define DOOR_SENSOR_CONFIG_H

#include <Arduino.h>
#include <DHT.h>

// Global/Application Macros
#define POLL_FREQUENCY       1               // multiples of 8 seconds

// Battery Macros
#define BATTERY_MIN_VOLTAGE   3700
#define BATTERY_MAX_VOLTAGE   2000
#define BATTERY_SENSE_PIN     A0
#define BATTERY_REF_VOLTAGE   INTERNAL
#define BATTERY_REF_VOLTAGEMV 1100
#define BATTERY_DIVIDER_RATIO (300+100)/100

// Environmental Sensing Macros
#define ENV_SENSOR_TYPE       DHT22
#define ENV_SENSOR_PIN        4

// Radio Macros
#define RADIO_CE_PIN          9
#define RADIO_CS_PIN          10
#define RADIO_WRITING_PIPE    0xF0F0F0F0E1LL
#define RADIO_READING_PIPE    0xF0F0F0F0D2LL


#endif // DOOR_SENSOR_CONFIG_H

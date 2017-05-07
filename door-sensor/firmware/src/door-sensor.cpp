#include <Arduino.h>
#include <SPI.h>
#include "door-sensor-config.h"

#include <Battery.h>
#include <Adafruit_Sensor.h>
#include <DHT.h>
#include <DHT_U.h>
#include <nRF24L01.h>
#include <RF24.h>
#include <avr/sleep.h>
#include <LowPower.h>

Battery battery(BATTERY_MIN_VOLTAGE, BATTERY_MAX_VOLTAGE, BATTERY_SENSE_PIN);
DHT_Unified envSensor(ENV_SENSOR_PIN, ENV_SENSOR_TYPE);
RF24 radio(RADIO_CE_PIN, RADIO_CS_PIN);

uint8_t sleep_ctr;

void sleepNow() {
  // reset sleep counter
  sleep_ctr = 0;
  while (sleep_ctr < POLL_FREQUENCY) {
    sleep_ctr++;
    // go to sleep for 8 seconds
    LowPower.powerDown(SLEEP_8S, ADC_OFF, BOD_OFF);
  }
}

void txNow() {
  String txMsg;
  // temperature
  sensors_event_t event;
  envSensor.temperature().getEvent(&event);
  txMsg += "Temp (C): ";
  txMsg += String(event.temperature, 2);

  // battery
  txMsg += " Battery (mV): ";
  txMsg += String(battery.voltage());

  // send the message
  radio.write(txMsg.c_str(), txMsg.length()*sizeof(char));
  Serial.println(txMsg);
}

void setup()
{
  Serial.begin(9600);

  analogReference(BATTERY_REF_VOLTAGE);

  // start all the services
  battery.begin(BATTERY_REF_VOLTAGEMV, BATTERY_DIVIDER_RATIO);
  envSensor.begin();

  radio.begin();
  radio.openWritingPipe(RADIO_WRITING_PIPE);
  radio.openReadingPipe(1, RADIO_READING_PIPE);
}

void loop()
{
  // tx sensor info
  txNow();
  // go to sleep
  //sleepNow();
}

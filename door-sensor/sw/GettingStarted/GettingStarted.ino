
/*
* Getting Started example sketch for nRF24L01+ radios
* This is a very basic example of how to send data from one node to another
* Updated: Dec 2014 by TMRh20
*/

#include <SPI.h>
#include "RF24.h"
#include <Adafruit_Sensor.h>
#include <DHT.h>
#include <DHT_U.h>

/****************** User Config ***************************/
/***      Set this radio as radio number 0 or 1         ***/
bool radioNumber = 0;

/* Hardware configuration: Set up nRF24L01 radio on SPI bus plus pins 7 & 8 */
RF24 radio(9,10);
/**********************************************************/

byte addresses[][6] = {"1Node","2Node"};

// Used to control whether this node is sending or receiving
bool role = 0;

// Battery Health
const int batteryPin = A0;
const float adcRef = 1.1;
const float divider = 4;
const float calibration = 0.9767981439;

// DHT22
DHT_Unified dht(2, DHT22);
int delayMs;

void setup() {
  Serial.begin(115200);
//  Serial.println(F("RF24/examples/GettingStarted"));
//  Serial.println(F("*** PRESS 'T' to begin transmitting to the other node"));
  
  radio.begin();
  dht.begin();

  // Set the PA Level low to prevent power supply related issues since this is a
 // getting_started sketch, and the likelihood of close proximity of the devices. RF24_PA_MAX is default.
  radio.setPALevel(RF24_PA_LOW);
  
  // Open a writing and reading pipe on each radio, with opposite addresses
  if(radioNumber){
    radio.openWritingPipe(addresses[1]);
    radio.openReadingPipe(1,addresses[0]);
  }else{
    radio.openWritingPipe(addresses[0]);
    radio.openReadingPipe(1,addresses[1]);
  }
  // make sure we're using the 1.1V internal reference
  analogReference(INTERNAL);
  
  // setup dht22
  dht.begin();
  sensor_t sensor;
  dht.temperature().getSensor(&sensor);
  dht.humidity().getSensor(&sensor);
  delayMs = sensor.min_delay / 1000;
}

void loop() {
  
  // Read Battery Health
  float batteryRaw = (float)analogRead(batteryPin);
  float batteryVolts = calibration*divider * batteryRaw *  (adcRef/1023);
  
  // Read from DHT
  float temperature = -99;
  sensors_event_t event;  
  dht.temperature().getEvent(&event);
  if (!isnan(event.temperature)) {
    temperature = event.temperature;
  }
  float humidity = -99;
  dht.humidity().getEvent(&event);
  if (!isnan(event.relative_humidity)) {
    humidity = event.relative_humidity;
  }
  
  // build message
  String output;
  output += "V:";
  output += String(batteryVolts,2);
  output += ",T:";
  output += String(temperature,2);
  output += ",H:";
  output += String(humidity,2);
  
  // output to console for debugging
  Serial.println(output);
  
  // 
  if (!radio.write(output.c_str(), output.length()*sizeof(char))){
    Serial.println("TX Failed"); 
  }
  // transmit over radio
//  
///****************** Ping Out Role ***************************/  
//if (role == 1)  {
//    
//    radio.stopListening();                                    // First, stop listening so we can talk.
//    
//    
//    Serial.println(F("Now sending"));
//
//    unsigned long start_time = micros();                             // Take the time, and send it.  This will block until complete
//     if (!radio.write( &start_time, sizeof(unsigned long) )){
//       Serial.println(F("failed"));
//     }
//        
//    radio.startListening();                                    // Now, continue listening
//    
//    unsigned long started_waiting_at = micros();               // Set up a timeout period, get the current microseconds
//    boolean timeout = false;                                   // Set up a variable to indicate if a response was received or not
//    
//    while ( ! radio.available() ){                             // While nothing is received
//      if (micros() - started_waiting_at > 200000 ){            // If waited longer than 200ms, indicate timeout and exit while loop
//          timeout = true;
//          break;
//      }      
//    }
//        
//    if ( timeout ){                                             // Describe the results
//        Serial.println(F("Failed, response timed out."));
//    }else{
//        unsigned long got_time;                                 // Grab the response, compare, and send to debugging spew
//        radio.read( &got_time, sizeof(unsigned long) );
//        unsigned long end_time = micros();
//        
//        // Spew it
//        Serial.print(F("Sent "));
//        Serial.print(start_time);
//        Serial.print(F(", Got response "));
//        Serial.print(got_time);
//        Serial.print(F(", Round-trip delay "));
//        Serial.print(end_time-start_time);
//        Serial.println(F(" microseconds"));
//    }
//
//    // Try again 1s later
//    delay(1000);
//  }
  delay(delayMs);
} // Loop


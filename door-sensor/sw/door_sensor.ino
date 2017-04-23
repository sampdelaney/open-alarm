// Copyright (C) 2017 Sam Delaney
//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.

#include <avr/sleep.h>

#define SWITCH 2              // pin number switch
#define OPEN LOW            // alias low and high as open and closed respectively
#define CLOSED HIGH          

void wakeUpNow()        // here the interrupt is handled after wakeup
{
  ;
}

int sleep_until_switch() {
  // read current state of switch
  int switchState = digitalRead(SWITCH);
  // establish interrupt transition
  int intTransit = (switchState == HIGH) ? LOW : HIGH;
  set_sleep_mode(SLEEP_MODE_PWR_DOWN);
  sleep_enable();
  attachInterrupt(0,wakeUpNow, intTransit);
  sleep_mode();
  sleep_disable();
  detachInterrupt(0);
  return digitalRead(SWITCH);  
}

void setup()
{
  pinMode(SWITCH, INPUT);

  Serial.begin(9600); 
  while (!Serial) {
    ; // wait for serial port to connect. Needed for Leonardo only
  }  
  Serial.println("Sam's Alarm System");
}

void loop() {
  // read current state of switch
  int switchState = digitalRead(SWITCH);
  
  Serial.print("Current Switch State: ");
  if (switchState == CLOSED) {
    Serial.println("Closed");
  } else {
    Serial.println("Open");    
  }
  
  // go to sleep
  Serial.println("Going to sleep...");
  Serial.flush();
  Serial.end();
  switchState = sleep_until_switch();
  
  // send out alert
  Serial.begin(9600); 
  while (!Serial) {}
  Serial.println("Woke Up!");
  Serial.print("Switch State Changed to... ");
  if (switchState == CLOSED) {
    Serial.println("Closed");    
  } else {
    Serial.println("Open");
  }  
}


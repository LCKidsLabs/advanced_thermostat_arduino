#include <Arduino.h>
#line 1 "C:\\Users\\ctgcontrols8601\\Documents\\GitHub\\LckidsLabs\\advanced_thermostat_arduino\\advanced_thermostat_arduino.ino"
#include <HardwareSerial.h> 

#line 3 "C:\\Users\\ctgcontrols8601\\Documents\\GitHub\\LckidsLabs\\advanced_thermostat_arduino\\advanced_thermostat_arduino.ino"
void setup();
#line 8 "C:\\Users\\ctgcontrols8601\\Documents\\GitHub\\LckidsLabs\\advanced_thermostat_arduino\\advanced_thermostat_arduino.ino"
void loop();
#line 3 "C:\\Users\\ctgcontrols8601\\Documents\\GitHub\\LckidsLabs\\advanced_thermostat_arduino\\advanced_thermostat_arduino.ino"
void setup() {
  // Initialize serial communication at 19200 bits per second:
  Serial1.begin(115200);
}

void loop() {
  
  // Print "Hello World!" to the Serial Monitor, followed by a new line:
  printf("Hello World!\r\n");
  // Wait for 1 second (1000 milliseconds) before repeating:
  delay(1000);
}

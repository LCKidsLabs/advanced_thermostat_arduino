#include <HardwareSerial.h> 

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
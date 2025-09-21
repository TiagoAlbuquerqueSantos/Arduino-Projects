#include <TimerOne.h>
#include <Wire.h>
#include <MultiFuncShield.h>

/*

For more information and help, please visit https://www.cohesivecomputing.co.uk/hackatronics/arduino-multi-function-shield/part-1/

*/

void setup() {
  // put your setup code here, to run once:

  Timer1.initialize();
  MFS.initialize(&Timer1);    // initialize multi-function shield library
}

void loop() {
  // put your main code here, to run repeatedly:

  MFS.write(analogRead(POT_PIN));
  
  delay(100);
}

#include "Stop.h"
#include <Arduino.h>
//#include <iostream>

void Stop::execute() const
{
  //std::cout << "Executing Stop strategy" << std::endl;
  Serial.println("Stop");
  digitalWrite(MOTOR_1_PIN_1, 0);
  digitalWrite(MOTOR_1_PIN_2, 0);
  digitalWrite(MOTOR_2_PIN_1, 0);
  digitalWrite(MOTOR_2_PIN_2, 0);
}
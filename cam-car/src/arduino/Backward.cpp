#include "Backward.h"
#include <Arduino.h>
//#include <iostream>

void Backward::execute() const
{
 // std::cout << "Executing Backward strategy" << std::endl;

  Serial.println("Backward");
  digitalWrite(MOTOR_1_PIN_1, 0);
  digitalWrite(MOTOR_1_PIN_2, 1);
  digitalWrite(MOTOR_2_PIN_1, 0);
  digitalWrite(MOTOR_2_PIN_2, 1);
}
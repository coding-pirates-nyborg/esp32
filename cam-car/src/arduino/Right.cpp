#include "Right.h"
#include <Arduino.h>
//#include <iostream>

void Right::execute() const
{
  //std::cout << "Executing Right strategy" << std::endl;
  Serial.println("Right");
  digitalWrite(MOTOR_1_PIN_1, 1);
  digitalWrite(MOTOR_1_PIN_2, 0);
  digitalWrite(MOTOR_2_PIN_1, 0);
  digitalWrite(MOTOR_2_PIN_2, 1);
}
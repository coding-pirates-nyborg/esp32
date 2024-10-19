#include "Forward.h"
#include <Arduino.h>
//#include <iostream>

void Forward::execute() const
{
 // std::cout << "Executing Forward strategy" << std::endl;

  Serial.println("Forward");
  digitalWrite(MOTOR_1_PIN_1, 1);
  digitalWrite(MOTOR_1_PIN_2, 0);
  digitalWrite(MOTOR_2_PIN_1, 1);
  digitalWrite(MOTOR_2_PIN_2, 0);
}

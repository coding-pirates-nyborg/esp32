#include "Left.h"
#include <Arduino.h>
//#include <iostream>

void Left::execute() const
{
  //std::cout << "Executing Left strategy" << std::endl;

    Serial.println("Left");
    digitalWrite(MOTOR_1_PIN_1, 0);
    digitalWrite(MOTOR_1_PIN_2, 1);
    digitalWrite(MOTOR_2_PIN_1, 1);
    digitalWrite(MOTOR_2_PIN_2, 0);
}
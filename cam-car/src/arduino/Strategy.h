#ifndef STRATEGY_H
#define STRATEGY_H
#include "GPIO.h"
class Strategy{
  public:
    virtual ~Strategy() {};
    virtual void execute() const = 0;
};

#endif // STRATEGY_H

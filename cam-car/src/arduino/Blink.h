#ifndef BLINK_H
#define BLINK_H

#include "Strategy.h"

class Blink : public Strategy
{
public:
    void execute() const override;
};

#endif // BLINK_H
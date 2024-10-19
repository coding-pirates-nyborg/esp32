#ifndef BACKWARD_H
#define BACKWARD_H

#include "Strategy.h"

class Backward : public Strategy
{
public:
    void execute() const override;
};

#endif // BACKWARD_H
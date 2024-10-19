#ifndef STOP_H
#define STOP_H

#include "Strategy.h"

class Stop : public Strategy {
public:
    void execute() const override;
};

#endif // STOP_H
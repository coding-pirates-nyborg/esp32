#ifndef FORWARD_H
#define FORWARD_H

#include "Strategy.h"

class Forward : public Strategy {
public:
    void execute() const override;
};

#endif // FORWARD_H
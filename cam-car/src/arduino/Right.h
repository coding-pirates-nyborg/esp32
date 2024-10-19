#ifndef RIGHT_H
#define RIGHT_H

#include "Strategy.h"

class Right : public Strategy {
public:
    void execute() const override;
};

#endif // RIGHT_H
#ifndef Left_H
#define Left_H

#include "Strategy.h"

class Left : public Strategy {
public:
    void execute() const override;
};

#endif // Left_H
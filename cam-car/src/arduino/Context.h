#ifndef CONTEXT_H
#define CONTEXT_H

#include "Strategy.h"

class Context {
private:
    Strategy* strategy;
public:
    Context(Strategy* strategy = nullptr) : strategy(strategy) {}
    ~Context() {
        delete strategy;
    }
    void setStrategy(Strategy* strategy) {
        if (this->strategy) {
            delete this->strategy;
        }
        this->strategy = strategy;
    }
    void executeStrategy() const {
        if (strategy) {
            strategy->execute();
        }
    }
};

#endif // CONTEXT_H
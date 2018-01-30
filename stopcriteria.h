#ifndef STOPCRITERIA
#define STOPCRITERIA

class StopCriteria {
    virtual void step() = 0;
    virtual bool execute() = 0;
};

class VoidStopCriteria {
    virtual bool check() {
        return false;
    }
};

class MaxIterationSC : public StopCriteria {
    virtual bool check() {
        return
    }
};

#endif // STOPCRITERIA


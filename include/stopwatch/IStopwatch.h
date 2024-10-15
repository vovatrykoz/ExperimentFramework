#ifndef _I_STOPWATCH_H_
#define _I_STOPWATCH_H_

class IStopwatch {
public:
    ~IStopwatch() = default;
    virtual void Start() = 0;
    virtual void Stop() = 0;
};

#endif
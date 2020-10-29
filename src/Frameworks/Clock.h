#ifndef SLIMSHADY_CLOCK_H
#define SLIMSHADY_CLOCK_H

#include <ctime>
#include <string>

class Clock {
  public:
    Clock();
    ~Clock();

    static std::string GetCurrentTimeAsString();
};


#endif //SLIMSHADY_CLOCK_H

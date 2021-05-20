#include "Clock.h"

#include <chrono>
#include <date/date.h>

Clock::Clock() = default;

Clock::~Clock() = default;

std::string Clock::GetCurrentTimeAsString() {
  return date::format("%F %R", std::chrono::system_clock::now());
}

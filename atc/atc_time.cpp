#include "atc_time.h"
#include <string>
#include <iostream>
#include <chrono>

long atc_time::now_epoch(){
    const auto now = std::chrono::system_clock::now();
    const auto epoch   = now.time_since_epoch();
    const auto seconds = std::chrono::duration_cast<std::chrono::seconds>(epoch);
    return seconds.count();
}

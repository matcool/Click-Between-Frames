#pragma once

using TimestampType = int64_t;

TimestampType getCurrentTimestamp();

#if defined(GEODE_IS_WINDOWS)
#include "windows.hpp"
#elif defined(GEODE_IS_ANDROID)
#include "android.hpp"
#endif
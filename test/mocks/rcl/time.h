#pragma once

#include <cstdint>

#define RCL_MS_TO_NS(milliseconds) (static_cast<uint64_t>(milliseconds) * 1000000ULL)
#include <rcl/rcl.h>

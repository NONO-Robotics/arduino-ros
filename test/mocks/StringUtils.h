#pragma once

#include <Arduino.h>

inline char *toCharArray(const String &value) {
  return const_cast<char *>(value.c_str());
}

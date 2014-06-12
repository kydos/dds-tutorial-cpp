#ifndef SIMD_DEMO_DDJ_01_UTIL_HPP_
#define SIMD_DEMO_DDJ_01_UTIL_HPP_

// -- Std C/C++ Include
#include <time.h>
#include <iostream>

// -- Custom Include
#include "gen/ccpp_TempControl.h"

extern const unsigned long int  ONE_SECOND;
extern const unsigned int       DEFAULT_SAMPLE_NUM;
extern const unsigned int       DEFAULT_SENSOR_ID;
extern const TemperatureScale   DEFAULT_TEMP_SCALE;
extern const float              T0;
extern const float              DT;
extern const float              H0;
extern const float              DH;

// Command line options for the tssub program
struct tssub_options {
  unsigned int samples;
  timespec period; // in msec
};

// Command line options for the tspub program
struct tspub_options {
  unsigned short id;
  TemperatureScale scale;
  unsigned int samples;
  timespec period; // in msec
  float t0;  
  float dt; // delta-T
  float h0;
  float dh; // delta-H
};

// converts milliseconds into a struct timespec that can be
// used with a nanosleep
timespec msec2timespec(unsigned long long msec);

// Converts a character into a proper TemperatureScale enumeration.
TemperatureScale char2tempscale(char s);

// Converts a TemperatureScale into a string
char tempscale2char(TemperatureScale scale);

std::ostream& operator<< (std::ostream& os, const TempSensorType& ts);

// parses the command-line arguments
tssub_options parse_tssub_args(int argc, char* argv[]);

tspub_options parse_tspub_args(int argc, char* argv[]);

#endif /* SIMD_DEMO_DDJ_01_UTIL_HPP_ */

// -- Custom Include
#include "util.hpp"
// -- Boost Include
// -- Std C++ Include
#include <iostream>

const long int       ONE_SECOND = 1E9;
const unsigned int   DEFAULT_SAMPLE_NUM = 1E3;


timespec msec2timespec(unsigned long long msec) {
  long int nsec = msec * 1E6;
  timespec ts = {0, nsec};
  // nsec has to be <= 999999999
  
  if (nsec >= ONE_SECOND) {
    ts.tv_sec = nsec / ONE_SECOND;
    ts.tv_nsec = nsec - (ts.tv_sec * ONE_SECOND);
  } 

  return ts;
}

tutorial::TemperatureScale char2tempscale(char s) {
  tutorial::TemperatureScale scale;
  switch (s) {
    case 'C': 
      scale = tutorial::TemperatureScale::CELSIUS;
      break;
    case 'F':
      scale = tutorial::TemperatureScale::FAHRENHEIT;
      break;
    case 'K':
      scale = tutorial::TemperatureScale::KELVIN;
      break;
    default:
      throw std::range_error("Unknown Temperature Scale");
  }
  return scale;
}

char scale_name[3] = {'C', 'F', 'K'};

char tempscale2char(tutorial::TemperatureScale scale) {
  int idx = static_cast<int>(scale);
  return scale_name[idx];
}

std::ostream&
operator << (std::ostream& os, const tutorial::TempSensorType& ts)
{
  os << "(id = " << ts.id()  
     << ", temp = " << ts.temp()
     << ", hum = " << ts.hum()
     << ", scale = " << tempscale2char(ts.scale())
     << ")";
     
  return os;
}




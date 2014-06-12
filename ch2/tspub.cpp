// -- Std C/C++ Include
#include <iostream>
#include <stdlib.h>
#include <time.h>
// -- Boost Include
#include <boost/program_options.hpp>
// -- SimD Include
#include <dds/dds.hpp>
// -- Custom Include
#include "gen/ccpp_TempControl.h"
#include "util.hpp"

REGISTER_TOPIC_TRAITS(TempSensorType);

const unsigned short DEFAULT_ID = 0;

int
main(int argc, char* argv[])
{
  tspub_options opt;
  try {
    // Parse the command line args
    opt = parse_tspub_args(argc, argv);
  }
  catch (...) { }
    // Initialize random number generation with a seed
    srandom(clock());
    // Create a SimD runtime
    dds::Runtime runtime("");
    // Create the "TempSensor" Topic
    dds::Topic<TempSensorType> tsTopic("TempSensor");
    // Create a DataWriter
    dds::DataWriter<TempSensorType> dw(tsTopic);
    
    // Write some temperature randomly changing around a set point
    float temp = opt.t0 + ((random()*opt.dt)/RAND_MAX);
    float hum  = opt.h0 + ((random()*opt.dh)/RAND_MAX);
    TempSensorType sensor = { opt.id, temp, hum, opt.scale };
    for (unsigned int i = 0; i < opt.samples; ++i) {
      dw.write(sensor);
      std::cout << "." << std::flush;
      nanosleep(&opt.period, 0);
      sensor.temp = opt.t0 + ((random()*opt.dt)/RAND_MAX);
      sensor.hum =  opt.h0 + ((random()*opt.dh)/RAND_MAX);
    }
    std::cout << std::endl;
  return 0;
}

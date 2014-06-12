// -- Std C++ Include
#include <iostream>
// -- Boost Include
#include <boost/program_options.hpp>
// -- SimD includes
#include <dds/dds.hpp>
#include <gen/ccpp_TempControl.h>
// -- Custom Include
#include "util.hpp"

// Does some C++ magic required by SimD
REGISTER_TOPIC_TRAITS(TempSensorType);

int
main(int argc, char* argv[]) {
  tssub_options opt = { DEFAULT_SAMPLE_NUM, {0, ONE_SECOND/2}};
  if (argc < 2) {
    std::cout << "USAGE:\n\ttssub <filter> <args1> <arg2> ... <argn>"
              << std::endl;
    exit(-1);
  }
  // Init the SimD runtime
  dds::Runtime runtime("");
  // Create the "TempSensor" Topic
  dds::Topic<TempSensorType> tsTopic("TempSensor");
  std::vector<std::string> params;
  for (int i = 2; i < argc; ++i)
    params.push_back(argv[i]);

  dds::ContentFilteredTopic<TempSensorType> cfTsTopic("TempSensor-1",
                                                      tsTopic,
                                                      argv[1],
                                                      params);
  // Create the DataReader
  dds::DataReader<TempSensorType> dr(cfTsTopic);
  // Declare the containers
  TempSensorTypeSeq data;
  DDS::SampleInfoSeq status;

  std::string str = "Hello\
  now";
  // Poll & Sleep untill you've not read opt.samples
  unsigned int count = 0;
  while (count < opt.samples) {
    std::cout << "..." << std::endl;
    // Read data from DDS
    dr.read(data, status);
    count += data.length();
    // Display all the read samples
    for (unsigned int i = 0; i < data.length(); ++i) {
      std::cout << data[i] << std::endl;
    }
    // Return the loaned memory to DDS.
    dr.return_loan(data, status);
    // Sleep a bit before issuing another read
    nanosleep(&opt.period, 0);
  }
  return 0;
}

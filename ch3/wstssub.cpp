// -- Std C/C++ Include
#include <iostream>
#include <gen/TempControl_DCPS.hpp>
#include <thread>         // std::thread, std::this_thread::sleep_for
#include <chrono> 
#include "util.hpp"


int
main(int argc, char* argv[]) {
  dds::domain::DomainParticipant dp(0);
  dds::topic::Topic<tutorial::TempSensorType> topic(dp, "TTempSensor");
  dds::sub::Subscriber sub(dp);
  dds::sub::DataReader<tutorial::TempSensorType> dr(sub, topic);
  
  // Create the waitset
  dds::core::cond::WaitSet ws;
  // Create a ReadCondition for our data reader 
  // and configure it for new data
  dds::sub::cond::ReadCondition rc(dr, dds::sub::status::DataState::new_data());
  // attache the condition
  ws += rc;

  while (true) {
    // wait for new data to be avaialable
    ws.wait();
    auto samples = dr.read();
    std::for_each(samples.begin(),
		  samples.end(),
		  [](const dds::sub::Sample<tutorial::TempSensorType>& s) {
		    std::cout << s.data() << std::endl;
		  });
  }

  return 0;
}

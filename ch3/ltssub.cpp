// -- Std C/C++ Include
#include <iostream>
#include <algorithm>
#include <gen/TempControl_DCPS.hpp>
#include <thread>         // std::thread, std::this_thread::sleep_for
#include <chrono> 
#include "util.hpp"

class TempSensorListener : public dds::sub::NoOpDataReaderListener<tutorial::TempSensorType> {
public:
  virtual void 
  on_data_available(dds::sub::DataReader<tutorial::TempSensorType>& dr) 
  { 
    std::cout << "----------on_data_available-----------" << std::endl;      
    auto samples =  dr.read();
    std::for_each(samples.begin(),
		  samples.end(),
		  [](const dds::sub::Sample<tutorial::TempSensorType>& s) {
		    std::cout << s.data() << std::endl;
		  });
  }

  virtual void 
  on_liveliness_changed(dds::sub::DataReader<tutorial::TempSensorType>& dr, 
			const dds::core::status::LivelinessChangedStatus& status) 
  {
    std::cout << "!!! Liveliness Changed !!!" << std::endl;
  }
};


int
main(int argc, char* argv[]) {
  const int duration = 60; // sec
  dds::domain::DomainParticipant dp(0);
  dds::topic::Topic<tutorial::TempSensorType> topic(dp, "TTempSensor");
  dds::sub::Subscriber sub(dp);
  
  dds::sub::DataReader<tutorial::TempSensorType> dr(sub, topic); 

  TempSensorListener listener;
  dr.listener(&listener, dds::core::status::StatusMask::data_available());
  std::this_thread::sleep_for(std::chrono::seconds(duration));
  dr.listener(nullptr, dds::core::status::StatusMask::none());

  return 0;
}

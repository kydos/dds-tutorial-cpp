#include <dds/dds.hpp>
#include <gen/ccpp_TempControl.h>

REGISTER_TOPIC_TRAITS(TempSensorType);

class TempSensorDataHandler {
public:
  void operator() (dds::DataReader<TempSensorType>& reader) {
    std::cout << "Reading..." << std::endl;
    std::vector<TempSensorType>    data;
    std::vector<dds::SampleInfo>   info;
    reader.read(data, info);
  }
};

int main(int, char**) { 
  bool not_done = true;
  dds::Runtime rt("");
  dds::Topic<TempSensorType> topic("TempSensorTopic");

  dds::DataReader<TempSensorType> dr(topic);

  //[NOTE #1]: Create an instance of the handler
  TempSensorDataHandler handler;

  //[NOTE #2]: Create a read condition for the given reader
  auto rcond = dr.create_readcondition(handler);

  //[NOTE #3]: Create a Waitset and attach the condition
  dds::WaitSet ws;
  ws.attach(rcond);

  dds::Duration timeout = {1, 0};

      
  //[NOTE #4] Wait for some condition to become active, and retrieve
  //all active conditions
  auto conds = ws.wait(timeout);
  
  //[NOTE #5] Execute the condition handlers
  //  for (auto i = conds.begin(); i < conds.end(); ++i)
  //    i->execute();
  
  
  //[NOTE #5] Automatically dispatch the condition handlers when
  //conditions are notified
  ws.dispatch(timeout);

  return 0; 
}

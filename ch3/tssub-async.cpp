#include <dds/dds.hpp>
#include <gen/ccpp_TempControl.h>

REGISTER_TOPIC_TRAITS(TempSensorType);

class TempSensorDataHandler {
public:
  void handle_data(dds::DataReader<TempSensorType>& reader) {
    std::cout << "Reading..." << std::endl;
    std::vector<TempSensorType> data;
    std::vector<dds::SampleInfo>     info;
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


  auto func = 
    boost::bind(TempSensorDataHandler::handle_data, &handler, _1);

  //[NOTE #2]: Register the handler for the relevant event
  auto connection = 
    dr.connect<on_data_available>(func);
  

  return 0; 
}

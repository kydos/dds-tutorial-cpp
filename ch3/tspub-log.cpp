#include <dds/dds.hpp>
#include "gen/ccpp_TempControl.h"

/*
* struct TempSensorType
{
DDS::Short id;
DDS::Float temp;
DDS::Float hum;
TemperatureScale scale;
};
*/
REGISTER_TOPIC_TRAITS(TempSensorType);

int main(int argc, char* argv[]) {
  dds::Runtime runtime("sensor-data");
  
  dds::Topic<TempSensorType> topic("TempSensorTopic");
  
  // -- Avoid topic-instance dispose on unregister
  dds::DataWriterQos dwqos;
  dwqos.set_auto_dispose(false);

  dds::DataWriter<TempSensorType> dw(topic, dwqos);
  
  TempSensorType data = {1, 24.3F, 0.5F, CELSIUS};
  dw.write(data);
  
  data = {2, 23.5F, 0.6F, CELSIUS};
  dw.write(data);
  
  data = {3, 21.7F, 0.5F, CELSIUS};
  dw.write(data);

    TempSensorType key;
    key.id = 1;
    auto diw1 = dw.register_instance(key);

    key.id = 2;
    auto diw2 = dw.register_instance(key);
	
    key.id = 3;
    auto diw3 = dw.register_instance(key);

    data = {1, 24.3F, 0.5F, CELSIUS};
    diw1.write(data);
    
    data = {2, 23.5F, 0.6F, CELSIUS};
    diw2.write(data);
    
    data = {3, 21.7F, 0.5F, CELSIUS};
    diw3.write(data);
    
	
    diw2.unregister();

    diw3.dispose();
    
  return 0;
}

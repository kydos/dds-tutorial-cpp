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
  dds::DataWriterQos dwqos;
  //  dwqos.set_auto_dispose(false);

  dds::DataWriter<TempSensorType> dw(topic, dwqos);
  char ch;
  
  std::cout << "Perss a key to write  data: " << std::endl;
  std::cin >> ch;

  TempSensorType data = {1, 24.3F, 0.5F, CELSIUS};
  dw.write(data);
  
  data = {2, 23.5F, 0.6F, CELSIUS};
  dw.write(data);
  
  data = {3, 21.7F, 0.5F, CELSIUS};
  dw.write(data);

  std::cout << "Perss a key to continue (x to exit): " << std::endl;
  std::cin >> ch;
  if (ch == 'x')
    return 0;
  
  for (int i = 0; i < 3; ++i) {  
    std::cout << "Press a key to register instances: ";
    std::cin >> ch;

    TempSensorType key;
    key.id = 1;
    auto diw1 = dw.register_instance(key);

    key.id = 2;
    auto diw2 = dw.register_instance(key);
	
    key.id = 3;
    auto diw3 = dw.register_instance(key);

    std::cout << "Press a key to write instances: ";
    std::cin >> ch;
    TempSensorType data;

    data = {1, 24.3F, 0.5F, CELSIUS};
    diw1.write(data);
    
    data = {2, 23.5F, 0.6F, CELSIUS};
    diw2.write(data);
    
    data = {3, 21.7F, 0.5F, CELSIUS};
    diw3.write(data);
    
    
    std::cout << "Press a key to unregister id=2: ";
    std::cin >> ch;
    
    diw2.unregister();

    std::cout << "Press a key to dispose id=3: ";
    std::cin >> ch;
    diw3.dispose();
    
    std::cout << "Press a key to exit: ";
    std::cin >> ch;
  }
  return 0;
}

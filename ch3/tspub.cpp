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

int main(int, char**) {
   dds::Runtime runtime;
   
   dds::Topic<TempSensorType> topic("TempSensorTopic");
   
   //[NOTE #1]: Avoid topic-instance dispose on unregister
   dds::DataWriterQos dwqos;
   dwqos.set_auto_dispose(false);
   
   //[NOTE #2]: Creating DataWriter with custom QoS.
   // QoS will be covered in detail in article #4.
   dds::DataWriter<TempSensorType> dw(topic, dwqos);
   
   TempSensorType data = {0, 24.3F, 0.5F, CELSIUS};
   dw.write(data);   

   TempSensorType key;
   key.id = 1;
   
   //[NOTE #3]: Using C++0x “auto” to keep code more compact
   //[NOTE #4] Registering topic-instance explicitly 
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
   
   // [NOTE #5]: unregister topic-instance with id=1 
   diw1.unregister();
   
   // [NOTE #6]: dispose topic-instance with id=2
   diw2.dispose();
   

   //[NOTE #7]:topic-instance with id=3 will be unregistered
   // as result of the diw3 object destruction
   
   //[NOTE #8]: topic instance with id=0 will be unregistered as 
   // result of the dw object destruction 
   return 0;
}
 

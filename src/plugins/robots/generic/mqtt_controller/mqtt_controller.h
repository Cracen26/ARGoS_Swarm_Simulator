#ifndef MQTT_CONTROLLER_H
#define MQTT_CONTROLLER_H

#include <argos3/core/control_interface/ci_controller.h>
#include <mosquitto.h>

class CMQTTController : public argos::CCI_Controller {
public:
   CMQTTController();
   virtual ~CMQTTController() {}
   virtual void Init(argos::TConfigurationNode& t_node);
   virtual void ControlStep();
   virtual void Reset();
   virtual void Destroy();
private:
   struct mosquitto* m_pcMosq;
};

#endif
#endif


#include "mqtt_controller.h"
#include <argos3/core/utility/logging/argos_log.h>
#include <argos3/core/simulator/entity/embodied_entity.h>
#include <argos3/core/simulator/simulator.h>
#include <argos3/core/utility/math/vector3.h>

#include <mosquitto.h>   // from libmosquitto-dev

using namespace argos;

CMQTTController::CMQTTController() : m_pcMosq(nullptr) {}

void CMQTTController::Init(TConfigurationNode& t_node) {
    // Initialize MQTT client
    mosquitto_lib_init();
    m_pcMosq = mosquitto_new(nullptr, true, nullptr);
    if(!m_pcMosq) THROW_ARGOSEXCEPTION("Could not create MQTT client.");

    int rc = mosquitto_connect(m_pcMosq, "localhost", 1883, 60);
    if(rc) THROW_ARGOSEXCEPTION("Could not connect to MQTT broker.");
}

void CMQTTController::ControlStep() {
    // Example: publish position each tick
    CVector3 pos = GetRobot().GetEmbodiedEntity().GetOriginAnchor().Position;
    std::string payload = 
        "{" 
        "\"id\":\"" + GetId() + "\"," +
        "\"x\":" + std::to_string(pos.GetX()) + "," +
        "\"y\":" + std::to_string(pos.GetY()) +
        "}";

    mosquitto_publish(m_pcMosq, nullptr, "argos/robots/status",
                      payload.size(), payload.c_str(), 0, false);
}

void CMQTTController::Reset() {}
void CMQTTController::Destroy() {
    if(m_pcMosq) {
        mosquitto_disconnect(m_pcMosq);
        mosquitto_destroy(m_pcMosq);
        mosquitto_lib_cleanup();
    }
}

REGISTER_CONTROLLER(CMQTTController, "mqtt_controller")


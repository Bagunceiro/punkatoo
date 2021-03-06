#pragma once

#include <Arduino.h>
#include "infrared.h"
#include "mqtt.h"

class P2System : public MQTTClientDev
{
public:
    P2System();
    virtual ~P2System();

    void routine();

    void reset();

    void subscribeToMQTT();
    void mqttMsgRecd(const String &topic, const String &msg);

//    void subscribeToIR() {}
    String mqttGetStatus();

    enum State
    {
        STATE_0 = 0,
        STATE_AWAKE,
        STATE_NETWORK,
        STATE_MQTT,
        STATE_WPS,
        STATE_CONFIGURATOR,
        STATE_UPDATE
    };

    void enterState(enum State s);
    void revertState() { st = prevst; }

    private:
    bool resetFlag;
    State st;
    State prevst;
};

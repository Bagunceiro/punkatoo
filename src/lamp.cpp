#include <Arduino.h>
#include "config.h"
#include "lamp.h"
#include "rgbled.h"

extern RGBLed indicator;

const int MAXDEBOUNCE = 5; // Number of loops to allow light switch to settle

Lamp::Lamp(String devName) : MqttControlled(devName) {}

Lamp::~Lamp() {}

void Lamp::sw(int toState)
{
  Serial.printf("Switch Lamp to %d\n", toState);
  if (toState == 0)
  {
    digitalWrite(lpin, HIGH);
    #ifdef TESTING
    indicator.off();
    #endif
  }
  else
  {
    digitalWrite(lpin, LOW);
    #ifdef TESTING
    indicator.setColour(RGBLed::WHITE);
    #endif
  }
  sendStatus();
}

void Lamp::toggle()
{
  int isOn = getStatus().toInt();
  sw(isOn == 0 ? 1 : 0);
}

void Lamp::pollSwitch()
{
  for (SwBlk& si : swList)
  {
    int newState = digitalRead(si.spin);
    if (newState != si.switchState)
    {
      si.debounce++;
      if (si.debounce > MAXDEBOUNCE)
      {
        // Serial.println("Switch pressed");
        toggle();
        si.switchState = newState;
        si.debounce = 0;
      }
    }
  }
}

String Lamp::getStatus()
{
  int l = digitalRead(lpin);
  return String((l == 0 ? 1 : 0));
}

const int Lamp::blip(const int t)
{
  toggle();
  delay(t);
  toggle();
  return (t);
}

void Lamp::blip(const int number, const int length)
{
    for (int i = number; i > 0; i--)
    {
      blip(length);
      if (i > 1) delay(length);
    }
}

void Lamp::init(const SwitchList inpList, int out)
{
  for (int inp : inpList)
  {
    SwBlk si;
    si.spin = inp;
    pinMode(si.spin, INPUT_PULLUP);
    // delay(500); // input pin appears to need settling time after mode setting??
    si.switchState = digitalRead(si.spin);
    si.debounce = 0;
    swList.push_back(si);
  }
  lpin = out;
  pinMode(lpin, OUTPUT);
  sw(0);
}

void Lamp::mqttaction(const String& topic, const String& msg)
{
  if (topic == MQTT_TPC_SWITCH)
  {
    sw(msg.toInt());
  }
}

void Lamp::doSubscriptions(PubSubClient& mqttClient)
{
  mqttClient.subscribe((getPrefix() + MQTT_TPC_SWITCH).c_str());
  sendStatus();
}

void Lamp::irmsgRecd(uint32_t code)
{
  if (code == IRREMOTE_LIGHT_ONOFF) toggle();
}

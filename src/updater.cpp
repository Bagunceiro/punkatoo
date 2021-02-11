#include <ArduinoJson.h>

#include "config.h"
#include "updater.h"

Updater::Updater(const String &devName)
{
  startCallback = NULL;
  endCallback   = NULL;
  nullCallback  = NULL;
  failCallback  = NULL;
}

Updater::~Updater()
{
}

t_httpUpdate_return Updater::systemUpdate(const String &server, const uint16_t port, const String &image, const String& ver)
{
  // ToDo: Check out the version (ver) to see we dont already have it

  WiFiClient client;

  if (startCallback != NULL)
  {
    startCallback();
  }

  httpUpdate.rebootOnUpdate(false);

  t_httpUpdate_return ret = httpUpdate.update(client, server, port, image);

  switch (ret)
  {
  case HTTP_UPDATE_FAILED:
    serr.printf("HTTP_UPDATE_FAILED Error (%d): %s\n", httpUpdate.getLastError(), httpUpdate.getLastErrorString().c_str());
    if (failCallback != NULL) failCallback();
    break;

  case HTTP_UPDATE_NO_UPDATES:
    serr.println("HTTP_UPDATE_NO_UPDATES");
    if (nullCallback != NULL) nullCallback();
    break;

  case HTTP_UPDATE_OK:
    serr.println("HTTP_UPDATE_OK");
    if (endCallback != NULL) endCallback();
    break;
  }
  return ret;
}

void Updater::onStart(void(*callback)())
{
  startCallback = callback;
}

void Updater::onEnd(void(*callback)())
{
  endCallback = callback;
}

void Updater::onNone(void(*callback)())
{
  nullCallback = callback;
}

void Updater::onFail(void(*callback)())
{
  failCallback = callback;
}
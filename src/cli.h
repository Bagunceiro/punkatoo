#pragma once

#include <Arduino.h>
#include <WiFi.h>
#include <vector>

#include "p2task.h"

typedef std::vector<String> stringArray;

class CLITask : public P2Task
{
    public:
    CLITask(const char* name);
    virtual ~CLITask();
    bool operator()() override;
    void init();

    private:
    WiFiServer cliServer;
    WiFiClient cliClient;
    String getCommand();
    int parse(const char *line, stringArray &argv);
    int execute(stringArray argv);
    int upload(stringArray argv);
    void progress(const size_t comp, const size_t total);
    static void progcb(const size_t comp, const size_t total, void* obj);
    String error;
};
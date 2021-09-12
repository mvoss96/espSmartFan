#include "wifiFunctions.h"
#include "fan.h"

#include <ESPAsync_WiFiManager.h>
#include <DNSServer.h>
#include <ESP_DoubleResetDetector.h>
#include <AsyncElegantOTA.h>
#include <LittleFS.h>

DoubleResetDetector drd(DRD_TIMEOUT, DRD_ADDRESS);
AsyncWebServer webServer(80);
DNSServer dnsServer;
bool initialConfig = false;
extern Fan fan;

void wifiSetup1()
{
    WiFi.persistent(true);
    Serial.println(ARDUINO_BOARD);
    Serial.println(ESP_ASYNC_WIFIMANAGER_VERSION);
    if (WiFi.SSID() == "")
    {
        Serial.println(F("No AP credentials"));
        initialConfig = true; //no config: need for setup
    }
}

void wifiSetup2()
{
    WiFi.mode(WIFI_STA);
    if (WiFi.status() != WL_CONNECTED)
    {
        WiFi.reconnect();
    }
    unsigned long startedAt = millis();
    Serial.print(F("After waiting "));
    int connRes = WiFi.waitForConnectResult();
    float waited = (millis() - startedAt);
    Serial.print(waited / 1000);
    Serial.print(F(" secs , Connection result is "));
    Serial.println(connRes);
    if (WiFi.status() != WL_CONNECTED)
    {
        Serial.println(F("Failed to connect"));
    }
    else
    {
        Serial.print(F("Local IP: "));
        Serial.println(WiFi.localIP());
    }
}

void checkReset()
{
    if (drd.detectDoubleReset())
    {
        Serial.println(F("DRD"));
        initialConfig = true;
    }
    if (initialConfig)
    {
        Serial.println(F("Starting Config Portal"));
        ESPAsync_WiFiManager ESPAsync_wifiManager(&webServer, &dnsServer, DEVICE_NAME);
        ESPAsync_wifiManager.setConfigPortalTimeout(120);
        if (!ESPAsync_wifiManager.startConfigPortal(DEVICE_NAME))
        {
            Serial.println(F("Not connected to WiFi"));
        }
        else
        {
            Serial.println(F("connected"));
        }
    }
}

bool handleBodyPower(AsyncWebServerRequest *request, uint8_t *data, size_t len)
{
    Serial.printf("[REQUEST POWER]\t%.*s\r\n", len, (const char *)data);
    if (memcmp(data, "off", 3) == 0 || memcmp(data, "OFF", 3) == 0 || memcmp(data, "0", 1) == 0)
    {
        fan.setPower(false);
        return 1;
    }
    else if (memcmp(data, "on", 2) == 0 || memcmp(data, "ON", 2) == 0 || memcmp(data, "1", 1) == 0)
    {
        fan.setPower(true);
        return 1;
    }
    else
    {
        return 0;
    }
}

bool handleBodySweep(AsyncWebServerRequest *request, uint8_t *data, size_t len)
{
    Serial.printf("[REQUEST SWEEP]\t%.*s\r\n", len, (const char *)data);
    if (memcmp(data, "off", 3) == 0 || memcmp(data, "OFF", 3) == 0)
    {
        fan.setSweep(false);
        return 1;
    }
    else if (memcmp(data, "on", 2) == 0 || memcmp(data, "ON", 2) == 0)
    {
        fan.setSweep(true);
        return 1;
    }
    else
    {
        return 0;
    }
}

bool handleBodyBreathe(AsyncWebServerRequest *request, uint8_t *data, size_t len)
{
    Serial.printf("[REQUEST BREATHE]\t%.*s\r\n", len, (const char *)data);
    if (memcmp(data, "off", 3) == 0 || memcmp(data, "OFF", 3) == 0)
    {
        fan.setBreathe(false);
        return 1;
    }
    else if (memcmp(data, "on", 2) == 0 || memcmp(data, "ON", 2) == 0)
    {
        fan.setBreathe(true);
        return 1;
    }
    else
    {
        return 0;
    }
}

bool handleBodySpeed(AsyncWebServerRequest *request, uint8_t *data, size_t len)
{
    data[len] = '\0';
    Serial.printf("[REQUEST SPEED]\t%.*s\r\n", len, (const char *)data);
    int val = atoi((char *)data);
    Serial.printf("parsed: %d\n", val);
    if (val >= 0 && val <= 255)
    {
        fan.setSpeed(val);
        return 1;
    }
    else
    {
        return 0;
    }
}

bool handleBodyAngle(AsyncWebServerRequest *request, uint8_t *data, size_t len)
{
    data[len] = '\0';
    Serial.printf("[REQUEST ANGLE]\t%.*s\r\n", len, (const char *)data);
    int val = atoi((char *)data);
    Serial.printf("parsed: %d\n", val);
    if (val >= 0 && val <= 255)
    {
        fan.setAngle(val);
        return 1;
    }
    else
    {
        return 0;
    }
}

bool handleBodyTimer(AsyncWebServerRequest *request, uint8_t *data, size_t len)
{
    data[len] = '\0';
    Serial.printf("[REQUEST TIMER]\t%.*s\r\n", len, (const char *)data);
    int val = atoi((char *)data);
    Serial.printf("parsed: %d\n", val);
    fan.setTimer(val);
    return 1;
}

void startServer()
{
    webServer.on("/", HTTP_GET, [](AsyncWebServerRequest *request)
                 { request->redirect("/update"); });
    webServer.on("/power", HTTP_GET, [](AsyncWebServerRequest *request)
                 { request->send(200, "text/plain", (fan.getPower()) ? "on" : "off"); });
    webServer.on("/sweepMode", HTTP_GET, [](AsyncWebServerRequest *request)
                 { request->send(200, "text/plain", (fan.getSweep()) ? "on" : "off"); });
    webServer.on("/breatheMode", HTTP_GET, [](AsyncWebServerRequest *request)
                 { request->send(200, "text/plain", (fan.getBreathe()) ? "on" : "off"); });
    webServer.on("/speed", HTTP_GET, [](AsyncWebServerRequest *request)
                 {
                     char msg[4];
                     sprintf(msg, "%d", fan.getSpeed());
                     request->send(200, "text/plain", msg);
                 });
    webServer.on("/angle", HTTP_GET, [](AsyncWebServerRequest *request)
                 {
                     char msg[4];
                     sprintf(msg, "%d", fan.getAngle());
                     request->send(200, "text/plain", msg);
                 });
    webServer.on("/timer", HTTP_GET, [](AsyncWebServerRequest *request)
                 {
                     char msg[10];
                     sprintf(msg, "%d", fan.getTimer());
                     request->send(200, "text/plain", msg);
                 });
    webServer.on("/rpm", HTTP_GET, [](AsyncWebServerRequest *request)
                 {
                     char msg[10];
                     sprintf(msg, "%d", fan.getRpm());
                     request->send(200, "text/plain", msg);
                 });
    webServer.on("/status", HTTP_GET, [](AsyncWebServerRequest *request)
                 {
                     char msg[100];
                     sprintf(msg, "power:%d\nsweep:%d\nbreathe:%d\nspeed:%d\nangle:%d\ntimer:%d\nrpm:%d",
                             fan.getPower(), fan.getSweep(), fan.getBreathe(), fan.getSpeed(), fan.getAngle(), fan.getTimer(), fan.getRpm());
                     request->send(200, "text/plain", msg);
                 });
    webServer.onNotFound([](AsyncWebServerRequest *request)
                         { request->send(404, "text/plain", "Not found"); });

    webServer.onRequestBody([](AsyncWebServerRequest *request, uint8_t *data, size_t len, size_t index, size_t total)
                            {
                                if (request->url() == "/power")
                                {
                                    if (!handleBodyPower(request, data, len))
                                        request->send(400, "text/plain", "");
                                    request->send(200, "text/plain", "ok");
                                }
                                if (request->url() == "/sweepMode")
                                {
                                    if (!handleBodySweep(request, data, len))
                                        request->send(400, "text/plain", "");
                                    request->send(200, "text/plain", "ok");
                                }
                                if (request->url() == "/breatheMode")
                                {
                                    if (!handleBodyBreathe(request, data, len))
                                        request->send(400, "text/plain", "");
                                    request->send(200, "text/plain", "ok");
                                }
                                else if (request->url() == "/speed")
                                {
                                    if (!handleBodySpeed(request, data, len))
                                        request->send(400, "text/plain", "");
                                    request->send(200, "text/plain", "ok");
                                }
                                else if (request->url() == "/angle")
                                {
                                    if (!handleBodyAngle(request, data, len))
                                        request->send(400, "text/plain", "");
                                    request->send(200, "text/plain", "ok");
                                }
                                else if (request->url() == "/timer")
                                {
                                    if (!handleBodyTimer(request, data, len))
                                        request->send(400, "text/plain", "");
                                    request->send(200, "text/plain", "ok");
                                }
                            });
    AsyncElegantOTA.begin(&webServer); // Start ElegantOTA
    webServer.begin();
    Serial.println("OTA-server started");
}
void wifiLoop()
{
    drd.loop(); //doubleReset detection background loop
    AsyncElegantOTA.loop();
}

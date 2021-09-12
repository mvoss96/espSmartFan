#include "wifiFunctions.h"

#include <ESPAsync_WiFiManager.h>
#include <DNSServer.h>
#include <ESP_DoubleResetDetector.h>
#include <AsyncElegantOTA.h>
#include <LittleFS.h>

DoubleResetDetector drd(DRD_TIMEOUT, DRD_ADDRESS);
AsyncWebServer webServer(80);
DNSServer dnsServer;
bool initialConfig = false;

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

void startServer()
{
    webServer.on("/", HTTP_GET, [](AsyncWebServerRequest *request)
                 { request->redirect("/update"); });
    webServer.on("/power", HTTP_GET, [](AsyncWebServerRequest *request)
                 { request->send(200, "text/plain", (true) ? "on" : "off"); });
    AsyncElegantOTA.begin(&webServer); // Start ElegantOTA
    webServer.begin();
    Serial.println("OTA-server started");
}
void wifiLoop()
{
    drd.loop(); //doubleReset detection background loop
    AsyncElegantOTA.loop();
}

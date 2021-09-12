#pragma once

#define USE_LITTLEFS true
#define ESP_DRD_USE_LITTLEFS true
#define DRD_TIMEOUT 10
#define DRD_ADDRESS 0
#define DEVICE_NAME "espSmartFan"
#define REQUIRE_ONE_SET_SSID_PW true

void wifiSetup1();
void wifiSetup2();
void checkReset();
void startServer();
void wifiLoop();
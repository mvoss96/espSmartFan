#include <Arduino.h>
#include "fan.h"
#include "wifiFunctions.h"
#include "LittleFS.h" 

Fan fan;

void FsInfo(){
    Serial.println(F("Inizializing FS..."));
    if (LittleFS.begin()){
        Serial.println(F("done."));
    }else{
        Serial.println(F("fail."));
    }
 
    // To format all space in LittleFS
    // LittleFS.format()
 
    // Get all information of your LittleFS
    FSInfo fs_info;
    LittleFS.info(fs_info);
 
    Serial.println("File sistem info.");
 
    Serial.print("Total space:      ");
    Serial.print(fs_info.totalBytes);
    Serial.println("byte");
 
    Serial.print("Total space used: ");
    Serial.print(fs_info.usedBytes);
    Serial.println("byte");
 
    Serial.print("Block size:       ");
    Serial.print(fs_info.blockSize);
    Serial.println("byte");
 
    Serial.print("Page size:        ");
    Serial.print(fs_info.totalBytes);
    Serial.println("byte");
 
    Serial.print("Max open files:   ");
    Serial.println(fs_info.maxOpenFiles);
 
    Serial.print("Max path lenght:  ");
    Serial.println(fs_info.maxPathLength);
 
    Serial.println();
 
    // Open dir folder
    Dir dir = LittleFS.openDir("/");
    // Cycle all the content
    while (dir.next()) {
        // get filename
        Serial.print(dir.fileName());
        Serial.print(" - ");
        // If element have a size display It else write 0
        if(dir.fileSize()) {
            File f = dir.openFile("r");
            Serial.println(f.size());
            f.close();
        }else{
            Serial.println("0");
        }
    }
}

void setup()
{
    Serial.begin(115200);
    Serial.println("\n\nespSmartFan");
    Serial.print(F("\nStarting up... "));
    FsInfo();
    randomSeed(millis());
    wifiSetup1();
    checkReset();
    delay(100);
    wifiSetup2();
    startServer();
}

void loop()
{
    wifiLoop();
    fan.run();
}

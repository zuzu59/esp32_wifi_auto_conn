// Voir comment se connecter automatiquement aux WIFI connus
//
// ATTENTION, ce code a été testé sur un esp32-c3. Pas testé sur les autres boards !
//
#define zVERSION  "zf240528.1530"
#define zHOST     "wifiauto1"            // ATTENTION, tout en minuscule !

/*
Utilisation:

Astuce:



Installation:

Pour les esp32-c3 super mini, il faut:
 * choisir comme board ESP32C3 Dev Module
 * disabled USB CDC On Boot et utiliser USBSerial. au lieu de Serial. pour la console !
 * changer le schéma de la partition à Minimal SPIFFS (1.9MB APP with OTA/190kB SPIFFS)

Pour le WiFiManager, il faut installer cette lib depuis le lib manager sur Arduino:
https://github.com/tzapu/WiFiManager


Sources:
https://www.espboards.dev/blog/esp32-inbuilt-temperature-sensor
https://forum.fritzing.org/t/need-esp32-c3-super-mini-board-model/20561
https://www.aliexpress.com/item/1005006005040320.html
https://randomnerdtutorials.com/esp32-useful-wi-fi-functions-arduino
https://dronebotworkshop.com/wifimanager/
https://github.com/universam1/iSpindel/tree/master
https://components101.com/sensors/mpu6050-module
https://github.com/electroniccats/mpu6050
https://github.com/ElectronicCats/mpu6050/blob/master/examples/MPU6050_raw/MPU6050_raw.ino
https://lastminuteengineers.com/esp32-ota-web-updater-arduino-ide/
https://github.com/dawidchyrzynski/arduino-home-assistant/blob/main/examples/sensor-integer/sensor-integer.ino
https://chat.mistral.ai/    pour toute la partie API REST ᕗ
*/




// #define DEBUG true
// #undef DEBUG



// General
const int ledPin = 8;    // the number of the LED pin
const int buttonPin = 9;  // the number of the pushbutton pin
float rrsiLevel = 0;      // variable to store the RRSI level

int zDelay1Interval= 30000;        // Délais en mili secondes pour le zDelay1


float sensorValue1 = 0;  // variable to store the value coming from the sensor 1
float sensorValue2 = 0;  // variable to store the value coming from the sensor 2
float sensorValue3 = 0;  // variable to store the value coming from the sensor 3
float sensorValue4 = 0;  // variable to store the value coming from the sensor 4
float sensorValue5 = 0;  // variable to store the value coming from the sensor 5
#define TEMP_CELSIUS 0


// Solar Pulse
#include "zSonarpulse.h"


// WIFI
#include "zWifi.h"


// OTA WEB server
#include "otaWebServer.h"



void setup() {
  // Pulse deux fois pour dire que l'on démarre
  pinMode(ledPin, OUTPUT);
  digitalWrite(ledPin, LOW); delay(zSonarPulseOn); digitalWrite(ledPin, HIGH); delay(zSonarPulseOff);
  digitalWrite(ledPin, LOW); delay(zSonarPulseOn); digitalWrite(ledPin, HIGH); delay(zSonarPulseOff);
  delay(zSonarPulseWait);



  // start serial console
  USBSerial.begin(19200);
  USBSerial.setDebugOutput(true);       //pour voir les messages de debug des libs sur la console série !
  delay(3000);                          //le temps de passer sur la Serial Monitor ;-)
  USBSerial.println("\n\n\n\n**************************************\nCa commence !"); USBSerial.println(zHOST ", " zVERSION);




  // start WIFI
  zStartWifi();


  // start OTA server
  otaWebServer();



  // go go go
  USBSerial.println("\nC'est parti !\n");


}


void loop() {

  USBSerial.println("Coucou...");

  // Délais non bloquant pour le sonarpulse et l'OTA
  zDelay1(zDelay1Interval);
}





// Délais non bloquant pour le sonarpulse et l'OTA
void zDelay1(long zDelayMili){
  long zDelay1NextMillis = zDelayMili + millis(); 
  while(millis() < zDelay1NextMillis ){
    // OTA loop
    server.handleClient();
    // Un petit coup sonar pulse sur la LED pour dire que tout fonctionne bien
    sonarPulse();
  }
}


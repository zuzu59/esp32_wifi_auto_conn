// zf240527.1446


// WIFI
#include <WiFi.h>
#include <HTTPClient.h>
#include <WiFiManager.h>
#include "secrets.h"
WiFiClient client;
HTTPClient http;

static void ConnectWiFi() {
  WiFi.mode(WIFI_STA);   
// Pour le wifi manager
//--------
    // WiFiManager wm;
    // bool res;
    // res = wm.autoConnect("esp32_wifi_config",""); // password protected ap
    // if(!res) {
    //     USBSerial.println("Failed to connect");
    //     // ESP.restart();
    // }
//--------
// Pour le wifi avec secrets.h
//--------
  USBSerial.printf("WIFI_SSID: %s\nWIFI_PASSWORD: %s\n", WIFI_SSID, WIFI_PASSWORD);
  WiFi.mode(WIFI_STA); //Optional
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
//--------
  WiFi.setTxPower(WIFI_POWER_8_5dBm);  //c'est pour le Lolin esp32-c3 mini V1 ! https://www.wemos.cc/en/latest/c3/c3_mini_1_0_0.html
  int txPower = WiFi.getTxPower();
  USBSerial.print("TX power: ");
  USBSerial.println(txPower);
  USBSerial.println("Connecting");

  long zWifiTiemeout = 10000 + millis(); 

  while(WiFi.status() != WL_CONNECTED){
    USBSerial.print("."); delay(100);
    if(millis() > zWifiTiemeout ){
      USBSerial.println("\nOn a un problème avec le WIFI !");
      zWifiTiemeout = 1000 + millis();
      delay(200);
      USBSerial.flush(); 
      esp_deep_sleep_start();
    }
  }
  USBSerial.println("\nConnected to the WiFi network");
  USBSerial.print("SSID: ");
  USBSerial.println(WiFi.SSID());
  USBSerial.print("RSSI: ");
  USBSerial.println(WiFi.RSSI());
  USBSerial.print("IP: ");
  USBSerial.println(WiFi.localIP());
}


// // start WIFI
// void zStartWifi(){
//     digitalWrite(ledPin, HIGH);
//     USBSerial.println("Connect WIFI !");
//     ConnectWiFi();
//     digitalWrite(ledPin, LOW);
// }




// #include <WiFi.h>
#include <vector>

struct WifiCredentials {
  String ssid;
  String password;
};

std::vector<WifiCredentials> wifi_creds;



void connectToBestWifi() {
  int best_rssi = -1000;
  String best_ssid;
  String best_password;

  // Scanner les réseaux Wi-Fi
  WiFi.mode(WIFI_STA);
  WiFi.disconnect();
  delay(100);
  int n = WiFi.scanNetworks();
  USBSerial.print("Number SSID scanned: ");
  USBSerial.println(n);
  for (int i = 0; i < n; i++) {
    USBSerial.print("SSID scanned: ");
    USBSerial.println(WiFi.SSID(i));
    USBSerial.print("RSSI: ");
    USBSerial.println(WiFi.RSSI(i));
    for (const auto &cred : wifi_creds) {

      if (WiFi.SSID(i) == cred.ssid) {
        int rssi = WiFi.RSSI(i);
        if (rssi > best_rssi) {
          best_rssi = rssi;
          best_ssid = cred.ssid;
          best_password = cred.password;
        }
      }
    }
  }

  // Se connecter au réseau Wi-Fi avec le meilleur RSSI
  if (!best_ssid.isEmpty()) {
    WiFi.begin(best_ssid.c_str(), best_password.c_str());
    USBSerial.print("Connecting to ");
    USBSerial.println(best_ssid);
    int connAttempts = 0;
    while (WiFi.status() != WL_CONNECTED && connAttempts < 10) {
      delay(500);
      USBSerial.print(".");
      connAttempts++;
    }
    USBSerial.println("");
    if (WiFi.status() == WL_CONNECTED) {
      USBSerial.print("Connected to ");
      USBSerial.println(best_ssid);
      USBSerial.print("IP address: ");
      USBSerial.println(WiFi.localIP());
    } else {
      USBSerial.println("Failed to connect");
    }
  } else {
    USBSerial.println("No known networks found");
  }
}

void zStartWifi(){
  // Ajouter vos informations d'identification Wi-Fi au vecteur
  WifiCredentials creds1 = {WIFI_SSID1, WIFI_PASSWORD1};
  WifiCredentials creds2 = {WIFI_SSID2, WIFI_PASSWORD2};
  WifiCredentials creds3 = {WIFI_SSID3, WIFI_PASSWORD3};
  WifiCredentials creds4 = {WIFI_SSID4, WIFI_PASSWORD4};
  WifiCredentials creds5 = {WIFI_SSID5, WIFI_PASSWORD5};
  // Ajoutez autant de réseaux que vous le souhaitez
  wifi_creds.push_back(creds1);
  wifi_creds.push_back(creds2);
  wifi_creds.push_back(creds3);
  wifi_creds.push_back(creds4);
  wifi_creds.push_back(creds5);
  // ...
  connectToBestWifi();
}



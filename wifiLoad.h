#pragma once
// Megakadályozza, hogy ugyanazt a fejlécet egy fordítási egységben többször beillessze a fordító.
#include <WiFi.h>
#include <HTTPClient.h>
#include <HTTPUpdate.h>
#include "config.h"
#include "secrets.h"

const int wifiTimeout = 20;     // Próbálkozási idő hálózatonként (másodperc)

struct WifiCredentials
{
  uint32_t magic;
  char ssid[33];
  char password[65];
};

RTC_DATA_ATTR WifiCredentials rtcWifiCredentials = {};
const uint32_t wifiCredentialsMagic = 0x57494649;

const char *fwImageURL = "http://zeus.jedlik.eu:8000/timetable.ino.bin";
const char *fwVersionURL = "http://zeus.jedlik.eu:8000/version";
// const char* fwImageURL = "https://timetable-jedlik.koyeb.app/update.bin";
// const char* fwVersionURL = "https://timetable-jedlik.koyeb.app/version";


void check_OTA()
{
  HTTPClient http;
  if (http.begin(fwVersionURL))
  {
    int httpCode = http.GET();
    if (httpCode == HTTP_CODE_OK || httpCode == HTTP_CODE_MOVED_PERMANENTLY)
    {
      String newFWVersion = http.getString();
      float newVersion = newFWVersion.toFloat();
      DEBUG_PRINT("A régi verzió: ");
      DEBUG_PRINTLN(FW_VERSION);
      DEBUG_PRINT("Az új verzió: ");
      DEBUG_PRINTLN(newVersion);
      if (newVersion > FW_VERSION)
      {
        delay(100);
        DEBUG_PRINTLN(F("Frissítés folyamatban..."));
        WiFiClient updateClient;
        t_httpUpdate_return ret = httpUpdate.update(updateClient, fwImageURL); // Telepíti a letöltött timetable.ino.bin firmware-t, majd újraindítja a mikrokontrollert
        // Az újraindulást követően a soros monitoron megjelenik a "ets Jun 8 2016 00:22:57" üzenet. Ez a ROM bejelentkezési üzenete; ez az első dolog, ami kinyomtatódik, miután az ESP32 bekapcsol és a CPU elindul.
      }
    }
  } // end if http.begin
} // end check_OTA()

bool tryWifiNetwork(const char *networkSsid, const char *networkPassword)
{
  DEBUG_PRINT(F("Csatlakozás a(z) "));
  DEBUG_PRINT(networkSsid);
  DEBUG_PRINTLN(F(" hálózathoz..."));

  WiFi.begin(networkSsid, networkPassword);

  int szamlalo = 0;
  int maxProba = wifiTimeout * 2;
  while (WiFi.status() != WL_CONNECTED && szamlalo < maxProba)
  {
    delay(500);
    Serial.print(F("."));
    szamlalo++;
  }

  if (WiFi.status() != WL_CONNECTED)
  {
    DEBUG_PRINTLN();
    DEBUG_PRINT(F("Nem sikerült csatlakozni a(z) "));
    DEBUG_PRINT(networkSsid);
    DEBUG_PRINTLN(F(" hálózathoz."));
    WiFi.disconnect();
    return false;
  }

  DEBUG_PRINTLN();
  DEBUG_PRINT(F("Sikeres csatlakozás! IP cím: "));
  DEBUG_PRINTLN(WiFi.localIP());

  if (strlen(networkSsid) < sizeof(rtcWifiCredentials.ssid) &&
      strlen(networkPassword) < sizeof(rtcWifiCredentials.password))
  {
    strncpy(rtcWifiCredentials.ssid, networkSsid, sizeof(rtcWifiCredentials.ssid) - 1);
    strncpy(rtcWifiCredentials.password, networkPassword, sizeof(rtcWifiCredentials.password) - 1);
    rtcWifiCredentials.magic = wifiCredentialsMagic;
  }

  return true;
}

void wifiLoad()
{
  WiFi.setAutoReconnect(true);
  bool connected = false;

  if (rtcWifiCredentials.magic == wifiCredentialsMagic &&
      rtcWifiCredentials.ssid[0] != '\0' &&
      rtcWifiCredentials.ssid[sizeof(rtcWifiCredentials.ssid) - 1] == '\0' &&
      rtcWifiCredentials.password[sizeof(rtcWifiCredentials.password) - 1] == '\0')
  {
    connected = tryWifiNetwork(rtcWifiCredentials.ssid, rtcWifiCredentials.password);
  }

  // Próbálkozás az összes WiFi hálózattal sorban
  for (int i = 0; i < wifiNetworkCount && !connected; i++)
  {
    if (rtcWifiCredentials.magic == wifiCredentialsMagic &&
      strcmp(ssid[i], rtcWifiCredentials.ssid) == 0 &&
      strcmp(password[i], rtcWifiCredentials.password) == 0)
    {
      continue;
    }

    connected = tryWifiNetwork(ssid[i], password[i]);
    if (!connected) delay(1000);
  }

  // Ha egyik hálózathoz sem sikerült csatlakozni
  if (!connected)
  {
    DEBUG_PRINTLN(F("HIBA: Egyetlen WiFi hálózathoz sem sikerült csatlakozni!"));
    errorCode = 1;
    return;
  }

  DEBUG_PRINTLN();
  DEBUG_PRINT(F("MAC Address: "));
  String mac = WiFi.macAddress();
  DEBUG_PRINTLN(mac);
  if (mac == "24:DC:C3:81:FD:F4")
    Terem = 102;
  else if (mac == "24:DC:C3:8D:71:F8")
    Terem = 103;
  else if (mac == "38:18:2B:61:B7:70")
    Terem = 104;
  else if (mac == "38:18:2B:17:A3:A4")
    Terem = 106;
  else if (mac == "08:D1:F9:71:2E:A0")
    Terem = 108;
  else if (mac == "38:18:2B:69:F9:74")
    Terem = 110;
  else if (mac == "38:18:2B:18:62:18")
    Terem = 112;
  else if (mac == "3C:8A:1F:54:DF:10")
    Terem = 114;
  else if (mac == "94:54:C5:60:06:98")
    Terem = 115;
  else if (mac == "3C:8A:1F:54:1C:84")
    Terem = 116;
  else if (mac == "3C:8A:1F:54:03:28")
    Terem = 117;
  else if (mac == "3C:8A:1F:54:29:0C")
    Terem = 202;
  else if (mac == "3C:8A:1F:51:75:F8")
    Terem = 203;
  else if (mac == "38:18:2B:6A:F6:10")
    Terem = 204;
  else if (mac == "24:DC:C3:83:BE:B4")
    Terem = 205;
  else if (mac == "38:18:2B:69:E7:A0")
    Terem = 207;
  else if (mac == "38:18:2B:18:34:10")
    Terem = 208;
  else if (mac == "24:DC:C3:85:04:48")
    Terem = 209;
  else if (mac == "38:18:2B:14:58:5C")
    Terem = 210;
  else if (mac == "38:18:2B:19:59:F8")
    Terem = 212;
  else if (mac == "3C:8A:1F:54:96:20")
    Terem = 302;
  else if (mac == "3C:8A:1F:51:5F:0C")
    Terem = 303;
  // ... ide jön a többi terem majd
  else
    Terem = 41; // TESZTELÉSHEZ

  check_OTA();
}

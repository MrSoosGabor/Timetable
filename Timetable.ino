#include "config.h"
#include "wifiLoad.h"
#include "utilities.h"
#include "periodsLoad.h"
#include "cardsLoad.h"
#include "drawing.h"
#include "sendEmail.h"

void setup() {
  Serial.begin(115200);
  pinMode(EINK_POWER_PIN, OUTPUT);
  float accuLevel = readVbat();  // Az akkut ébredés után, a WiFi bekapcsolása előtt mérjük, így terhelés nélküli (nyugalmi) feszültséget kapunk
  DEBUG_PRINT("Akku szint: ");
  DEBUG_PRINTLN(accuLevel);
  if (accuLevel < urgentAccuLevel) {  // Ha az akku szintje kritikus, akkor nem próbálkozik a WiFi-vel, hanem figyelmeztető üzenetet ír ki a kijelzőre, emailt küld, majd mélyalvásba teszi a lapkát
    digitalWrite(EINK_POWER_PIN, HIGH);
    drawInit();
    const char *warningMessage = "Tölteni kell az akkut!!!";
    do {
      u8g2Fonts.setFont(u8g2_font_helvB18_tf);
      int textWidth = u8g2Fonts.getUTF8Width(warningMessage);
      u8g2Fonts.setCursor((display.width() - textWidth) / 2, display.height() / 2);
      u8g2Fonts.print(warningMessage);
    } while (display.nextPage());
    display.hibernate();
    display.end();
    digitalWrite(EINK_POWER_PIN, LOW);

    emailSend(accuLevel);
    esp_deep_sleep_start();  //  Ébresztés nélkül mélyalvásba tesszük a lapkát, így nem próbálkozik újra és újra a WiFi-vel, és nem meríti tovább az akkut.
  }

  wifiLoad();   // A Wifi kapcsolat beállítása, majd a frissítések ellenőrzése
  ++bootCount;  // A bootolások száma, hogy tudjuk ha nem sikerül egyből frissíteni az aktuális nap órarendjét (frissítések után visszaáll 1-re), illetve hogy sikerült-e egyből frissíteni az órarendet

  if (errorCode != 1) {            // Ha az errorCode = 1, akkor nem jött létre WiFi kapcsolat
    setClock();                    // Az aktuális dátum és idő lekérése egy időszerverről
    periodusSzam = PeriodsLoad();  // Betölti az aktuális nap időrendjét (lehet, hogy rövidített órák vannak) a /ringsystem végpontról a periodusok listába
    kartyaSzam = CardsLoad();      // Betölti az aktuális nap óráit a /cards végpontról a cards listába
    DEBUG_PRINT("Peródusok száma: ");
    DEBUG_PRINTLN(periodusSzam);
    DEBUG_PRINT("Kártyák száma: ");
    DEBUG_PRINTLN(kartyaSzam);

    //timeToSleep = (360-15);         // TESZTELÉSHEZ: 6 percenként frissít, a frissítés kb. 15 mp-ig tart  //
    setTimeToSleep();  // Beállítja a szükséges alvási időtartamot

    if (timeToSleep > 600 && !visszaAlszik) {  // Ha valamilyen hiba miatt újra kell frissíteni, akkor nem küldünk emailt
      // analogRead(ANALOG_PIN);  // 3,3V -> 4095 (12 bit)
      DEBUG_PRINT("Akku szint: ");
      DEBUG_PRINTLN(accuLevel);
      if (accuLevel < warningAccuLevel) {
        emailSend(accuLevel);  // Elem merülés esetén figyelmeztető emailt küld
        errorCode = 4;
      }
    }
  }

  WiFi.mode(WIFI_OFF);  // A WiFi kikapcsolása a kijelző frissítése előtt
  drawing();            // Kirajzolja az E-ink kijelzőre az aktuális nap órarendjét, illetve ha hiba volt, akkor a hibát

  esp_sleep_enable_timer_wakeup(timeToSleep * uS_TO_S_FACTOR);  // Beállítja a következő ébredésig visszalévő időt
  char msg[80];
  sprintf(msg, "Az ébredésig hátralévő idő: %lu másodperc", timeToSleep);
  DEBUG_PRINTLN(msg);
  DEBUG_PRINTLN("Most elalszom...");
  Serial.flush();
  esp_deep_sleep_start();  // Mélyalvásba küldi a mikrokontrollert
}

void loop(void) {}

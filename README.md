# Timetable

A Győri Jedlik Ányos Technikum termeihez tartozó órarend kijelzők Arduino nyelvű programja.

## Fordítás

Windows alatt futtasd a projekt gyökeréből a `build.ps1` szkriptet: `.\build.ps1`.

A buildszkript az Arduino CLI számára ideiglenesen elkülönített felhasználói könyvtárat állít be, így a globálisan telepített teljes GxEPD2 helyett a projekt `src/` könyvtárában lévő, 750c panelhez szükséges szűkített forráskészlet fordul. A GxEPD2 1.6.9 eredeti licence és könyvtár-metaadata a `third_party/GxEPD2/` alatt található. Az ArduinoJson, Adafruit GFX, Adafruit BusIO, U8g2_for_Adafruit_GFX, ESP_SSLClient és ESP Mail Client könyvtáraknak a szokásos Arduino sketchbook `libraries/` mappájában kell telepítve lenniük. Egyedi sketchbook útvonalhoz használd a `build.ps1 -SketchbookDirectory 'D:\Arduino'` formát.

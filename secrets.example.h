#pragma once  // Megakadályozza, hogy ugyanazt a fejlécet egy fordítási egységben többször beillessze a fordító.

// Másold ezt a fájlt secrets.h néven, és töltsd ki a saját (cserélt) hitelesítő adataiddal.
// A secrets.h-t a Git figyelmen kívül hagyja, soha ne commitold!
static const char *ssid[] = {"", "", ""};
static const char *password[] = {"", "", ""};
static const int wifiNetworkCount = 3;  // A ténylegesen használt hálózatok száma

#define SENDER_EMAIL ""      // Gmail cím, amelyről a figyelmeztető emailt küldjük
#define SENDER_PASSWORD ""   // A Gmail fiók alkalmazásjelszava
#define RECIPIENT_EMAIL ""   // A figyelmeztető email címzettje

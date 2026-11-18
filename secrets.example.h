#pragma once  // Megakadályozza, hogy ugyanazt a fejlécet egy fordítási egységben többször beillessze a fordító.

// Másold ezt a fájlt secrets.h néven, és töltsd ki a saját (cserélt) hitelesítő adataiddal.
// A secrets.h-t a Git figyelmen kívül hagyja, soha ne commitold!
static const char *ssid[] = {"IoT005705", "jcs"};  // A WiFi hálózatok SSID-jei, amelyekhez csatlakozni szeretnél
static const char *password[] = {"WP1x5EUZgXWj2", "Jcsengo2023"};
static const int wifiNetworkCount = 2;  // A ténylegesen használt hálózatok száma

#define SENDER_EMAIL "jedlik.orarend@gmail.com"      // Gmail cím, amelyről a figyelmeztető emailt küldjük
#define SENDER_PASSWORD "duzj roob gwsm xdwn"   // A Gmail fiók alkalmazásjelszava
#define RECIPIENT_EMAIL "soos.gabor@jedlik.eu"   // A figyelmeztető email címzettje

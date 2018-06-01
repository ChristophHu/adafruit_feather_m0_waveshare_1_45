// includes
#include <SPI.h>                                    // standard Bibliothek
#include <epaper_1_54.h>                                    // Klasse epd
#include "imagesdata.h"                             // Logos

int counter = 0;

void setup() {                                      // Setup Code
    Serial.begin(115200);                           // Serial Monitor
    delay(2000);                                    // Pause 2 Sek.

    EPaper_1_54 EPD;                                // Deklarieren des Objekt der Klasse epd (epd.h)
    if (EPD.Init() != 0) {                          // Rückgabewert der Initialisierung
        Serial.println("EPaper_1_54 initialisiert");
        return;
    }
    EPD.ClearFrame();                               // Funktioniert 01.06.2018
    //EPD.DisplayFrame(DEMO_IMAGE_BLACK, DEMO_IMAGE_RED); // Ladebild laden
    delay(5000);                                    // Pause 2 Sek.
    
    Serial.println("Setup abgeschlossen");
}

void loop() {                                       // Schleife
    // Code in der Schleife
    /* 
        1. Würfel (1-6) abbilden
        2. Zufallszahlen abbilden
        3. Laden verschiedenster Frames nacheinander
    */
   counter++;
   Serial.println("Schleife: ");
   delay(2000);                                     // Pause 2 Sek.
}

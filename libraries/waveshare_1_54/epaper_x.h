#ifndef EPAPER_X
#define EPAPER_X

#include <Arduino.h>                            // Standard Bibliothek

#define PIN_RST             12                  // Pin Definition
#define PIN_DC              13
#define PIN_CS              11
#define PIN_BUSY            10

class EPaper_X {
    public:
    EPaper_X(void);
    ~EPaper_X(void);

    static int XInit(void);
    static void DigitalWrite(int pin, int value);
    static int  DigitalRead(int pin);
    static void DelayMs(unsigned int delaytime);
    static void SpiTransfer(unsigned char date);
};

#endif
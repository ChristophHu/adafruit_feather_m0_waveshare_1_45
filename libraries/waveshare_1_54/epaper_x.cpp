#include "epaper_x.h"
#include <SPI.h>

EPaper_X::EPaper_X() {};
EPaper_X::~EPaper_X() {};

void EPaper_X::DigitalWrite(int pin, int value) {
    digitalWrite(pin, value);
}

int EPaper_X::DigitalRead(int pin) {
    return digitalRead(pin);
}

void EPaper_X::DelayMs(unsigned int delaytime) {
    delay(delaytime);
}

void EPaper_X::SpiTransfer(unsigned char data) {
    digitalWrite(PIN_CS, LOW);
    SPI.transfer(data);
    digitalWrite(PIN_CS, HIGH);
}

int EPaper_X::XInit(void) {
    pinMode(PIN_CS, OUTPUT);
    pinMode(PIN_RST, OUTPUT);
    pinMode(PIN_DC, OUTPUT);
    pinMode(PIN_BUSY, INPUT);
    SPI.beginTransaction(SPISettings(2000000, MSBFIRST, SPI_MODE0));
    SPI.begin();
    return 0;
}
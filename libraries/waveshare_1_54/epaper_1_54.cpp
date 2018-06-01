#include <stdlib.h>
#include "epaper_1_54.h"

EPaper_1_54::~EPaper_1_54() {
};

EPaper_1_54::EPaper_1_54() {
    pin_rst = PIN_RST;
    pin_dc = PIN_DC;
    pin_cs = PIN_CS;
    pin_busy = PIN_BUSY;
    width = EPAPER_WIDTH;
    height = EPAPER_HEIGHT;
};

int EPaper_1_54::Init(void) {
    if (XInit() !=0) {
        return -1;
    }
    Reset();
    SendCommand(POWER_SETTING);
    SendData(0x07);
    SendData(0x00);
    SendData(0x08);
    SendData(0x00);
    SendCommand(BOOSTER_SOFT_START);
    SendData(0x07);
    SendData(0x07);
    SendData(0x07);
    SendCommand(POWER_ON);

    WaitUntilIdle();

    SendCommand(PANEL_SETTING);
    SendData(0xcf);
    SendCommand(VCOM_AND_DATA_INTERVAL_SETTING);
    SendData(0x17);
    SendCommand(PLL_CONTROL);
    SendData(0x39);
    SendCommand(TCON_RESOLUTION);
    SendData(0xC8);
    SendData(0x00);
    SendData(0xC8);
    SendCommand(VCM_DC_SETTING_REGISTER);
    SendData(0x0E);

    SetLutBw();
    SetLutRed();

    return 0;
}

void EPaper_1_54::SendCommand(unsigned char command) {
    DigitalWrite(pin_dc, LOW);
    SpiTransfer(command);
}

void EPaper_1_54::SendData(unsigned char data) {
    DigitalWrite(pin_dc, HIGH);
    SpiTransfer(data);
}

void EPaper_1_54::Reset(void) {
    DigitalWrite(pin_rst, LOW);         // gem. EPaper_X::digitanWrite
    DelayMs(200);                       // gem. EPaper_X::delayMs
    DigitalWrite(pin_rst, HIGH);        // -||-
    DelayMs(200);                       // -||-
}

void EPaper_1_54::Sleep() {
    SendCommand(VCOM_AND_DATA_INTERVAL_SETTING);
    SendData(0x17);
    SendCommand(VCM_DC_SETTING_REGISTER);
    SendData(0x00);
    SendCommand(POWER_SETTING);
    SendData(0x02);                     //gate switch to external
    SendData(0x00);
    SendData(0x00);
    SendData(0x00);
    WaitUntilIdle();
    SendCommand(POWER_OFF);
}

void EPaper_1_54::WaitUntilIdle(void) {
    while(DigitalRead(pin_busy) == 0) {      //0: busy, 1: idle
        DelayMs(100);
    }      
}

void EPaper_1_54::DrawPixel(uint x, uint y) {
    // Rotation

}

void EPaper_1_54::ClearFrame(void) {
    SendCommand(DATA_START_TRANSMISSION_1);
    for (uint i = 0; i < EPAPER_BUFFER_SIZE * 2; i++) {
        SendData(0xFF);
    }
    SendCommand(DATA_START_TRANSMISSION_2);
    for (uint i = 0; i < EPAPER_BUFFER_SIZE * 2; i++) {
        SendData(0xFF);
    }
    DelayMs(2);
    SendCommand(DISPLAY_REFRESH);
    WaitUntilIdle();
}

void EPaper_1_54::DisplayFrame(const unsigned char* frame_buffer_black, const unsigned char* frame_buffer_red) {
    unsigned char temp;  
    if (frame_buffer_black != NULL) {
        SendCommand(DATA_START_TRANSMISSION_1);
        DelayMs(2);
        for (int i = 0; i < this->width * this->height / 8; i++) {
            temp = 0x00;
            for (int bit = 0; bit < 4; bit++) {
                if ((pgm_read_byte(&frame_buffer_black[i]) & (0x80 >> bit)) != 0) {
                    temp |= 0xC0 >> (bit * 2);
                }
            }
            SendData(temp);
            temp = 0x00;
            for (int bit = 4; bit < 8; bit++) {
                if ((pgm_read_byte(&frame_buffer_black[i]) & (0x80 >> bit)) != 0) {
                    temp |= 0xC0 >> ((bit - 4) * 2);
                }
            }
            SendData(temp);
        }
        DelayMs(2);
    }
    if (frame_buffer_red != NULL) {
        SendCommand(DATA_START_TRANSMISSION_2);
        DelayMs(2);
        for (int i = 0; i < this->width * this->height / 8; i++) {
            SendData(pgm_read_byte(&frame_buffer_red[i]));
        }
        DelayMs(2);
    }
    SendCommand(DISPLAY_REFRESH);
    WaitUntilIdle();
}

void EPaper_1_54::SetLutBw(void) {
    unsigned int count;
    SendCommand(0x20);         //g vcom
    for(count = 0; count < 15; count++) {
        SendData(lut_vcom0[count]);
    } 
    SendCommand(0x21);        //g ww --
    for(count = 0; count < 15; count++) {
        SendData(lut_w[count]);
    } 
    SendCommand(0x22);         //g bw r
    for(count = 0; count < 15; count++) {
        SendData(lut_b[count]);
    } 
    SendCommand(0x23);         //g wb w
    for(count = 0; count < 15; count++) {
        SendData(lut_g1[count]);
    } 
    SendCommand(0x24);         //g bb b
    for(count = 0; count < 15; count++) {
        SendData(lut_g2[count]);
    } 
}

void EPaper_1_54::SetLutRed(void) {
    unsigned int count;     
    SendCommand(0x25);
    for(count = 0; count < 15; count++) {
        SendData(lut_vcom1[count]);
    } 
    SendCommand(0x26);
    for(count = 0; count < 15; count++) {
        SendData(lut_red0[count]);
    } 
    SendCommand(0x27);
    for(count = 0; count < 15; count++) {
        SendData(lut_red1[count]);
    } 
}

const unsigned char lut_vcom0[] =
{
    0x0E, 0x14, 0x01, 0x0A, 0x06, 0x04, 0x0A, 0x0A,
    0x0F, 0x03, 0x03, 0x0C, 0x06, 0x0A, 0x00
};

const unsigned char lut_w[] =
{
    0x0E, 0x14, 0x01, 0x0A, 0x46, 0x04, 0x8A, 0x4A,
    0x0F, 0x83, 0x43, 0x0C, 0x86, 0x0A, 0x04
};

const unsigned char lut_b[] = 
{
    0x0E, 0x14, 0x01, 0x8A, 0x06, 0x04, 0x8A, 0x4A,
    0x0F, 0x83, 0x43, 0x0C, 0x06, 0x4A, 0x04
};

const unsigned char lut_g1[] = 
{
    0x8E, 0x94, 0x01, 0x8A, 0x06, 0x04, 0x8A, 0x4A,
    0x0F, 0x83, 0x43, 0x0C, 0x06, 0x0A, 0x04
};

const unsigned char lut_g2[] = 
{
    0x8E, 0x94, 0x01, 0x8A, 0x06, 0x04, 0x8A, 0x4A,
    0x0F, 0x83, 0x43, 0x0C, 0x06, 0x0A, 0x04
};

const unsigned char lut_vcom1[] = 
{
    0x03, 0x1D, 0x01, 0x01, 0x08, 0x23, 0x37, 0x37,
    0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};

const unsigned char lut_red0[] = 
{
    0x83, 0x5D, 0x01, 0x81, 0x48, 0x23, 0x77, 0x77,
    0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};

const unsigned char lut_red1[] = 
{
    0x03, 0x1D, 0x01, 0x01, 0x08, 0x23, 0x37, 0x37,
    0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};
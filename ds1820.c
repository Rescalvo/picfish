#ifndef DS1820_C
#define DS1820_C

#include "1wire.c"

//DS18x20 Commands and functions
#define DS1820_SKIP_ROM        0xCC
#define DS1820_CONVERT_T       0x44
#define DS1820_READ_SCRATHPAD  0xBE
#define DS1820_WRITE_SCRATHPAD 0x4E



float ds1820_read();
void ds1820_configure(int8 TH, int8 TL, int8 config);

/*
 * ds1820_read()
 * Description: reads the ds18x20 device on the 1-wire bus and returns
 *              the temperature
 */
 
float ds1820_read() {
    int8 busy=0, temp1, temp2;
    signed int16 temp3;
    float result;
    
    //ds1820_configure(0x00, 0x00, 0x00);     //9 bit resolution

    onewire_reset();
    onewire_write(DS1820_SKIP_ROM);      //Skip ROM, address all devices
    onewire_write(DS1820_CONVERT_T);     //Start temperature conversion

    while(busy == 0)                //Wait while busy (bus is low)
        busy = onewire_read();

    onewire_reset();
    onewire_write(DS1820_SKIP_ROM);            //Skip ROM, address all devices
    onewire_write(DS1820_READ_SCRATHPAD);      //Read scratchpad
    temp1 = onewire_read();
    temp2 = onewire_read();
    temp3 = make16(temp2, temp1);
 
    //result = (float) temp3 / 2.0;   //Calculation for DS18S20 with 0.5 deg C resolution
    result = (float) temp3 / 16.0;    //Calculation for DS18B20
 
    delay_ms(200);
    return(result);
}

/*
 * ds1820_configure(int8 TH, int8 LH, int8 config)
 * Description: writes configuration data to the DS18x20 device
 * Arguments: alarm trigger high, alarm trigger low, configuration
 */

void ds1820_configure(int8 TH, int8 TL, int8 config) {
    onewire_reset();
    onewire_write(DS1820_SKIP_ROM);            //Skip ROM, address all devices
    onewire_write(DS1820_WRITE_SCRATHPAD);     //Write to scratchpad
    onewire_write(TH);
    onewire_write(TL);
    onewire_write(config);
}

#endif /*DS1820_C*/
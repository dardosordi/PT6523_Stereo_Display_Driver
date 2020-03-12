#ifndef StereoLCD_h_
#define StereoLCD_h_

#include <stdint.h>

#if defined(ARDUINO) && ARDUINO >= 100
#include "Arduino.h"
#elif defined(WIRING)
#include "Wiring.h"
#else
#include "WProgram.h"
#include "pins_arduino.h"
#endif


/**
 * Driver class for the display of Pioneer car stereos with the PT3523 driver IC
 */
class StereoLCD
{
	protected:
		// Bits to send
    const static uint8_t bits = 156;

		// Pins
    uint8_t inh_pin;
    uint8_t ce_pin;
    uint8_t clk_pin;
    uint8_t data_pin;

	uint8_t address = 0x41;
	uint8_t flag_DR = 0;
	uint8_t flag_SC = 0;
	uint8_t flag_BU = 0;

	int period = 5;


  public:
		// Buffer
    uint8_t buff[20];


    StereoLCD(uint8_t inh, uint8_t ce, uint8_t clk, uint8_t data);

	void enable();
	void disable();
	void send();

	void writeDigit(uint8_t digit, uint8_t value);

};



#endif

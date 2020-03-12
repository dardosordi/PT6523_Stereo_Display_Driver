#include "StereoLCD.h"

static const uint8_t font_table[67][2] = {

	// -BCMKHDA    JLIGEF--
	// -BC\/HDA    |/\GEF--
	// -BC..HDA    ...GEF--
	{0b01101011, 0b01001100}, // 0
	{0b01101000, 0b00000000}, // 1
	{0b01000111, 0b00011000}, // 2
	{0b01100111, 0b00010000}, // 3
	{0b01100100, 0b00010100}, // 4
	{0b00100111, 0b00010100}, // 5
	{0b00100111, 0b00011100}, // 6
	{0b01100001, 0b00000000}, // 7
	{0b01100111, 0b00011100}, // 8
	{0b01100111, 0b00010100}, // 9

	{0b01100101, 0b00011100}, // A
	{0b01100111, 0b10000000}, // B
	{0b00000011, 0b00001100}, // C
	{0b01100011, 0b10000000}, // D
	{0b00000011, 0b00011100}, // E
	{0b00000001, 0b00011100}, // F
	{0b00100111, 0b00001100}, // G
	{0b01100100, 0b00011100}, // H
	{0b00000000, 0b10000000}, // 1
	{0b01100010, 0b00001000}, // J
	{0b00011000, 0b00011100}, // K
	{0b00000010, 0b00001100}, // L
	{0b01101000, 0b00101100}, // M
	{0b01110000, 0b00101100}, // N
	{0b01100011, 0b00001100}, // O
	{0b01000101, 0b00011100}, // P
	{0b01110011, 0b00001100}, // Q
	{0b01010101, 0b00011100}, // R
	{0b00101111, 0b01010100}, // S
	{0b00000001, 0b10000000}, // T
	{0b01100010, 0b00001100}, // U
	{0b00001000, 0b01001100}, // V
	{0b01110000, 0b01001100}, // W
	{0b00011000, 0b01100000}, // X
	{0b00001000, 0b01100000}, // Y
	{0b00001111, 0b01010000}, // Z
	{0b00011100, 0b11110000}, // *
	{0b00000100, 0b10010000}, // +
	{0b00000100, 0b00010000}, // -
	{0b00001000, 0b01000000}, // /
	{0b00010000, 0b00100000}, // \ (back slash)
	{0b01001100, 0b01011000}, // %
	{0b00011000, 0b00000000}, // (
	{0b00000000, 0b01100000}, // )
	{0b00100111, 0b10010100}, // $
	{0b00111011, 0b00111000}, // &
	{0b00011011, 0b01100000}, // Clock
	{0b00001101, 0b00110000}, // Top
	{0b00010110, 0b01010000}, // Bottom
	{0b01111100, 0b00000000}, // Right
	{0b00000000, 0b01111100}, // Left
	{0b00000001, 0b00000000}, // Seg A
	{0b01000000, 0b00000000}, // Seg B
	{0b00100000, 0b00000000}, // Seg C
	{0b00000010, 0b00000000}, // Seg D
	{0b00000000, 0b00001000}, // Seg E
	{0b00000000, 0b00000100}, // Seg E
	{0b00000000, 0b00010000}, // Seg G
	{0b00000100, 0b00000000}, // Seg H
	{0b01000001, 0b00000100}, // Raising
	{0b00100010, 0b00001000}, // Falling
	{0b01001000, 0b00010000}, // sin1
	{0b00110000, 0b00010000}, // sin2
	{0b00000100, 0b01001000}, // sin3
	{0b00000100, 0b00100100}, // sin4

	// -BC\/HDA    |/\GEF--
	{0b01111111, 0b11111100}, // FULL
	{0b00000000, 0b00000000}, // BLANK
};

StereoLCD::StereoLCD(uint8_t inh, uint8_t ce, uint8_t clk, uint8_t data)
{
  inh_pin = inh;
  ce_pin = ce;
  clk_pin = clk;
  data_pin = data;

  pinMode(inh_pin, OUTPUT);
  pinMode(ce_pin, OUTPUT);
  pinMode(clk_pin, OUTPUT);
  pinMode(data_pin, OUTPUT);

  // Top Bars
  buff[0] = buff[0] | 0b00000001; // Seg 1
  buff[0] = buff[0] | 0b00000010; // Seg 2
  buff[1] = buff[1] | 0b10000000; // Seg 3
  buff[2] = buff[2] | 0b00000001; // Seg 4

  // Decimal Point
  buff[15] = buff[15] | 0b00100000;

  // LCD                  -CBDGAEF
  buff[17] = buff[17] | 0b01111111;

  // LCD                  ----1---
  buff[16] = buff[16] | 0b00001000;

  // lights ON
  buff[18] = buff[18] | 0b01000000;

  // SubW On
  buff[18] = buff[18] | 0b10000000;

  // Music
  buff[11] = buff[11] | 0b00001000;

  // Top Options
  buff[9] = buff[9] | 0b00001000; // Rpt
  buff[7] = buff[7] | 0b00100000; // ROM
  buff[5] = buff[5] | 0b01000000; // Track
  buff[3] = buff[3] | 0b10000000; // Int


  // EQ.
  buff[16] = buff[16] | 0b00000010; // EQ OutLine
  buff[16] = buff[16] | 0b10000000; // Classical
  buff[16] = buff[16] | 0b01000000; // Rock
  buff[16] = buff[16] | 0b00010000; // Flat
  buff[16] = buff[16] | 0b00000100; // Pop


  // Options
  buff[16] = buff[16] | 0b00000001; // TP
  buff[15] = buff[15] | 0b10000000; // TA
  buff[15] = buff[15] | 0b01000000; // EON
  buff[15] = buff[15] | 0b00010000; // AF
  buff[15] = buff[15] | 0b00001000; // PTY

  buff[15] = buff[15] | 0b00000001; // Loud
  buff[15] = buff[15] | 0b00000100; // Repeat Icon
  buff[15] = buff[15] | 0b00000010; // Arti

  buff[7]  = buff[7]  | 0b00010000; // Disc-
  buff[9]  = buff[9]  | 0b00010000; // MP3
  buff[11] = buff[11] | 0b00000100; // Folder Icon
  buff[13] = buff[13] | 0b00000010; // Musical Note Icon
  buff[3]  = buff[3]  | 0b01000000; // Mute
  buff[13] = buff[13] | 0b00000100; // Album


  // Bars
  buff[17] = buff[17] | 0b10000000;
  buff[18] = buff[18] | 0b00000001;
  buff[18] = buff[18] | 0b00000010;
  buff[18] = buff[18] | 0b00000100;
  buff[18] = buff[18] | 0b00001000;
  buff[18] = buff[18] | 0b00010000;
  buff[18] = buff[18] | 0b00100000;

}

void StereoLCD::enable()
{
    digitalWrite(inh_pin, HIGH);
}

void StereoLCD::disable()
{
    digitalWrite(inh_pin, LOW);
}

void StereoLCD::send()
{
	// Set CE pin low for address
	digitalWrite(ce_pin, LOW);

	// Clock in
	digitalWrite(clk_pin, LOW);
	delayMicroseconds(period);

	// Write chip address
	for (uint8_t i = 0; i <8 ; ++i) {
     delayMicroseconds(period);
	  digitalWrite(clk_pin, LOW);

	  digitalWrite(data_pin, (address>>i) & 0x01);
	  digitalWrite(clk_pin, HIGH);
	  delayMicroseconds(period);
	}

	// Set CE pin high for data
	digitalWrite(ce_pin, HIGH);
	delayMicroseconds(period);

	// send buffer
	for (uint8_t i = 0; i < 20 ; ++i) {
		uint8_t b = i < 19 ? 8 : 5;
		for (uint8_t j = 0; j < b; j++) {
			uint8_t d_bit = (buff[i]>>j) & 0x01;
			delayMicroseconds(period);
			digitalWrite(clk_pin, LOW);
			digitalWrite(data_pin, d_bit);
			digitalWrite(clk_pin, HIGH);
		}
	}

	// send DR flag
	delayMicroseconds(period);
	digitalWrite(clk_pin, LOW);
	digitalWrite(data_pin, flag_DR);
	digitalWrite(clk_pin, HIGH);

	// send SC flag
	delayMicroseconds(period);
	digitalWrite(clk_pin, LOW);
	digitalWrite(data_pin, flag_SC);
	digitalWrite(clk_pin, HIGH);

	// send BU flag
	delayMicroseconds(period);
	digitalWrite(clk_pin, LOW);
	digitalWrite(data_pin, flag_BU);
	digitalWrite(clk_pin, HIGH);


	// not used bits
	delayMicroseconds(period);
	digitalWrite(clk_pin, LOW);
	digitalWrite(data_pin, LOW);
	digitalWrite(data_pin, HIGH);


	// DONE
	digitalWrite(clk_pin, LOW);
	delayMicroseconds(period);
	digitalWrite(ce_pin, LOW);

}

void StereoLCD::writeDigit(uint8_t digit, uint8_t value)
{
	/*
	uint32_t mask = 0b00000000000000000001111111111111;
	uint32_t data = font_table[value];


	data = data<<offset;
	mask = mask<<offset;
	mask = ~mask;
*/


	uint8_t index  = digit * 2;
	uint8_t offset = digit;


	uint32_t data = font_table[value][0]<<8 | font_table[value][1];
	data = data << 8;
	data = (data>>offset);

//	uint32_t mask = 0b11111111110000000000000111111111;
	uint32_t mask = 0b1111111100000000000001111111111;

	mask = mask>>offset;

	uint8_t mask_high = mask >> 16;
	uint8_t mask_low = mask >> 8;
	uint8_t mask_reminder = mask;


	buff[index+1] = (buff[index+1] & mask_high) | data >> 16;
	buff[index]   = (buff[index] & mask_low) | data >> 8;

	if (digit > 1) {
		buff[index - 1] = (buff[index - 1] & mask_reminder) | data;
	}

}


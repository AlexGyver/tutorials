#ifndef FastIO_h
#define FastIO_h
// быстрый IO, v1.0

// быстрое чтение пина
bool fastRead(const uint8_t pin) {
#if defined(__AVR_ATmega328P__) || defined(__AVR_ATmega168__)
	if (pin < 8) return bitRead(PIND, pin);
	else if (pin < 14) return bitRead(PINB, pin - 8);
	else if (pin < 20) return bitRead(PINC, pin - 14);
	
#elif defined(__AVR_ATtiny85__) || defined(__AVR_ATtiny13__)
	return bitRead(PINB, pin);
	
#elif defined(AVR)
	uint8_t *_pin_reg = portInputRegister(digitalPinToPort(pin));
	uint8_t _bit_mask = digitalPinToBitMask(pin);
	return bool(*_pin_reg & _bit_mask);

#else
	return digitalRead(pin);

#endif
	return 0;
}

// быстрая запись
void fastWrite(const uint8_t pin, bool val) {
#if defined(__AVR_ATmega328P__) || defined(__AVR_ATmega168__)
	if (pin < 8) bitWrite(PORTD, pin, val);
	else if (pin < 14) bitWrite(PORTB, (pin - 8), val);
	else if (pin < 20) bitWrite(PORTC, (pin - 14), val);

#elif defined(__AVR_ATtiny85__) || defined(__AVR_ATtiny13__)
	bitWrite(PORTB, pin, val);
	
#elif defined(AVR)	
	uint8_t *_port_reg = portInputRegister(digitalPinToPort(pin));
	uint8_t _bit_mask = digitalPinToBitMask(pin);
	_port_reg = portOutputRegister(digitalPinToPort(pin));
	_bit_mask = digitalPinToBitMask(pin);
	if (val) *_port_reg |= _bit_mask;	// HIGH
	else *_port_reg &= ~_bit_mask;		// LOW

#else
	digitalWrite(pin, val);

#endif
}

#endif

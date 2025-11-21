#ifndef ATMEGADRIVERS_ADC_HPP_INCLUDED
#define ATMEGADRIVERS_ADC_HPP_INCLUDED

#include <stdint.h>
#include <avr/io.h>

class AdcDriver
{
	public: // <-- ACUM E PUBLIC
	AdcDriver();
	~AdcDriver();

	void Init();
	uint16_t Read(uint8_t channel);

	private:
	AdcDriver(const AdcDriver&) = delete;
	AdcDriver& operator=(const AdcDriver&) = delete;
};

#endif
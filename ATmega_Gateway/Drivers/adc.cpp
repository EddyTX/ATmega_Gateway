#include "adc.hpp"
#include "project_defines.hpp"

// --- IMPLEMENT?RILE LIPSA ---
AdcDriver::AdcDriver()
{
}

AdcDriver::~AdcDriver()
{
}
// ---------------------------

void AdcDriver::Init()
{
	ADMUX |= (1 << REFS0);
	// Prescaler 128
	ADCSRA |= (1 << ADPS2) | (1 << ADPS1) | (1 << ADPS0);
	// Enable
	ADCSRA |= (1 << ADEN);
}

uint16_t AdcDriver::Read(uint8_t channel)
{
	channel &= ADC_CHANNEL_MASK;

	ADMUX = (ADMUX & ADC_MUX_MASK) | channel;

	ADCSRA |= (1 << ADSC);

	// Safety Timeout
	uint16_t timeout = ADC_TIMEOUT_CYCLES;
	while ((ADCSRA & (1 << ADSC)) && timeout > 0)
	{
		timeout--;
	}

	return ADC;
}
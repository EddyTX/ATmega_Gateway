#include "gpio.hpp"

GpioDriver::GpioDriver()
{
}

GpioDriver::~GpioDriver()
{
}

void GpioDriver::SetPinDirection(volatile uint8_t* ddr, uint8_t pin, PinDirection direction)
{
	if (ddr == nullptr)
	{
		return;
	}

	if (direction == PinDirection::OUTPUT)
	{
		*ddr |= (1 << pin);
	}
	else
	{
		*ddr &= ~(1 << pin);
	}
}

void GpioDriver::SetPinLevel(volatile uint8_t* port, uint8_t pin, PinLevel level)
{
	if (port == nullptr)
	{
		return;
	}

	if (level == PinLevel::HIGH)
	{
		*port |= (1 << pin);
	}
	else
	{
		*port &= ~(1 << pin);
	}
}

void GpioDriver::TogglePin(volatile uint8_t* port, uint8_t pin)
{
	if (port == nullptr)
	{
		return;
	}

	*port ^= (1 << pin);
}

PinLevel GpioDriver::ReadPin(volatile uint8_t* pinReg, uint8_t pin)
{
	if (pinReg == nullptr)
	{
		return PinLevel::LOW;
	}

	if (*pinReg & (1 << pin))
	{
		return PinLevel::HIGH;
	}
	
	return PinLevel::LOW;
}
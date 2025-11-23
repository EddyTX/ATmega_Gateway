#ifndef ATMEGADRIVERS_GPIO_HPP_INCLUDED
#define ATMEGADRIVERS_GPIO_HPP_INCLUDED

#include <stdint.h>
#include <avr/io.h>

enum PinDirection : uint8_t
{
	INPUT = 0,
	OUTPUT = 1
};

enum PinLevel : uint8_t
{
	LOW = 0,
	HIGH = 1
};

class GpioDriver
{
	public:
		GpioDriver();
		~GpioDriver();

		void SetPinDirection(volatile uint8_t* ddr, uint8_t pin, PinDirection direction);
		void SetPinLevel(volatile uint8_t* port, uint8_t pin, PinLevel level);
		void TogglePin(volatile uint8_t* port, uint8_t pin);
		PinLevel ReadPin(volatile uint8_t* pinReg, uint8_t pin);

	private:
		GpioDriver(const GpioDriver&) = delete;
		GpioDriver& operator=(const GpioDriver&) = delete;
};

#endif
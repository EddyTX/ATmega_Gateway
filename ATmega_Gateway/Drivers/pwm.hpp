#ifndef ATMEGADRIVERS_PWM_HPP_INCLUDED
#define ATMEGADRIVERS_PWM_HPP_INCLUDED

#include <stdint.h>
#include <avr/io.h>

class PwmDriver
{
	public:
		PwmDriver();
		~PwmDriver();

		void Init();
		void SetDutyCycle(uint8_t channel, uint8_t duty);

	private:
		PwmDriver(const PwmDriver&) = delete;
		PwmDriver& operator=(const PwmDriver&) = delete;
};

#endif
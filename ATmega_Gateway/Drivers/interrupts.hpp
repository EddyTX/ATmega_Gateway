#ifndef ATMEGADRIVERS_INTERRUPTS_HPP_INCLUDED
#define ATMEGADRIVERS_INTERRUPTS_HPP_INCLUDED

#ifdef __AVR__
#include <avr/interrupt.h>
#endif

class Interrupts
{
	public:
		static void Enable()
		{
			#ifdef __AVR__
			sei();
			#endif
		}

		static void Disable()
		{
			#ifdef __AVR__
			cli();
			#endif
		}
};

#endif
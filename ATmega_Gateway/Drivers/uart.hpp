#ifndef ATMEGADRIVERS_UART_HPP_INCLUDED
#define ATMEGADRIVERS_UART_HPP_INCLUDED

#include <stdint.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include "project_defines.hpp"

class UartDriver
{
	public:
	UartDriver();
	~UartDriver();

	void Init(uint32_t baud);
	void SendChar(char c);
	bool ReadChar(char& c);

	void ReceiveCharISR(uint8_t c);
	void SendCharISR();

	static UartDriver* isr_instance;

	private:
	UartDriver(const UartDriver&) = delete;
	UartDriver& operator=(const UartDriver&) = delete;

	uint8_t rxBuffer[UART_RX_BUFFER_SIZE];
	volatile uint8_t head;
	volatile uint8_t tail;

	uint8_t txBuffer[UART_TX_BUFFER_SIZE];
	volatile uint8_t txHead;
	volatile uint8_t txTail;
};
#endif
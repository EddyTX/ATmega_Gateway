#include "gpio_handler.hpp"
#include "project_defines.hpp"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

// FIX: Constructorul foloseste ICommunicationChannel si GpioDriver
GpioHandler::GpioHandler(ICommunicationChannel& comm, GpioDriver& gpio)
: comm_(comm), gpioDriver_(gpio)
{
	// Ini?ializ?m un pin default (LED-ul built-in)
	// Pe UNO/Nano e Port B, Pin 5
	gpioDriver_.SetPinDirection(&DDRB, 5, PinDirection::OUTPUT);
}

void GpioHandler::SendOK()
{
	const char* msg = "HTTP/1.1 200 OK\r\nContent-Length: 2\r\n\r\nOK";
	comm_.SendData((const uint8_t*)msg, strlen(msg));
}

bool GpioHandler::Handle(const char* method, const char* uri)
{
	if (strcmp(method, "POST") != 0) return false;

	if (strncmp(uri, "/gpio/toggle/", STR_LEN_GPIO_TOGGLE) == 0)
	{
		char* params = (char*)(uri + STR_LEN_GPIO_TOGGLE);
		char* portStr = strtok(params, "/");
		char* pinStr = strtok(NULL, "/");

		if (portStr && pinStr && strcmp(portStr, "B") == 0)
		{
			uint8_t pin = atoi(pinStr);
			gpioDriver_.TogglePin(&PORTB, pin);
			SendOK();
			return true;
		}
	}
	return false;
}
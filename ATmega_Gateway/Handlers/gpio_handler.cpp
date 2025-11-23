#include "gpio_handler.hpp"
#include "project_defines.hpp"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

GpioHandler::GpioHandler(ICommunicationChannel& comm, GpioDriver& gpio)
: comm_(comm), gpioDriver_(gpio)
{
	gpioDriver_.SetPinDirection(&DDRB, 5, PinDirection::OUTPUT);
}

void GpioHandler::SendOK()
{
	const char* msg = "HTTP/1.1 200 OK\r\nContent-Length: 2\r\n\r\nOK";
	comm_.SendData((const uint8_t*)msg, strlen(msg));
}

void SendState(ICommunicationChannel& comm, PinLevel level)
{
	const char* body = (level == PinLevel::HIGH) ? "1" : "0";
	char response[64];
	snprintf(response, sizeof(response),
	"HTTP/1.1 200 OK\r\nContent-Length: 1\r\n\r\n%s", body);
	comm.SendData((const uint8_t*)response, strlen(response));
}

bool GpioHandler::Handle(const char* method, const char* uri)
{
	if (strcmp(method, "POST") == 0)
	{
		if (strncmp(uri, "/gpio/toggle/", STR_LEN_GPIO_TOGGLE) == 0)
		{
			char* params = (char*)(uri + STR_LEN_GPIO_TOGGLE);
			char* portStr = strtok(params, "/");
			char* pinStr = strtok(NULL, "/");

			if (portStr && pinStr && (portStr[0] == 'B' || portStr[0] == 'b'))
			{
				uint8_t pin = atoi(pinStr);
				gpioDriver_.TogglePin(&PORTB, pin);
				SendOK();
				return true;
			}
		}

		if (strncmp(uri, "/gpio/set/", STR_LEN_GPIO_SET) == 0)
		{
			char* params = (char*)(uri + STR_LEN_GPIO_SET);
			char* portStr = strtok(params, "/");
			char* pinStr = strtok(NULL, "/");
			char* levelStr = strtok(NULL, "/");

			if (portStr && pinStr && levelStr && (portStr[0] == 'B' || portStr[0] == 'b'))
			{
				uint8_t pin = atoi(pinStr);
				PinLevel level = PinLevel::LOW;
				if (strcmp(levelStr, "high") == 0 || strcmp(levelStr, "1") == 0)
				{
					level = PinLevel::HIGH;
				}

				gpioDriver_.SetPinLevel(&PORTB, pin, level);
				SendOK();
				return true;
			}
		}
	}

	if (strcmp(method, "GET") == 0)
	{
		if (strncmp(uri, "/gpio/get/", STR_LEN_GPIO_GET) == 0)
		{
			char* params = (char*)(uri + STR_LEN_GPIO_GET);
			char* portStr = strtok(params, "/");
			char* pinStr = strtok(NULL, "/");

			if (portStr && pinStr)
			{
				uint8_t pin = atoi(pinStr);
				volatile uint8_t* inputReg = nullptr;
				
				if (portStr[0] == 'B' || portStr[0] == 'b') inputReg = &PINB;
				else if (portStr[0] == 'C' || portStr[0] == 'c') inputReg = &PINC;
				else if (portStr[0] == 'D' || portStr[0] == 'd') inputReg = &PIND;

				if (inputReg != nullptr)
				{
					PinLevel val = gpioDriver_.ReadPin(inputReg, pin);
					SendState(comm_, val);
					return true;
				}
			}
		}
	}

	return false;
}
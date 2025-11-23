#include "project_defines.hpp"
#include "serial_channel.hpp"
#include "http_server.hpp"
#include "timer.hpp"
#include "gpio.hpp"
#include "adc.hpp"
#include "pwm.hpp"
#include "uart.hpp"
#include "timer_handler.hpp"
#include "gpio_handler.hpp"
#include "adc_handler.hpp"
#include "pwm_handler.hpp"
#include "interrupts.hpp"
#include "system_handler.hpp"
#include <string.h>


int main()
{
	UartDriver uartDriver;
	TimerDriver timerDriver;
	GpioDriver gpioDriver;
	AdcDriver adcDriver;
	PwmDriver pwmDriver;

	uartDriver.Init(UART_BAUD_RATE);
	pwmDriver.Init();
	
	Interrupts::Enable();

	SerialChannel commChannel(uartDriver);

	HttpServer server(commChannel);

	TimerHandler timerHandler(commChannel, timerDriver);
	GpioHandler gpioHandler(commChannel, gpioDriver);
	AdcHandler adcHandler(commChannel, adcDriver);
	PwmHandler pwmHandler(commChannel, pwmDriver);
	SystemHandler systemHandler(commChannel, timerDriver);

	server.RegisterHandler(&timerHandler);
	server.RegisterHandler(&gpioHandler);
	server.RegisterHandler(&adcHandler);
	server.RegisterHandler(&pwmHandler);
	server.RegisterHandler(&systemHandler);

	const char* startupMsg = "\r\n--- Modular HTTP Server Started ---\r\n";
	commChannel.SendData((const uint8_t*)startupMsg, strlen(startupMsg));

	while (1)
	{
		server.Process();
		timerDriver.Run();
	}

	return 0;
}
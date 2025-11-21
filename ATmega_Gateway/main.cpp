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
#include <string.h>
#include <avr/interrupt.h>

int main()
{
	// 1. Instantiere Drivere (Hardware)
	UartDriver uartDriver;
	TimerDriver timerDriver;
	GpioDriver gpioDriver;
	AdcDriver adcDriver;
	PwmDriver pwmDriver;

	// 2. Initializare Hardware
	uartDriver.Init(UART_BAUD_RATE);
	pwmDriver.Init();
	
	// Activare intreruperi globale
	sei();

	// 3. Instantiere Comunicatie
	SerialChannel commChannel(uartDriver);

	// 4. Instantiere Server
	HttpServer server(commChannel);

	// 5. Instantiere Handlers (Dependency Injection)
	TimerHandler timerHandler(commChannel, timerDriver);
	GpioHandler gpioHandler(commChannel, gpioDriver);
	AdcHandler adcHandler(commChannel, adcDriver);
	PwmHandler pwmHandler(commChannel, pwmDriver);

	// 6. Inregistrare Handlers
	server.RegisterHandler(&timerHandler);
	server.RegisterHandler(&gpioHandler);
	server.RegisterHandler(&adcHandler);
	server.RegisterHandler(&pwmHandler);

	const char* startupMsg = "\r\n--- Modular HTTP Server Started ---\r\n";
	commChannel.SendData((const uint8_t*)startupMsg, strlen(startupMsg));

	while (1)
	{
		server.Process();
		timerDriver.Run();
	}

	return 0;
}
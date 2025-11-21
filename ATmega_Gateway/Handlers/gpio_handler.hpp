#pragma once

#include "iendpoint_handler.hpp"
#include "i_communication_channel.hpp" // FIX: Include fisierul nou
#include "gpio.hpp"

class GpioHandler : public IEndpointHandler
{
	public:
	// FIX: Folosim ICommunicationChannel
	GpioHandler(ICommunicationChannel& comm, GpioDriver& gpioDriver);
	bool Handle(const char* method, const char* uri) override;

	private:
	void SendOK();
	
	// FIX: Folosim ICommunicationChannel
	ICommunicationChannel& comm_;
	GpioDriver& gpioDriver_;
};
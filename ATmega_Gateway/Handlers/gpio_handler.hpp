#pragma once

#include "iendpoint_handler.hpp"
#include "i_communication_channel.hpp"
#include "gpio.hpp"

class GpioHandler : public IEndpointHandler
{
	public:
		GpioHandler(ICommunicationChannel& comm, GpioDriver& gpioDriver);
		bool Handle(const char* method, const char* uri) override;

	private:
		void SendOK();
	
		ICommunicationChannel& comm_;
		GpioDriver& gpioDriver_;
};
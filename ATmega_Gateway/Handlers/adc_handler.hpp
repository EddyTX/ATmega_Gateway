#pragma once

#include "iendpoint_handler.hpp"
#include "i_communication_channel.hpp"
#include "adc.hpp"

class AdcHandler : public IEndpointHandler
{
	public:
	AdcHandler(ICommunicationChannel& comm, AdcDriver& adcDriver);
	bool Handle(const char* method, const char* uri) override;

	private:
	void SendResponse(const char* code, const char* contentType, const char* body);

	ICommunicationChannel& comm_;
	AdcDriver& adcDriver_;
};
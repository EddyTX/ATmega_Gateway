#pragma once

#include "iendpoint_handler.hpp"
#include "i_communication_channel.hpp"
#include "pwm.hpp"

class PwmHandler : public IEndpointHandler
{
	public:
	PwmHandler(ICommunicationChannel& comm, PwmDriver& pwmDriver);
	bool Handle(const char* method, const char* uri) override;

	private:
	void SendOK();
	ICommunicationChannel& comm_;
	PwmDriver& pwmDriver_;
};
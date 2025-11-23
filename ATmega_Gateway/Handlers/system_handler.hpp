#pragma once

#include "iendpoint_handler.hpp"
#include "i_communication_channel.hpp"
#include "timer.hpp"

class SystemHandler : public IEndpointHandler
{
	public:
		SystemHandler(ICommunicationChannel& comm, TimerDriver& timerDriver);
		bool Handle(const char* method, const char* uri) override;

	private:
		void SendResponse(const char* code, const char* contentType, const char* body);
	
		ICommunicationChannel& comm_;
		TimerDriver& timerDriver_;
};
#pragma once

#include "iendpoint_handler.hpp"
#include "i_communication_channel.hpp" // FIX: Numele nou al fisierului
#include "timer.hpp"

class TimerHandler : public IEndpointHandler
{
	public:
	// FIX: Folosim ICommunicationChannel (numele complet/clar)
	TimerHandler(ICommunicationChannel& comm, TimerDriver& timerDriver);

	// FIX: Parametrul se numeste 'uri' ca sa se potriveasca cu implementarea din .cpp
	bool Handle(const char* method, const char* uri) override;

	private:
	void SendResponse(const char* code, const char* contentType, const char* body);

	// FIX: Tipul corect
	ICommunicationChannel& comm_;
	TimerDriver& timerDriver_;
};
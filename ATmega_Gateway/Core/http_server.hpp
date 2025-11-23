#pragma once

#include "i_communication_channel.hpp"
#include "iendpoint_handler.hpp"
#include "project_defines.hpp"

class HttpServer
{
	public:
		HttpServer(ICommunicationChannel& comm);
		void Process();
		bool RegisterHandler(IEndpointHandler* handler);

	private:
		bool HandleRequest(char* req);
		void SendResponse(const char* code, const char* contentType, const char* body);
		void Send400();
		void Send403();
		void Send404();

		ICommunicationChannel& comm_;
		IEndpointHandler* handlers_[MAX_HANDLERS];
		uint8_t handlerCount_;
};
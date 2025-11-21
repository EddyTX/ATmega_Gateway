#include "timer_handler.hpp"
#include "project_defines.hpp"
#include <string.h>
#include <stdio.h>

TimerHandler::TimerHandler(ICommunicationChannel& comm, TimerDriver& timerDriver)
: comm_(comm), timerDriver_(timerDriver)
{
}

void TimerHandler::SendResponse(const char* code, const char* contentType, const char* body)
{
	// FIX: Folosim constanta, nu magic number 128
	char response[HTTP_RESPONSE_BUFFER_SIZE];
	
	// Safety check: body poate fi null
	int bodyLen = body ? strlen(body) : 0;

	snprintf(response, sizeof(response),
	"HTTP/1.1 %s\r\n"
	"Content-Type: %s\r\n"
	"Content-Length: %d\r\n\r\n",
	code, contentType, bodyLen);

	comm_.SendData((const uint8_t*)response, strlen(response));
	
	if (body && bodyLen > 0)
	{
		comm_.SendData((const uint8_t*)body, bodyLen);
	}
	comm_.SendData((const uint8_t*)"\r\n", 2);
}

// FIX: Parametrul se nume?te 'uri', nu 'path'
bool TimerHandler::Handle(const char* method, const char* uri)
{
	if (strcmp(method, "GET") != 0)
	{
		return false;
	}

	if (strcmp(uri, "/timer/elapsed") == 0)
	{
		uint32_t seconds = timerDriver_.GetSystemSeconds();
		
		// 20 e ok, dar putem fi mai expliciti sau lasam asa fiind mic
		char body[20];
		snprintf(body, sizeof(body), "%lu", seconds);
		
		SendResponse("200 OK", "text/plain", body);
		return true;
	}

	if (strcmp(uri, "/msg") == 0)
	{
		SendResponse("200 OK", "text/plain", "Hello World Modular");
		return true;
	}

	return false;
}
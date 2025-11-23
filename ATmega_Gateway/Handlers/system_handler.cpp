#include "system_handler.hpp"
#include "project_defines.hpp"
#include <string.h>
#include <stdio.h>

SystemHandler::SystemHandler(ICommunicationChannel& comm, TimerDriver& timerDriver)
: comm_(comm), timerDriver_(timerDriver)
{
}

void SystemHandler::SendResponse(const char* code, const char* contentType, const char* body)
{
	char response[HTTP_RESPONSE_BUFFER_SIZE];
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

bool SystemHandler::Handle(const char* method, const char* uri)
{
	if (strcmp(method, "GET") != 0)
	{
		return false;
	}

	if (strcmp(uri, "/msg") == 0 || strcmp(uri, "/ping") == 0)
	{
		SendResponse("200 OK", "text/plain", "SysLink Gateway Online");
		return true;
	}

	if (strcmp(uri, "/status") == 0)
	{
		uint32_t totalSeconds = timerDriver_.GetSystemSeconds();
		
		uint32_t hours = totalSeconds / 3600;
		uint32_t remainder = totalSeconds % 3600;
		uint32_t minutes = remainder / 60;
		uint32_t seconds = remainder % 60;

		char body[128];
		snprintf(body, sizeof(body),
		"System Status: OK\n"
		"Uptime: %02lu:%02lu:%02lu\n"
		"Auth Host: %s",
		hours, minutes, seconds, ALLOWED_HOST);

		SendResponse("200 OK", "text/plain", body);
		return true;
	}

	return false;
}
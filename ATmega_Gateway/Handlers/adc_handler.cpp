#include "adc_handler.hpp"
#include "project_defines.hpp"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

AdcHandler::AdcHandler(ICommunicationChannel& comm, AdcDriver& adcDriver)
: comm_(comm), adcDriver_(adcDriver)
{
	adcDriver_.Init();
}

void AdcHandler::SendResponse(const char* code, const char* contentType, const char* body)
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

bool AdcHandler::Handle(const char* method, const char* uri)
{
	if (strcmp(method, "GET") != 0)
	{
		return false;
	}

	if (strncmp(uri, "/adc/read/", STR_LEN_ADC_PATH) == 0)
	{
		uint8_t channel = atoi(uri + STR_LEN_ADC_PATH);
		uint16_t val = adcDriver_.Read(channel);

		char body[16];
		snprintf(body, sizeof(body), "%d", val);

		SendResponse("200 OK", "text/plain", body);
		return true;
	}

	return false;
}
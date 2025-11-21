#include "pwm_handler.hpp"
#include "project_defines.hpp"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

PwmHandler::PwmHandler(ICommunicationChannel& comm, PwmDriver& pwmDriver)
: comm_(comm), pwmDriver_(pwmDriver)
{
	pwmDriver_.Init();
}

void PwmHandler::SendOK()
{
	const char* response = "HTTP/1.1 200 OK\r\nContent-Length: 2\r\n\r\nOK";
	comm_.SendData((const uint8_t*)response, strlen(response));
}

bool PwmHandler::Handle(const char* method, const char* uri)
{
	if (strcmp(method, "POST") != 0)
	{
		return false;
	}

	if (strncmp(uri, "/pwm/set/", STR_LEN_PWM_SET) == 0)
	{
		char* params = (char*)(uri + STR_LEN_PWM_SET);
		char* slash = strchr(params, '/');

		if (slash)
		{
			*slash = 0;
			uint8_t channel = atoi(params);
			uint8_t duty = atoi(slash + 1);
			*slash = '/';

			pwmDriver_.SetDutyCycle(channel, duty);
			SendOK();
			return true;
		}
	}

	return false;
}
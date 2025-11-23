#include "http_server.hpp"
#include "project_defines.hpp"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

HttpServer::HttpServer(ICommunicationChannel& comm) : comm_(comm), handlerCount_(0)
{
}

bool HttpServer::RegisterHandler(IEndpointHandler* handler)
{
	if (handler == nullptr)
	{
		return false;
	}

	if (handlerCount_ < MAX_HANDLERS)
	{
		handlers_[handlerCount_++] = handler;
		return true;
	}

	return false;
}

void HttpServer::SendResponse(const char* code, const char* contentType, const char* body)
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

void HttpServer::Send400() { SendResponse("400 Bad Request", "text/plain", "Bad Request"); }
void HttpServer::Send403() { SendResponse("403 Forbidden", "text/plain", "Access Denied"); }
void HttpServer::Send404() { SendResponse("404 Not Found", "text/plain", "Not Found"); }

void HttpServer::Process()
{
	static char buffer[HTTP_REQUEST_BUFFER_SIZE];
	static uint16_t index = 0;
	uint8_t data;

	while (comm_.ReceiveData(data))
	{
		char c = static_cast<char>(data);

		if (index < HTTP_REQUEST_BUFFER_SIZE - 1)
		{
			buffer[index++] = c;
			buffer[index] = 0;
		}
		else
		{
			index = 0;
		}

		if (index >= HTTP_HEADER_END_LEN &&
		buffer[index - 4] == '\r' && buffer[index - 3] == '\n' &&
		buffer[index - 2] == '\r' && buffer[index - 1] == '\n')
		{
			char expectedHost[HTTP_MAX_HOST_LEN];
			snprintf(expectedHost, sizeof(expectedHost), "Host: %s", ALLOWED_HOST);

			if (strstr(buffer, expectedHost) != nullptr)
			{
				HandleRequest(buffer);
			}
			else
			{
				Send403();
			}
			index = 0;
		}
	}
}

bool HttpServer::HandleRequest(char* req)
{
	if (req == nullptr) return false;

	char* method = strtok(req, " ");
	char* path = strtok(NULL, " ");
	char* version = strtok(NULL, " \r\n");

	if (method == nullptr || path == nullptr || version == nullptr)
	{
		Send400();
		return false;
	}

	if (strcmp(version, "HTTP/1.1") != 0)
	{
		Send400();
		return false;
	}

	for (uint8_t i = 0; i < handlerCount_; i++)
	{
		if (handlers_[i]->Handle(method, path))
		{
			return true;
		}
	}

	Send404();
	return false;
}
#pragma once
#include <stdint.h>
#include <stddef.h>

class ICommunicationChannel
{
	public:
		virtual void SendData(const uint8_t* data, size_t size) = 0;
		virtual bool ReceiveData(uint8_t& data) = 0;
		virtual ~ICommunicationChannel() = default;
};
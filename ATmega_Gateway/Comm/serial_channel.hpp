#pragma once
#include "i_communication_channel.hpp"
#include "uart.hpp"

class SerialChannel : public ICommunicationChannel
{
	public:
	SerialChannel(UartDriver& uartDriver);
	void SendData(const uint8_t* data, size_t size) override;
	bool ReceiveData(uint8_t& data) override;

	private:
	UartDriver& uartDriver_;
};
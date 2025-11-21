#include "serial_channel.hpp"

SerialChannel::SerialChannel(UartDriver& uartDriver)
: uartDriver_(uartDriver)
{
}

void SerialChannel::SendData(const uint8_t* data, size_t size)
{
	if (data == nullptr || size == 0) return;

	for (size_t i = 0; i < size; ++i)
	{
		uartDriver_.SendChar(data[i]);
	}
}

bool SerialChannel::ReceiveData(uint8_t& data)
{
	char c;
	if (uartDriver_.ReadChar(c))
	{
		data = static_cast<uint8_t>(c);
		return true;
	}
	return false;
}
#include "Uart.h"

Uart::Uart(PinName tx, PinName rx, uint16_t baudrate) : _baudrate(baudrate), _esp_serial_port(tx, rx, baudrate)
{
    _esp_serial_port.baud(_baudrate);
    _esp_serial_port.format(
        8,
        SerialBase::Parity::Even,
        1);
}

Data Uart::ReceiveData()
{
    int16_t len = _esp_serial_port.read(this->_message_buffer, sizeof(this->_message_buffer));

    if (len > 0) && (len == 4)
    {
        this->_message.pitch = this->_message_buffer[0:2];
        this->_message.yaw = this->_message_buffer[3];
    }
    return this->_message;
}
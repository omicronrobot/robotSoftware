#include "uart.h"
#include "mbed.h"

Uart::Uart(PinName tx, PinName rx, int baudrate) : _tx_pin(tx), _rx_pin(rx), _baudrate(baudrate), _esp_serial_port(tx, rx, MAXIMUM_BUFFER_SIZE)
{
}

void Uart::init()
{
    _esp_serial_port.baud(_baudrate);
    _esp_serial_port.format(
        8,
        SerialBase::Parity::Even,
        1);
}

Data Uart::_format_data(char buffer[MAXIMUM_BUFFER_SIZE])
{
}

Data Uart::ReceiveData()
{
    uint32_t num = _esp_serial_port.read(_message_buffer, sizeof(_message_buffer))

}

Uart::~Uart()
{
}

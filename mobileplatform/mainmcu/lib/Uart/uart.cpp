#include "uart.h"
#include "mbed.h"

Uart::Uart(PinName tx, PinName rx, int baudrate) : _tx_pin(tx), _rx_pin(rx), _baudrate(baudrate), _esp_serial_port(tx, rx, baudrate)
{
    _esp_serial_port.baud(_baudrate);
    _esp_serial_port.format(
        8,
        SerialBase::Parity::Even,
        1);
}

void Uart::init()
{
    _thread.start(callback(&_queue, &EventQueue::dispatch_forever));

    _esp_serial_port.attach(callback(_rx_callback), AsynchSerial::RX);
    _esp_serial_port.attach(callback(_tx_callback), AsynchSerial::TX);
    _esp_serial_port.init();
    _queue.dispatch();
}

Data Uart::ReceiveData()
{
    return _message;
}

void Uart::_get_data()
{
    int16_t len = _esp_serial_port.read(_message_buffer, sizeof(_message_buffer));

    if (len > 0)
    {
        // 0200.1200.1000.h.m
        _message.pitch = _message_buffer[0, 3];
        _message.roll = _message_buffer[5, 8];
        _message.yaw = _message_buffer[10, 13];
        // // Echo back what we received
        // _esp_serial_port.write(_message_buffer, len);
    }
}
void Uart::_rx_callback()
{
    _queue.call(_get_data);
}
void Uart::_tx_callback()
{
    printf("sent\n");
}

Uart::~Uart()
{
}

#ifndef UART_H
#define UART_H

#include "defs.h"
#include "mbed.h"
// #include "BufferedSerial.h"

// TODO add "AsynchSerial/AsynchSerial.h"

class Uart
{
private:
    /* data */
    PinName _rx_pin;
    PinName _tx_pin;
    int _baudrate;
    int _buffer_size = MAXIMUM_BUFFER_SIZE;
    char* _message_buffer[MAXIMUM_BUFFER_SIZE] = {0};
    BufferedSerial _esp_serial_port;
    Data _format_data(char buffer[MAXIMUM_BUFFER_SIZE]);

public:
    Uart(PinName rx, PinName tx, int baudrate);
    void init();
    Data ReceiveData();
    ~Uart();
};

#endif
#ifndef MOBILEPLATFORM_MAINMCU_LIB_UART_UART_H_
#define MOBILEPLATFORM_MAINMCU_LIB_UART_UART_H_

#include "mbed.h"
#include <Defs.h>
#include <AsynchSerial.h>

class Uart
{
private:
    uint16_t _baudrate;                                 // Baudrate for communication between wifi client and main mcu
    uint8_t _buffer_size = MAXIMUM_BUFFER_SIZE;         // the buffer size of messages sent
    unsigned char _message_buffer[MAXIMUM_BUFFER_SIZE]; // the message buffer
    Data _message;                                      // The data received
    AsynchSerial _esp_serial_port;                      // Serial port to the wifi client

public:
    Uart(PinName rx, PinName tx, uint16_t baudrate);
    Data ReceiveData(); // Receive data from serial communication after formating data
};

#endif // MOBILEPLATFORM_MAINMCU_LIB_UART_UART_H_
#ifndef UART_H
#define UART_H

#include "defs.h"
#include "mbed.h"
#include <AsynchSerial.h>

class Uart
{
private:
    /* data */
    PinName _rx_pin;                                    // RX pin attached to wifi client
    PinName _tx_pin;                                    // TX pin attached to wifi client
    int _baudrate;                                      // Baudrate for communication between wifi client and main mcu
    int _buffer_size = MAXIMUM_BUFFER_SIZE;             // the buffer size of messages sent
    unsigned char _message_buffer[MAXIMUM_BUFFER_SIZE]; // the message buffer
    Data _message;                                      // The data received
    AsynchSerial _esp_serial_port;                      // Serial port to the wifi client
    Thread _thread;                                     // Thread for controlling thread functions in the uart communication.
    EventQueue _queue;                                  // flexible queue for scheduling events.
    void _tx_callback();                                // Callback function for the transmit line
    void _rx_callback();                                // Callback function for the receive line
    void _get_data();                                   // Gets data from queue

public:
    Uart(PinName rx, PinName tx, int baudrate);
    void init();        // Initializes the serial communication
    Data ReceiveData(); // Receive data from serial communication after formating data
    ~Uart();
};

#endif
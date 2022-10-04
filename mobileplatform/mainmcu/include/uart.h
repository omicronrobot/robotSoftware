#ifndef UART_H
#define UART_H

#include "defs.h"

class Uart
{
private:
    /* data */
    int rx_pin;
    int tx_pin;
    char format_data(Data data);
public:
    Uart(/* args */);
    void init(int rx, int tx, int baudrate);
    void ReceiveData(Data data);
    ~Uart();
};


#endif
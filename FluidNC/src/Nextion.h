// NextionChannel.h
#pragma once

#include "UartChannel.h"

class Nextion : public UartChannel {
protected:
    Lineedit* _lineedit;
    Uart*     _uart;

    int _uart_num           = 0;
    int _report_interval_ms = 100;

    // size_t writeNextionFormatted(const uint8_t* buffer, size_t size);  // Nextion-specific formatting

public:
    // void init();
    using UartChannel::UartChannel;  // Makes UartChannel's constructors available in Nextion
    // Nextion::Nextion(int num, bool addCR) : UartChannel(num, addCR) {};

    void init();
    void init(Uart* uart);

    size_t write(uint8_t byte) override;                        // Have to override this :(
    size_t write(const uint8_t* buffer, size_t size) override;  // Override write method

    // Configuration methods
    void group(Configuration::HandlerBase& handler) override {
        handler.item("report_interval_ms", _report_interval_ms);
        handler.item("uart_num", _uart_num);
        // handler.item("message_level", _message_level, messageLevels2);
    }
};
extern UartChannel Uart0;
// extern void uartInit();
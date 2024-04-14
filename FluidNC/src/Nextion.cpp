// NextionChannel.cpp
#include "Nextion.h"
#include "Machine/MachineConfig.h"  // config

// Nextion::Nextion(int ignore) {  // Constructor implementation, if needed
//     log_info("Hello from nextion contructior");
// }

size_t Nextion::write(uint8_t c) {
    return _uart->write(c);
}

// size_t Nextion::write(const uint8_t* buffer, size_t size) {
//     // Override the write method to format data for the Nextion display
//     const uint8_t prefix[]            = "t0.val=\"";
//     const uint8_t suffix[]            = { '"', 0xFF, 0xFF, 0xFF };
//     int           aggregateBufferSize = 0;
//     aggregateBufferSize += _uart->write(prefix, sizeof(prefix));
//     aggregateBufferSize += _uart->write(buffer, size);
//     aggregateBufferSize += _uart->write(suffix, sizeof(suffix));
//     return aggregateBufferSize;
// }

void Nextion::init() {
    auto uart = config->_uarts[_uart_num];
    if (uart) {
        init(uart);
    } else {
        log_error("Nextion: missing uart" << _uart_num);
    }
    setReportInterval(_report_interval_ms);
}
void Nextion::init(Uart* uart) {
    _uart = uart;
    log_info("Nextion: creating for uart" << _uart_num);
    allChannels.registration(this);
    if (_report_interval_ms) {
        log_info("Nextion" << _uart_num << " created at report interval: " << _report_interval_ms);
    } else {
        log_info("Nextion" << _uart_num << " created");
    }
    log_info("Nextion: Sending 'RST' with log_msg_to()");
    log_msg_to(*this, "RST");

    // Give the extender a little time to process the command
    log_info("Nextion: Delay for 100ms");
    delay(100);
    log_info("End of Nextion init");
}

size_t Nextion::write(const uint8_t* buffer, size_t length) {
    // Replace \n with \r\n
    log_info("Nextion write called");
    if (_addCR) {
        size_t rem      = length;
        char   lastchar = '\0';
        size_t j        = 0;
        while (rem) {
            const int bufsize = 80;
            uint8_t   modbuf[bufsize];
            // bufsize-1 in case the last character is \n
            size_t k = 0;
            while (rem && k < (bufsize - 1)) {
                char c = buffer[j++];
                if (c == '\n' && lastchar != '\r') {
                    modbuf[k++] = '\r';
                }
                lastchar    = c;
                modbuf[k++] = c;
                --rem;
            }
            _uart->write(modbuf, k);
        }
        return length;
    } else {
        return _uart->write(buffer, length);
    }
}

// void uartInit() {
//     // Do nothing to avoid this getting called when the uartChannel uartInit() is called
// }
// size_t Nextion::writeNextionFormatted(const uint8_t* buffer, size_t size) {
//     // Example formatting logic; adapt as needed for your specific data and commands
//     return 0;
// }

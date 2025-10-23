#include "uart_driver.h"
#include <iostream>
#include <iomanip>

void printBuffer(const char* label, const uint8_t* buffer, size_t length) {
    std::cout << label << ": ";
    for (size_t i = 0; i < length; i++) {
        std::cout << "0x" << std::hex << std::setw(2) << std::setfill('0') 
                  << static_cast<int>(buffer[i]) << " ";
    }
    std::cout << std::dec << std::endl;
}

int main() {
    std::cout << "UART Driver Demo" << std::endl;
    std::cout << "=================" << std::endl << std::endl;
    
    uart::UARTDriver uart;
    
    // Initialize UART
    std::cout << "Initializing UART at 115200 baud..." << std::endl;
    if (!uart.initialize(115200, true, false)) {
        std::cerr << "Failed to initialize UART!" << std::endl;
        return 1;
    }
    std::cout << "UART initialized successfully" << std::endl << std::endl;
    
    // Transmit some data
    std::cout << "Transmitting data..." << std::endl;
    const uint8_t tx_message[] = "Hello UART!";
    size_t tx_len = sizeof(tx_message) - 1;  // Exclude null terminator
    
    size_t written = uart.writeData(tx_message, tx_len);
    std::cout << "Queued " << written << " bytes for transmission" << std::endl;
    std::cout << "TX FIFO count: " << uart.getTxFifoCount() << std::endl;
    
    printBuffer("TX Data", tx_message, tx_len);
    std::cout << std::endl;
    
    // Simulate transmission
    std::cout << "Simulating transmission..." << std::endl;
    uart.simulateTransmit(written);
    std::cout << "TX FIFO count after transmit: " << uart.getTxFifoCount() << std::endl;
    std::cout << std::endl;
    
    // Simulate receiving data
    std::cout << "Simulating data reception..." << std::endl;
    const uint8_t rx_data[] = {0x48, 0x65, 0x6C, 0x6C, 0x6F};  // "Hello"
    uart.simulateReceive(rx_data, sizeof(rx_data));
    
    std::cout << "RX FIFO count: " << uart.getRxFifoCount() << std::endl;
    std::cout << "Has data: " << (uart.hasData() ? "Yes" : "No") << std::endl;
    std::cout << std::endl;
    
    // Read received data
    std::cout << "Reading received data..." << std::endl;
    uint8_t rx_buffer[20];
    size_t read = uart.readData(rx_buffer, sizeof(rx_buffer));
    std::cout << "Read " << read << " bytes" << std::endl;
    
    printBuffer("RX Data", rx_buffer, read);
    std::cout << std::endl;
    
    // Error handling demo
    std::cout << "Testing error conditions..." << std::endl;
    
    // Try to overfill the RX FIFO
    uint8_t large_data[20];
    for (int i = 0; i < 20; i++) {
        large_data[i] = i;
    }
    
    uart.simulateReceive(large_data, 20);
    
    if (uart.hasError()) {
        std::cout << "ERROR: RX overrun detected (expected)" << std::endl;
        uart.clearErrors();
        std::cout << "Errors cleared" << std::endl;
    }
    std::cout << std::endl;
    
    // Shutdown
    std::cout << "Shutting down UART..." << std::endl;
    uart.shutdown();
    std::cout << "UART shutdown complete" << std::endl;
    
    return 0;
}

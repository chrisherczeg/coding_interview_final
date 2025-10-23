#include "uart_driver.h"
#include <iostream>
#include <cstring>

namespace uart {
namespace test {

// Simple test framework
int tests_run = 0;
int tests_passed = 0;
int tests_failed = 0;

void reportTest(const char* name, bool passed) {
    tests_run++;
    if (passed) {
        tests_passed++;
        std::cout << "[PASS] " << name << std::endl;
    } else {
        tests_failed++;
        std::cout << "[FAIL] " << name << std::endl;
    }
}

#define TEST(name, condition) \
    reportTest(name, (condition))

// Basic functionality tests
void testInitialization() {
    std::cout << "\n=== Basic Initialization Tests ===" << std::endl;
    
    UARTDriver uart;
    TEST("Initialize UART", uart.initialize(115200));
    TEST("Can transmit after init", uart.canTransmit());
    TEST("No data after init", !uart.hasData());
    TEST("No errors after init", !uart.hasError());
    TEST("TX FIFO count is 0", uart.getTxFifoCount() == 0);
    TEST("RX FIFO count is 0", uart.getRxFifoCount() == 0);
}

void testSingleByteTransmit() {
    std::cout << "\n=== Single Byte Transmit Tests ===" << std::endl;
    
    UARTDriver uart;
    uart.initialize(115200);
    
    TEST("Write single byte", uart.writeByte(0x42));
    TEST("TX FIFO count is 1", uart.getTxFifoCount() == 1);
    TEST("Can still transmit", uart.canTransmit());
    
    uart.simulateTransmit(1);
    TEST("TX FIFO count is 0 after transmit", uart.getTxFifoCount() == 0);
}

void testSingleByteReceive() {
    std::cout << "\n=== Single Byte Receive Tests ===" << std::endl;
    
    UARTDriver uart;
    uart.initialize(115200);
    
    uint8_t test_data = 0x55;
    uart.simulateReceive(&test_data, 1);
    
    TEST("Has data after receive", uart.hasData());
    TEST("RX FIFO count is 1", uart.getRxFifoCount() == 1);
    
    uint8_t received = 0;
    TEST("Read byte succeeds", uart.readByte(received));
    TEST("Received data matches", received == 0x55);
    TEST("No data after read", !uart.hasData());
}

void testMultiByteOperations() {
    std::cout << "\n=== Multi-Byte Operations Tests ===" << std::endl;
    
    UARTDriver uart;
    uart.initialize(115200);
    
    const uint8_t tx_data[] = {0x01, 0x02, 0x03, 0x04, 0x05};
    size_t written = uart.writeData(tx_data, 5);
    
    TEST("Write 5 bytes", written == 5);
    TEST("TX FIFO count is 5", uart.getTxFifoCount() == 5);
    
    const uint8_t rx_data[] = {0xAA, 0xBB, 0xCC};
    uart.simulateReceive(rx_data, 3);
    
    TEST("RX FIFO count is 3", uart.getRxFifoCount() == 3);
    
    uint8_t buffer[10];
    size_t read = uart.readData(buffer, 10);
    
    TEST("Read 3 bytes", read == 3);
    TEST("Data byte 0 correct", buffer[0] == 0xAA);
    TEST("Data byte 1 correct", buffer[1] == 0xBB);
    TEST("Data byte 2 correct", buffer[2] == 0xCC);
}

void testParityConfiguration() {
    std::cout << "\n=== Parity Configuration Tests ===" << std::endl;
    
    UARTDriver uart1;
    uart1.initialize(115200, false, false);
    TEST("Initialize without parity", uart1.canTransmit());
    
    UARTDriver uart2;
    uart2.initialize(115200, true, false);
    TEST("Initialize with even parity", uart2.canTransmit());
    
    UARTDriver uart3;
    uart3.initialize(115200, true, true);
    TEST("Initialize with odd parity", uart3.canTransmit());
}

void testShutdown() {
    std::cout << "\n=== Shutdown Tests ===" << std::endl;
    
    UARTDriver uart;
    uart.initialize(115200);
    uart.writeByte(0x42);
    
    uart.shutdown();
    
    TEST("Cannot transmit after shutdown", !uart.canTransmit());
    TEST("Cannot write after shutdown", !uart.writeByte(0x55));
}

int runBasicTests() {
    std::cout << "\n========================================" << std::endl;
    std::cout << "Running Basic UART Driver Tests" << std::endl;
    std::cout << "========================================" << std::endl;
    
    testInitialization();
    testSingleByteTransmit();
    testSingleByteReceive();
    testMultiByteOperations();
    testParityConfiguration();
    testShutdown();
    
    return tests_failed;
}

} // namespace test
} // namespace uart

#ifndef UART_DRIVER_H
#define UART_DRIVER_H

#include "uart_registers.h"
#include <cstdint>
#include <cstddef>

namespace uart {

/**
 * @brief UART driver for simulated hardware peripheral
 * 
 * This driver provides an interface to a simulated UART hardware block.
 * It includes TX/RX FIFOs and handles typical UART operations.
 */
class UARTDriver {
public:
    UARTDriver();
    ~UARTDriver();
    
    /**
     * @brief Initialize the UART with specified configuration
     * @param baud_rate Baud rate divisor
     * @param enable_parity Enable parity checking
     * @param odd_parity Use odd parity (true) or even parity (false)
     * @return true if initialization successful
     */
    bool initialize(uint32_t baud_rate, bool enable_parity = false, bool odd_parity = false);
    
    /**
     * @brief Shutdown the UART
     */
    void shutdown();
    
    /**
     * @brief Write a single byte to TX FIFO
     * @param data Byte to transmit
     * @return true if byte was queued, false if TX FIFO full
     */
    bool writeByte(uint8_t data);
    
    /**
     * @brief Write multiple bytes to TX FIFO
     * @param data Pointer to data buffer
     * @param length Number of bytes to write
     * @return Number of bytes successfully queued
     */
    size_t writeData(const uint8_t* data, size_t length);
    
    /**
     * @brief Read a single byte from RX FIFO
     * @param data Pointer to store received byte
     * @return true if byte was read, false if RX FIFO empty
     */
    bool readByte(uint8_t& data);
    
    /**
     * @brief Read multiple bytes from RX FIFO
     * @param buffer Pointer to destination buffer
     * @param max_length Maximum bytes to read
     * @return Number of bytes actually read
     */
    size_t readData(uint8_t* buffer, size_t max_length);
    
    /**
     * @brief Check if TX FIFO has space
     * @return true if TX FIFO can accept more data
     */
    bool canTransmit() const;
    
    /**
     * @brief Check if RX FIFO has data
     * @return true if data is available to read
     */
    bool hasData() const;
    
    /**
     * @brief Get number of bytes in TX FIFO
     */
    size_t getTxFifoCount() const;
    
    /**
     * @brief Get number of bytes in RX FIFO
     */
    size_t getRxFifoCount() const;
    
    /**
     * @brief Check for errors
     * @return true if any error flags are set
     */
    bool hasError() const;
    
    /**
     * @brief Clear error flags
     */
    void clearErrors();
    
    /**
     * @brief Simulate receiving data (for testing)
     * This simulates data arriving from the external device
     */
    void simulateReceive(const uint8_t* data, size_t length);
    
    /**
     * @brief Simulate transmission completion (for testing)
     * This moves data from TX FIFO to simulate actual transmission
     */
    void simulateTransmit(size_t num_bytes);
    
private:
    UARTRegisters registers;
    
    // TX FIFO
    uint8_t tx_fifo[FIFO_DEPTH];
    size_t tx_head;
    size_t tx_tail;
    size_t tx_count;
    
    // RX FIFO
    uint8_t rx_fifo[FIFO_DEPTH];
    size_t rx_head;
    size_t rx_tail;
    size_t rx_count;
    
    // Helper functions
    void updateStatusFlags();
    bool txFifoFull() const;
    bool txFifoEmpty() const;
    bool rxFifoFull() const;
    bool rxFifoEmpty() const;
};

} // namespace uart

#endif // UART_DRIVER_H

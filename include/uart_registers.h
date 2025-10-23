#ifndef UART_REGISTERS_H
#define UART_REGISTERS_H

#include <cstdint>

namespace uart {

// UART Register offsets
constexpr uint32_t UART_DATA_REG     = 0x00;  // Data register (TX/RX)
constexpr uint32_t UART_STATUS_REG   = 0x04;  // Status register
constexpr uint32_t UART_CONTROL_REG  = 0x08;  // Control register
constexpr uint32_t UART_BAUD_REG     = 0x0C;  // Baud rate divisor

// Status register bits
constexpr uint32_t STATUS_TX_EMPTY   = (1 << 0);  // TX FIFO empty
constexpr uint32_t STATUS_TX_FULL    = (1 << 1);  // TX FIFO full
constexpr uint32_t STATUS_RX_EMPTY   = (1 << 2);  // RX FIFO empty
constexpr uint32_t STATUS_RX_FULL    = (1 << 3);  // RX FIFO full
constexpr uint32_t STATUS_FRAME_ERR  = (1 << 4);  // Frame error
constexpr uint32_t STATUS_OVERRUN    = (1 << 5);  // RX overrun error

// Control register bits
constexpr uint32_t CTRL_ENABLE       = (1 << 0);  // Enable UART
constexpr uint32_t CTRL_TX_ENABLE    = (1 << 1);  // Enable transmitter
constexpr uint32_t CTRL_RX_ENABLE    = (1 << 2);  // Enable receiver
constexpr uint32_t CTRL_PARITY_EN    = (1 << 3);  // Enable parity
constexpr uint32_t CTRL_PARITY_ODD   = (1 << 4);  // Odd parity (0=even)

// FIFO depth
constexpr size_t FIFO_DEPTH = 16;

/**
 * @brief Simulates UART hardware registers
 * 
 * This class models a typical UART peripheral with memory-mapped registers.
 * In real hardware, these would be physical registers at specific addresses.
 */
class UARTRegisters {
public:
    UARTRegisters();
    
    // Register access
    void writeRegister(uint32_t offset, uint32_t value);
    uint32_t readRegister(uint32_t offset) const;
    
    // Direct status flag access for internal use
    void setStatusBit(uint32_t bit);
    void clearStatusBit(uint32_t bit);
    bool isStatusBitSet(uint32_t bit) const;
    
    // Control register queries
    bool isEnabled() const;
    bool isTxEnabled() const;
    bool isRxEnabled() const;
    bool isParityEnabled() const;
    bool isParityOdd() const;
    
    // Reset to power-on state
    void reset();
    
private:
    uint32_t data_reg;
    uint32_t status_reg;
    uint32_t control_reg;
    uint32_t baud_reg;
};

} // namespace uart

#endif // UART_REGISTERS_H

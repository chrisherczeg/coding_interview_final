#include "uart_driver.h"
#include <cstring>

namespace uart {

UARTDriver::UARTDriver()
    : tx_head(0)
    , tx_tail(0)
    , tx_count(0)
    , rx_head(0)
    , rx_tail(0)
    , rx_count(0) {
    // Initialize FIFOs
    memset(tx_fifo, 0, sizeof(tx_fifo));
    memset(rx_fifo, 0, sizeof(rx_fifo));
}

UARTDriver::~UARTDriver() {
    shutdown();
}

bool UARTDriver::initialize(uint32_t baud_rate, bool enable_parity, bool odd_parity) {
    // Reset hardware state
    registers.reset();
    
    // Configure baud rate
    registers.writeRegister(UART_BAUD_REG, baud_rate);
    
    // Configure control register
    uint32_t ctrl = CTRL_ENABLE | CTRL_TX_ENABLE | CTRL_RX_ENABLE;
    if (enable_parity) {
        ctrl |= CTRL_PARITY_EN;
        if (odd_parity) {
            ctrl |= CTRL_PARITY_ODD;
        }
    }
    registers.writeRegister(UART_CONTROL_REG, ctrl);
    
    // Clear FIFOs
    tx_head = tx_tail = tx_count = 0;
    rx_head = rx_tail = rx_count = 0;
    
    updateStatusFlags();
    
    return registers.isEnabled();
}

void UARTDriver::shutdown() {
    registers.writeRegister(UART_CONTROL_REG, 0);
    tx_head = tx_tail = tx_count = 0;
    rx_head = rx_tail = rx_count = 0;
}

bool UARTDriver::writeByte(uint8_t data) {
    if (!registers.isTxEnabled() || txFifoFull()) {
        return false;
    }
    
    // Add byte to TX FIFO
    tx_fifo[tx_head] = data;
    tx_head = (tx_head + 1) % FIFO_DEPTH;
    tx_count++;
    
    updateStatusFlags();
    return true;
}

size_t UARTDriver::writeData(const uint8_t* data, size_t length) {
    if (!data || !registers.isTxEnabled()) {
        return 0;
    }
    
    size_t written = 0;
    for (size_t i = 0; i < length; i++) {
        if (!writeByte(data[i])) {
            break;
        }
        written++;
    }
    
    return written;
}

bool UARTDriver::readByte(uint8_t& data) {
    if (!registers.isRxEnabled() || rxFifoEmpty()) {
        return false;
    }
    
    // Read byte from RX FIFO
    data = rx_fifo[rx_tail];
    rx_tail = (rx_tail + 1) % FIFO_DEPTH;
    rx_count--;
    
    updateStatusFlags();
    return true;
}

size_t UARTDriver::readData(uint8_t* buffer, size_t max_length) {
    if (!buffer || !registers.isRxEnabled()) {
        return 0;
    }
    
    size_t read = 0;
    for (size_t i = 0; i < max_length; i++) {
        if (!readByte(buffer[i])) {
            break;
        }
        read++;
    }
    
    return read;
}

bool UARTDriver::canTransmit() const {
    return registers.isTxEnabled() && !txFifoFull();
}

bool UARTDriver::hasData() const {
    return registers.isRxEnabled() && !rxFifoEmpty();
}

size_t UARTDriver::getTxFifoCount() const {
    return tx_count;
}

size_t UARTDriver::getRxFifoCount() const {
    return rx_count;
}

bool UARTDriver::hasError() const {
    return registers.isStatusBitSet(STATUS_FRAME_ERR | STATUS_OVERRUN);
}

void UARTDriver::clearErrors() {
    // Write 1 to clear error bits
    registers.writeRegister(UART_STATUS_REG, STATUS_FRAME_ERR | STATUS_OVERRUN);
}

void UARTDriver::simulateReceive(const uint8_t* data, size_t length) {
    if (!data || !registers.isRxEnabled()) {
        return;
    }
    
    for (size_t i = 0; i < length; i++) {
        if (rxFifoFull()) {
            // Set overrun error if FIFO is full
            registers.setStatusBit(STATUS_OVERRUN);
            break;
        }
        
        // Add byte to RX FIFO
        rx_fifo[rx_head] = data[i];
        rx_head = (rx_head + 1) % FIFO_DEPTH;
        rx_count++;
    }
    
    updateStatusFlags();
}

void UARTDriver::simulateTransmit(size_t num_bytes) {
    if (!registers.isTxEnabled()) {
        return;
    }
    
    size_t to_transmit = (num_bytes < tx_count) ? num_bytes : tx_count;
    
    for (size_t i = 0; i < to_transmit; i++) {
        // Simulate transmitting the byte (just remove from FIFO)
        tx_tail = (tx_tail + 1) % FIFO_DEPTH;
        tx_count--;
    }
    
    updateStatusFlags();
}

// Private helper functions

void UARTDriver::updateStatusFlags() {
    // Update TX status flags
    if (txFifoEmpty()) {
        registers.setStatusBit(STATUS_TX_EMPTY);
        registers.clearStatusBit(STATUS_TX_FULL);
    } else if (txFifoFull()) {
        registers.clearStatusBit(STATUS_TX_EMPTY);
        registers.setStatusBit(STATUS_TX_FULL);
    } else {
        registers.clearStatusBit(STATUS_TX_EMPTY);
        registers.clearStatusBit(STATUS_TX_FULL);
    }
    
    // Update RX status flags
    if (rxFifoEmpty()) {
        registers.setStatusBit(STATUS_RX_EMPTY);
        registers.clearStatusBit(STATUS_RX_FULL);
    } else if (rxFifoFull()) {
        registers.clearStatusBit(STATUS_RX_EMPTY);
        registers.setStatusBit(STATUS_RX_FULL);
    } else {
        registers.clearStatusBit(STATUS_RX_EMPTY);
        registers.clearStatusBit(STATUS_RX_FULL);
    }
}

bool UARTDriver::txFifoFull() const {
    return tx_count > FIFO_DEPTH;
}

bool UARTDriver::txFifoEmpty() const {
    return tx_count == 0;
}

bool UARTDriver::rxFifoFull() const {
    return rx_count > FIFO_DEPTH;
}

bool UARTDriver::rxFifoEmpty() const {
    return rx_count == 0;
}

} // namespace uart

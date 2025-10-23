#include "uart_registers.h"

namespace uart {

UARTRegisters::UARTRegisters() 
    : data_reg(0)
    , status_reg(STATUS_TX_EMPTY | STATUS_RX_EMPTY)  // FIFOs empty on reset
    , control_reg(0)
    , baud_reg(0) {
}

void UARTRegisters::writeRegister(uint32_t offset, uint32_t value) {
    switch (offset) {
        case UART_DATA_REG:
            data_reg = value & 0xFF;  // Only 8 bits for data
            break;
        case UART_STATUS_REG:
            // Status register is read-only except for error bits which are W1C (write-1-to-clear)
            status_reg &= ~(value & (STATUS_FRAME_ERR | STATUS_OVERRUN));
            break;
        case UART_CONTROL_REG:
            control_reg = value;
            break;
        case UART_BAUD_REG:
            baud_reg = value;
            break;
        default:
            // Invalid register offset - ignore
            break;
    }
}

uint32_t UARTRegisters::readRegister(uint32_t offset) const {
    switch (offset) {
        case UART_DATA_REG:
            return data_reg;
        case UART_STATUS_REG:
            return status_reg;
        case UART_CONTROL_REG:
            return control_reg;
        case UART_BAUD_REG:
            return baud_reg;
        default:
            return 0;  // Invalid register reads return 0
    }
}

void UARTRegisters::setStatusBit(uint32_t bit) {
    status_reg |= bit;
}

void UARTRegisters::clearStatusBit(uint32_t bit) {
    status_reg &= ~bit;
}

bool UARTRegisters::isStatusBitSet(uint32_t bit) const {
    return (status_reg & bit) != 0;
}

bool UARTRegisters::isEnabled() const {
    return (control_reg & CTRL_ENABLE) != 0;
}

bool UARTRegisters::isTxEnabled() const {
    return (control_reg & CTRL_TX_ENABLE) != 0;
}

bool UARTRegisters::isRxEnabled() const {
    return (control_reg & CTRL_RX_ENABLE) != 0;
}

bool UARTRegisters::isParityEnabled() const {
    return (control_reg & CTRL_PARITY_EN) != 0;
}

bool UARTRegisters::isParityOdd() const {
    return (control_reg & CTRL_PARITY_ODD) != 0;
}

void UARTRegisters::reset() {
    data_reg = 0;
    status_reg = STATUS_TX_EMPTY | STATUS_RX_EMPTY;
    control_reg = 0;
    baud_reg = 0;
}

} // namespace uart

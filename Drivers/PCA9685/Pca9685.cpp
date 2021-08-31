// ======================================================================
// \title  PCA9685ComponentImpl.cpp
// \author starchmd
// \brief  cpp file for PCA9685 component implementation class
// ======================================================================

#include <Drivers/PCA9685/Pca9685.hpp>
#include "Fw/Types/BasicTypes.hpp"
#include "Fw/Types/Assert.hpp"

namespace GwcRobot {

// ----------------------------------------------------------------------
// Construction, initialization, and destruction
// ----------------------------------------------------------------------

Pca9685ComponentImpl ::Pca9685ComponentImpl(const char* const compName) : Pca9685ComponentBase(compName), m_address(0) {}

void Pca9685ComponentImpl ::init(const NATIVE_INT_TYPE instance) {
    Pca9685ComponentBase::init(instance);
}

Pca9685ComponentImpl ::~Pca9685ComponentImpl() {}

void Pca9685ComponentImpl ::startup(U8 address) {
    m_address = address;

    U8 data[2];
    Fw::Buffer buffer(data, sizeof(data));
    buffer.getData()[0] = MODE_REG;
    buffer.getData()[1] = 0x10; // Mode 1, set to sleep
    i2cWrite_out(0, m_address, buffer);

    // Set the mode register to sleep (in preparation to set the mode register)
    buffer.getData()[0] = MODE_REG;
    buffer.getData()[1] = 0x10; // Mode 1
    i2cWrite_out(0, m_address, buffer);

    // Set the prescale value
    buffer.getData()[0] = PRESCALE_REG;
    buffer.getData()[1] = PRESCALE;
    i2cWrite_out(0, m_address, buffer);

    // No sleep, reset, and enable autoincrement
    buffer.getData()[0] = MODE_REG;
    buffer.getData()[1] = 0xA0; // Mode 1
    i2cWrite_out(0, m_address, buffer);

    Os::Task::delay(1); // At least 500us
}

// ----------------------------------------------------------------------
// Handler implementations for user-defined typed input ports
// ----------------------------------------------------------------------

void Pca9685ComponentImpl ::pwm_handler(const NATIVE_INT_TYPE portNum, U16 duty_cycle) {
    FW_ASSERT(duty_cycle < 0x1000);
    U8 reg = (portNum * 4) + PWM_REG_BASE;
    // Word 0
    U16 on = (duty_cycle != 0xFFF) ? 0 : 0x1000;
    // Word 1
    U16 off = (duty_cycle != 0xFFF) ? duty_cycle : 0;

    // Create a sized message
    U8 data[sizeof(U8) + sizeof(U16) + sizeof(U16)];
    data[0] = reg;
    // Serialize two words in little endian format
    data[1] = static_cast<U8>(on);
    data[2] = static_cast<U8>(on >> 8);
    data[3] = static_cast<U8>(off);
    data[4] = static_cast<U8>(off >> 8);

    // Wrap it in a buffer for transport
    Fw::Buffer buffer(data, sizeof(data));
    i2cWrite_out(0, m_address, buffer);
}

}  // end namespace GwcRobot

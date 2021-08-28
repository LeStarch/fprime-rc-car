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

Pca9685ComponentImpl ::~Pca9685ComponentImpl(void) {}

 void Pca9685ComponentImpl ::configure(U8 address) {
    m_address = address;
}

// ----------------------------------------------------------------------
// Handler implementations for user-defined typed input ports
// ----------------------------------------------------------------------

void Pca9685ComponentImpl ::pwm_handler(const NATIVE_INT_TYPE portNum, U16 duty_cycle) {
    FW_ASSERT(duty_cycle < 0x1000);
    U8 reg = (portNum * 4) + 6;
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

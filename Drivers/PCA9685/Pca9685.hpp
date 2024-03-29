// ======================================================================
// \title  PCA9685ComponentImpl.hpp
// \author starchmd
// \brief  hpp file for PCA9685 component implementation class
// ======================================================================

#ifndef PCA9685_HPP
#define PCA9685_HPP

#include "Drivers/PCA9685/Pca9685ComponentAc.hpp"

namespace GwcRobot {

class Pca9685ComponentImpl : public Pca9685ComponentBase {
  public:
    // ----------------------------------------------------------------------
    // Construction, initialization, and destruction
    // ----------------------------------------------------------------------
    static const U8 PRESCALE = 3; //25000000 (clock frequence) / 4096 (counter size) / 1600 (PWM frequency) - 1

    static const U8 MODE_REG = 0x00;
    static const U8 PRESCALE_REG = 0xFE;
    static const U8 PWM_REG_BASE = 0x06;

    //! Construct object PCA9685
    //!
    Pca9685ComponentImpl(const char* const compName /*!< The component name*/
    );

    //! Initialize object PCA9685
    //!
    void init(const NATIVE_INT_TYPE instance = 0 /*!< The instance number*/
    );

    void startup(U8 address);

    //! Destroy object PCA9685
    //!
    ~Pca9685ComponentImpl(void);

  private:
    // ----------------------------------------------------------------------
    // Handler implementations for user-defined typed input ports
    // ----------------------------------------------------------------------

    //! Handler implementation for pwm
    //!
    void pwm_handler(const NATIVE_INT_TYPE portNum, /*!< The port number*/
                     U16 duty_cycle                 /*!< Duty cycle of the PWM output*/
    );
    U8 m_address;
};

}  // end namespace GwcRobot

#endif

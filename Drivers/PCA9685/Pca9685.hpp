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

    //! Construct object PCA9685
    //!
    Pca9685ComponentImpl(const char* const compName /*!< The component name*/
    );

    //! Initialize object PCA9685
    //!
    void init(const NATIVE_INT_TYPE instance = 0 /*!< The instance number*/
    );

    void configure(U8 address);

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

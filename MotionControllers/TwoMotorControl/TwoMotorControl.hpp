// ======================================================================
// \title  TwoMotorControlComponentImpl.hpp
// \author starchmd
// \brief  hpp file for TwoMotorControl component implementation class
//
// \copyright
// Copyright 2009-2015, by the California Institute of Technology.
// ALL RIGHTS RESERVED.  United States Government Sponsorship
// acknowledged.
//
// ======================================================================

#ifndef TwoMotorControl_HPP
#define TwoMotorControl_HPP

#include "MotionControllers/TwoMotorControl/TwoMotorControlComponentAc.hpp"

namespace GwcRobot {

class TwoMotorControlComponentImpl : public TwoMotorControlComponentBase {
  public:
    // ----------------------------------------------------------------------
    // Construction, initialization, and destruction
    // ----------------------------------------------------------------------

    //! Construct object TwoMotorControl
    //!
    TwoMotorControlComponentImpl(const char* const compName /*!< The component name*/
    );

    //! Initialize object TwoMotorControl
    //!
    void init(const NATIVE_INT_TYPE queueDepth,  /*!< The queue depth*/
              const NATIVE_INT_TYPE instance = 0 /*!< The instance number*/
    );

    //! Destroy object TwoMotorControl
    //!
    ~TwoMotorControlComponentImpl(void);

  private:
    //! Helper to experiment with motion
    //!
    void motion_helper();

    // ----------------------------------------------------------------------
    // Command handler implementations
    // ----------------------------------------------------------------------

    //! Implementation for GO command handler
    //!
    void GO_cmdHandler(const FwOpcodeType opCode, /*!< The opcode*/
                       const U32 cmdSeq           /*!< The command sequence number*/
    );
};

}  // end namespace GwcRobot

#endif

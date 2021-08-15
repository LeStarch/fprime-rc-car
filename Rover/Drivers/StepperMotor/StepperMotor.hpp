// ======================================================================
// \title  StepperMotorComponentImpl.hpp
// \author starchmd
// \brief  hpp file for StepperMotor component implementation class
//
// \copyright
// Copyright 2009-2015, by the California Institute of Technology.
// ALL RIGHTS RESERVED.  United States Government Sponsorship
// acknowledged.
//
// ======================================================================

#ifndef StepperMotor_HPP
#define StepperMotor_HPP

#include "Rover/Drivers/StepperMotor/StepperMotorComponentAc.hpp"

namespace Rover {

    class StepperMotorComponentImpl :
            public StepperMotorComponentBase {

    public:

        // ----------------------------------------------------------------------
        // Construction, initialization, and destruction
        // ----------------------------------------------------------------------

        //! Construct object StepperMotor
        //!
        StepperMotorComponentImpl(
                const char *const compName /*!< The component name*/
        );

        //! Initialize object StepperMotor
        //!
        void init(
                const NATIVE_INT_TYPE instance = 0 /*!< The instance number*/
        );

        //! Destroy object StepperMotor
        //!
        ~StepperMotorComponentImpl(void);

        PRIVATE:

                // ----------------------------------------------------------------------
                // Handler implementations for user-defined typed input ports
                // ----------------------------------------------------------------------

                //! Handler implementation for spin
                //!
                Status

        spin_handler(
                const NATIVE_INT_TYPE portNum, /*!< The port number*/
                Direction direction, /*!< Direction of the spin*/
                U8 speed, /*!< Speed of the spin*/
                F32 revolutions /*!< Count of the revolutions to spin*/
        );

        //! Handler implementation for stop
        //!
        Status stop_handler(
                const NATIVE_INT_TYPE portNum /*!< The port number*/
        );

        //! Handler implementation for run
        //!
        void run_handler(
                const NATIVE_INT_TYPE portNum, /*!< The port number*/
                NATIVE_UINT_TYPE context /*!< The call order*/
        );


    };

} // end namespace Rover

#endif

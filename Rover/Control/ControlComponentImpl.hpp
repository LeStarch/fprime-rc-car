// ======================================================================
// \title  ControlComponentImpl.hpp
// \author starchmd
// \brief  hpp file for Control component implementation class
//
// \copyright
// Copyright 2009-2015, by the California Institute of Technology.
// ALL RIGHTS RESERVED.  United States Government Sponsorship
// acknowledged.
//
// ======================================================================

#ifndef Control_HPP
#define Control_HPP

#include <Fw/Types/BasicTypes.hpp>
#include "Rover/Control/ControlComponentAc.hpp"

namespace Rover {

  class ControlComponentImpl :
    public ControlComponentBase
  {

    public:
      /**
       * Internal enumeration items for the actions this can take.
       */
      enum InternalAction {
          MOVE_IDLE_INT,
          MOVE_BREAK_INT,
          MOVE_FORWARD_INT,
          MOVE_BACKWARD_INT,
          MOVE_FORWARD_LEFT_INT,
          MOVE_FORWARD_RIGHT_INT,
          MOVE_BACKWARD_LEFT_INT,
          MOVE_BACKWARD_RIGHT_INT,
          MOVE_FORCED_COOLOFF_INT,
      };
      // ----------------------------------------------------------------------
      // Construction, initialization, and destruction
      // ----------------------------------------------------------------------

      //! Construct object Control
      //!
      ControlComponentImpl(
#if FW_OBJECT_NAMES == 1
          const char *const compName /*!< The component name*/
#else
          void
#endif
      );

      //! Initialize object Control
      //!
      void init(
          const NATIVE_INT_TYPE queueDepth, /*!< The queue depth*/
          const NATIVE_INT_TYPE instance = 0 /*!< The instance number*/
      );

      //! Destroy object Control
      //!
      ~ControlComponentImpl(void);

    PRIVATE:

      //! Clocked function at 100Hz
      void clock_100hz();
      // ----------------------------------------------------------------------
      // Handler implementations for user-defined typed input ports
      // ----------------------------------------------------------------------

      //! Handler implementation for Run
      //!
      void Run_handler(
          const NATIVE_INT_TYPE portNum, /*!< The port number*/
          NATIVE_UINT_TYPE context /*!< The call order*/
      );

    PRIVATE:

      // ----------------------------------------------------------------------
      // Command handler implementations
      // ----------------------------------------------------------------------

      //! Implementation for ROVER_MOVE command handler
      //! Controls the rover to move with a certain direction and power. Power controls a
      //! software PWM for the primary engine. This will also accept a duration for the action
      //! in milliseconds. Zero duration will run forever.
      void ROVER_MOVE_cmdHandler(
          const FwOpcodeType opCode, /*!< The opcode*/
          const U32 cmdSeq, /*!< The command sequence number*/
          MoveDirection action, /*!< Directionality of the movement.*/
          U8 power, /*!< Power for movement 0-100.*/
          U32 duration /*!< Duration in milliseconds to perform this action. A zero will run the action forever.*/
      );

      //! Implementation for ROVER_HEADLIGHT command handler
      //! Controls the rover headlights.
      void ROVER_HEADLIGHT_cmdHandler(
          const FwOpcodeType opCode, /*!< The opcode*/
          const U32 cmdSeq, /*!< The command sequence number*/
          HeadlightsOnOff action /*!< State to command headlights*/
      );

      //! Implementation for ROVER_SNAPSHOT command handler
      //! Controls the rover headlights.
      void ROVER_SNAPSHOT_cmdHandler(
          const FwOpcodeType opCode, /*!< The opcode*/
          const U32 cmdSeq, /*!< The command sequence number*/
          const Fw::CmdStringArg& action /*!< Snapshot filename*/
      );

      //! Implementation for ROVER_MOVE_ABORT command handler
      //! Aborts the current motion.
      void ROVER_MOVE_ABORT_cmdHandler(
          const FwOpcodeType opCode, /*!< The opcode*/
          const U32 cmdSeq /*!< The command sequence number*/
      );


      InternalAction m_current_action;
      FwOpcodeType m_op;
      U32 m_seq;
      U32 m_duration;
      U8 m_power;
      U8 m_count;
      bool m_new_command;
      bool m_headlight;
  };

} // end namespace Rover

#endif

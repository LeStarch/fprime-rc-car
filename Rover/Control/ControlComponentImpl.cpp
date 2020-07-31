// ======================================================================
// \title  ControlComponentImpl.cpp
// \author starchmd
// \brief  cpp file for Control component implementation class
//
// \copyright
// Copyright 2009-2015, by the California Institute of Technology.
// ALL RIGHTS RESERVED.  United States Government Sponsorship
// acknowledged.
//
// ======================================================================
#include <stdlib.h>
#include <Rover/Top/RateSchedContexts.hpp>
#include <Rover/Control/ControlComponentImpl.hpp>
#include "Fw/Types/BasicTypes.hpp"

#define CLOCK_STEP_MS 10
#define SECOND_MS 1000

namespace Rover {

  // ----------------------------------------------------------------------
  // Construction, initialization, and destruction
  // ----------------------------------------------------------------------

  ControlComponentImpl ::
#if FW_OBJECT_NAMES == 1
    ControlComponentImpl(
        const char *const compName
    ) :
      ControlComponentBase(compName),
#else
    ControlComponentImpl(void),
#endif
    m_current_action(MOVE_IDLE_INT),
    m_op(0),
    m_seq(0),
    m_duration(0),
    m_power(0),
    m_count(0),
    m_new_command(false),
    m_headlight(false)
  {}

  void ControlComponentImpl ::
    init(
        const NATIVE_INT_TYPE queueDepth,
        const NATIVE_INT_TYPE instance
    )
  {
    ControlComponentBase::init(queueDepth, instance);
  }

  ControlComponentImpl ::
    ~ControlComponentImpl(void)
  {}

  // ----------------------------------------------------------------------
  // Handler implementations for user-defined typed input ports
  // ----------------------------------------------------------------------

  void ControlComponentImpl ::
    Run_handler(
        const NATIVE_INT_TYPE portNum,
        NATIVE_UINT_TYPE context
    )
  {
      // Check the context parameter to perform the correct actions
      switch (context) {
          case Rover::CONTEXT_DEMO_1Hz:
              break;
          case Rover::CONTEXT_DEMO_10Hz:
              clock_100hz();
              break;
          default:
              FW_ASSERT(0,context);
              break; // for the code checkers
      }
  }

  void ControlComponentImpl :: clock_100hz() {
      // Forward, Backward, Left, Right, and Headlights
      // each of the outputs starts off
      bool outputs[this->getNum_GpioWrite_OutputPorts()];
      memset(outputs, 0, sizeof(outputs));

      // Handle the duration.
      if ((m_current_action == MOVE_FORCED_COOLOFF_INT || m_current_action == MOVE_IDLE_INT)
           && m_duration <= CLOCK_STEP_MS && m_duration != 0) {
          this->cmdResponse_out(m_op, m_seq, Fw::COMMAND_OK);
          m_current_action = MOVE_IDLE_INT;
          m_duration = 0;
          m_new_command = true;
      } else if (m_current_action != MOVE_IDLE_INT && m_duration <= CLOCK_STEP_MS && m_duration != 0) {
          m_current_action = MOVE_FORCED_COOLOFF_INT;
          m_duration = SECOND_MS;
          m_new_command = true;
      }

      // Set all outputs based on action
      switch (m_current_action) {
          // Powered breaking turns both on
          case MOVE_BREAK_INT:
              outputs[0] = true;
              outputs[1] = true;
              break;
          case MOVE_FORWARD_INT:
              outputs[0] = true;
              break;
          case MOVE_BACKWARD_INT:
              outputs[1] = true;
              break;
          case MOVE_FORWARD_LEFT_INT:
              outputs[0] = true;
              outputs[2] = true;
              break;
          case MOVE_FORWARD_RIGHT_INT:
              outputs[0] = true;
              outputs[3] = true;
              break;
          case MOVE_BACKWARD_LEFT_INT:
              outputs[1] = true;
              outputs[2] = true;
              break;
          case MOVE_BACKWARD_RIGHT_INT:
              outputs[1] = true;
              outputs[3] = true;
              break;
          case MOVE_FORCED_COOLOFF_INT:
          case MOVE_IDLE_INT:
              break;
          default:
              FW_ASSERT(false);
      }
      outputs[4] = m_headlight;

      // Override the output based on the power
      if (m_count >= m_power) {
          outputs[0] = false;
          outputs[1] = false;
      }
      m_count = (m_count + CLOCK_STEP_MS) % 100;

      // Write all outputs
      for (int i = 0; i < this->getNum_GpioWrite_OutputPorts(); i++) {
          GpioWrite_out(i, outputs[i]);
      }
      tlmWrite_HeadlightState(m_headlight ? LIGHTS_ON : LIGHTS_OFF);
      // Print new events to the console
      if (m_new_command) {
          MoveEvent event = static_cast<MoveEvent>(m_current_action);
          log_ACTIVITY_HI_RoverAction(event, m_power, m_duration);
          m_new_command = false;
      }
      m_duration = (m_duration >= CLOCK_STEP_MS) ? m_duration - CLOCK_STEP_MS : m_duration;
  }
  // ----------------------------------------------------------------------
  // Command handler implementations
  // ----------------------------------------------------------------------

  void ControlComponentImpl ::
    ROVER_MOVE_cmdHandler(
        const FwOpcodeType opCode,
        const U32 cmdSeq,
        MoveDirection action,
        U8 power,
        U32 duration
    )
  {
      m_new_command = true;
      m_current_action = static_cast<InternalAction>(action);
      m_duration = duration;
      m_power = power;
      m_count = 0;
      m_op = opCode;
      m_seq = cmdSeq;
  }

  void ControlComponentImpl ::
    ROVER_HEADLIGHT_cmdHandler(
        const FwOpcodeType opCode,
        const U32 cmdSeq,
        HeadlightsOnOff action
    )
  {
    m_headlight = (action == HEADLIGHTS_ON);
    this->cmdResponse_out(opCode,cmdSeq,Fw::COMMAND_OK);
  }

  void ControlComponentImpl ::
    ROVER_SNAPSHOT_cmdHandler(
        const FwOpcodeType opCode,
        const U32 cmdSeq,
        const Fw::CmdStringArg& filename
    )
  {
    char command[150];
    snprintf(command, sizeof(command), "raspistill -o %s --nopreview --exposure sports --timeout 1 &", filename.toChar());
    int ret = system(command);
    this->cmdResponse_out(opCode,cmdSeq,(ret == 0) ? Fw::COMMAND_OK : Fw::COMMAND_EXECUTION_ERROR);
  }

  void ControlComponentImpl ::
    ROVER_MOVE_ABORT_cmdHandler(
        const FwOpcodeType opCode,
        const U32 cmdSeq
    )
  {
    m_duration = 10;  // Will abort shortly
    this->cmdResponse_out(opCode,cmdSeq,Fw::COMMAND_OK);
  }

} // end namespace Rover

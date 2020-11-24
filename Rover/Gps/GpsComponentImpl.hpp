// ====================================================================== 
// \title  GpsImpl.hpp
// \author lemstarch
// \brief  hpp header file for the sample F' GPS component, based on a
//         NMEA GPS receiver.
//
// \copyright
// Copyright 2018, lestarch
// ====================================================================== 

#ifndef Gps_HPP
#define Gps_HPP
#include "Utils/Types/CircularBuffer.hpp"
#include "Rover/Gps/GpsComponentAc.hpp"

#define GP_HEADER_LENGTH 6
#define GPGGA_LENGTH 82
// Serial buffer size + maximum length needed storred to avoid losing data
#define GP_BUFF_SIZE (64 + GPGGA_LENGTH)

#define GPS_SERIAL_SIZE 1024

namespace Rover {

  class GpsComponentImpl :
    public GpsComponentBase
  {
      /**
       * GpsPacket:
       *   A structure containing the information in the GPS location pacaket
       * received via the NMEA GPS receiver.
       */
      struct GgaPacket {
          float utcTime;
          float dmNS;
          char northSouth;
          float dmEW;
          char eastWest;
          unsigned int lock;
          unsigned int count;
          float hdop;
          float altitude;
      };
    public:

      // ----------------------------------------------------------------------
      // Construction, initialization, and destruction
      // ----------------------------------------------------------------------

      //! Construct object Gps
      //!
      GpsComponentImpl(
          const char *const compName /*!< The component name*/
      );

      //! Initialize object Gps
      //!
      void init(
          const NATIVE_INT_TYPE instance = 0 /*!< The instance number*/
      );

      //! Destroy object Gps
      //!
      ~GpsComponentImpl(void);

    PRIVATE:
      //! Read a float field
      bool readFloat(float& output);
      //! Process the GGA buffer that was previously read
      void processGga(GgaPacket& packet);
      //! Detect a GGA packet in the ring buffer
      bool detectGga(GgaPacket& packet);
      //! Process the ring buffer
      void processRing();
      //! Process a buffer read in the schedIn call
      void processBuffer(Fw::Buffer& buffer);

      // ----------------------------------------------------------------------
      // Handler implementations for user-defined typed input ports
      // ----------------------------------------------------------------------

      //! Handler implementation for schedIn
      //!
      void schedIn_handler(
          const NATIVE_INT_TYPE portNum, /*!< The port number*/
          NATIVE_UINT_TYPE context /*!< The call order*/
      );

      //! Handler implementation for readCallback
      //!
      void readCallback_handler(
          const NATIVE_INT_TYPE portNum, /*!< The port number*/
          Fw::Buffer& fwBuffer, /*!< Buffer*/
          Drv::SerialReadStatus& status
      );

    PRIVATE:

      // ----------------------------------------------------------------------
      // Command handler implementations 
      // ----------------------------------------------------------------------

      //! Implementation for Gps_ReportLockStatus command handler
      //! A command to force an EVR reporting lock status.
      void Gps_ReportLockStatus_cmdHandler(
          const FwOpcodeType opCode, /*!< The opcode*/
          const U32 cmdSeq /*!< The command sequence number*/
      );
      //!< Data store. TODO: replace with BufferManager
      U8 m_in_buffer[GP_BUFF_SIZE];
      //!< Ring buffer to hold the incoming data
      Types::CircularBuffer m_in_ring;
      //!< Good message count
      U32 m_good;
      //!< Mangled message count
      U32 m_mangled;
      //!< Is the GPS UART setup
      bool m_setup;
      //!< Has the device acquired GPS lock?
      bool m_locked;
    };

} // end namespace GpsApp

#endif

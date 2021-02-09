#pragma once
#include "entities/Entities.h"

namespace database
{
   class ISerialPortRequester
   {
   public:
      virtual ~ISerialPortRequester() = default;

      //--------------------------------------------------------------
      /// \brief                    Get all serial ports
      /// \return                   The serial ports list
      //--------------------------------------------------------------
      virtual std::vector<boost::shared_ptr<entities::CSerialPort>> getSerialPorts() = 0;

      //--------------------------------------------------------------
      /// \brief                    Find serial port entities from serial port path
      /// \param [in] lastKnownSerialPortPath    Serial port path
      /// \return                   The corresponding database entities
      /// \throw shared::exception::CEmptyResult is ID not found
      //--------------------------------------------------------------
      virtual std::vector<boost::shared_ptr<entities::CSerialPort>> getSerialPortsFromLastKnownSerialPortPath(
         const std::string& lastKnownSerialPortPath) = 0;

      //--------------------------------------------------------------
      /// \brief                    Add a serial port
      /// \param [in] serialPort    New serial port
      //--------------------------------------------------------------
      virtual void addSerialPort(const entities::CSerialPort& serialPort) = 0;

      //--------------------------------------------------------------
      /// \brief                    Remove a serial port
      /// \param [in] id            Serial port ID to delete
      //--------------------------------------------------------------
      virtual void removeSerialPort(int id) = 0;

      //--------------------------------------------------------------
      /// \brief                    Update a serial port
      /// \param lastKnownSerialPortPath Key of the serial port to update
      /// \param [in] serialPort    New serial port data
      //--------------------------------------------------------------
      virtual void updateSerialPort(const std::string& lastKnownSerialPortPath,
                                    const entities::CSerialPort& serialPort) = 0;
   };
} //namespace database 

#pragma once

#include "database/ISerialPortRequester.h"
#include "database/IDatabaseRequester.h"

namespace database
{
   namespace common
   {
      namespace requesters
      {
         //--------------------------------------------------------------
         /// \Brief		   Serial port requester for SQLite database
         //--------------------------------------------------------------
         class CSerialPort : public ISerialPortRequester
         {
         public:
            explicit CSerialPort(boost::shared_ptr<IDatabaseRequester> databaseRequester);
            virtual ~CSerialPort() = default;

            // ISerialPort implementation
            std::vector<boost::shared_ptr<entities::CSerialPort>> getSerialPorts() override;
            std::vector<boost::shared_ptr<entities::CSerialPort>> getSerialPortsFromLastKnownSerialPortPath(
               const std::string& lastKnownSerialPortPath) override;
            void addSerialPort(const entities::CSerialPort& serialPort) override;
            void removeSerialPort(int id) override;
            void updateSerialPort(const std::string& lastKnownSerialPortPath,
                                  const entities::CSerialPort& serialPort) override;
            // [END] ISerialPort implementation

         private:
            //--------------------------------------------------------------
            /// \Brief		   Reference to IDatabaseRequester
            //--------------------------------------------------------------
            boost::shared_ptr<IDatabaseRequester> m_databaseRequester;
         };
      } //namespace requesters
   } //namespace common
} //namespace database 

#pragma once
#include "IManuallyDefinedPortLister.h"
#include "ISerialPortsManager.h"
#include "ISerialPortsLister.h"
#include "database/ISerialPortRequester.h"

namespace hardware
{
   namespace serial
   {
      class CSerialPortsManager : public ISerialPortsManager
      {
      public:
         /// @brief Construct the serial port manager
         /// @param serialPortLister The class providing way to list all machine serial ports
         /// @param serialPortDatabaseRequester Access to database (serial port table)
         /// @param reserverdPorts The list of system reserved ports (= not usable by Yadoms or its plugins)
         /// @param manuallyDefinedPortLister The class providing way to list all reserved ports (manually defined on plugin configuration)
         explicit CSerialPortsManager(boost::shared_ptr<ISerialPortsLister> serialPortLister,
                                      boost::shared_ptr<database::ISerialPortRequester> serialPortDatabaseRequester,
                                      //TODO serialPortDatabaseRequester utile ?
                                      std::vector<std::string> reserverdPorts,
                                      boost::shared_ptr<IManuallyDefinedPortLister> manuallyDefinedPortLister);
         virtual ~CSerialPortsManager() = default;

         // ISerialPortsManager Implementation
         std::vector<std::string> requestPort(int pluginInstanceId,
                                              boost::shared_ptr<shared::CDataContainer> filter,
                                              const std::string& manuallyDefinedPort) override;
         void reservePort(int pluginInstanceId, const std::string& port) override;
         void releasePort(int pluginInstanceId) override;
         // [END] ISerialPortsManager Implementation

      private:
         static bool isPortIn(const std::string& port,
                              const std::vector<boost::shared_ptr<ISerialPort>>& in);
         static bool isPortIn(const std::string& port,
                              const std::vector<std::string>& in);
         std::vector<std::string> portListAsString(const std::vector<boost::shared_ptr<ISerialPort>>& in) const;

         static database::entities::CSerialPort fromHardwareSerialPort(const ISerialPort& hardwareSerialPort); //TODO utile ?
         void updateDatabase(const std::vector<boost::shared_ptr<ISerialPort>>& hardwareSerialPorts) const; //TODO utile ?
         void updatePhysicalPortInDatabase(const boost::shared_ptr<ISerialPort>& hardwareSerialPort) const; //TODO utile ?
         void updateUsbAdapterSerialPortInDatabase(const boost::shared_ptr<ISerialPort>& hardwareSerialPort) const; //TODO utile ?

         // The list of affected ports by plugin (a plugin can hold several ports)
         std::map<int, std::vector<std::string>> m_pluginsPorts;

         boost::shared_ptr<ISerialPortsLister> m_serialPortLister;
         boost::shared_ptr<database::ISerialPortRequester> m_serialPortDatabaseRequester;
         const std::vector<std::string> m_reserverdPorts;
         boost::shared_ptr<IManuallyDefinedPortLister> m_manuallyDefinedPortLister;
      };
   } // namespace serial
} // namespace hardware

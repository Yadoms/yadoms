#pragma once
#include "ISerialPort.h"

namespace hardware
{
   namespace serial
   {
      class ISerialPortsManager
      {
      public:
         virtual ~ISerialPortsManager() = default;

         // TODO faire 2 fonctions distinctes ? Une pour filtre, l'autre pour auto-détection
         /// @brief Request port for a plugin
         /// @param pluginInstanceId The plugin source of the request
         /// @param filter The filter to auto-detect a port (my be null to use manually port definition or get all available ports)
         /// @param manuallyDefinedPort The plugin explicitly ask for this port (empty if port should be auto-detected, default)
         /// @return A list of usable serial ports for this plugin
         /// @note The search for a compatible available port is based on the plugin package.json
         virtual std::vector<std::string> requestPort(
            int pluginInstanceId,
            boost::shared_ptr<shared::CDataContainer> filter,
            const std::string& manuallyDefinedPort = std::string()) = 0;

         /// @brief After requesting a port, the plugin should test for device connection and confirm port reservation
         /// (so it can not be used for another plugin instance)
         /// @param pluginInstanceId The plugin instance who want to lock port
         /// @param port The port to lock
         virtual void reservePort(int pluginInstanceId,
                                  const std::string& port) = 0;

         /// @brief Release the port(s) (make it(them) available for other plugins)
         /// @param pluginInstanceId The plugin currently using the port
         /// @note Do nothing if no port associated to plugin
         virtual void releasePort(int pluginInstanceId) = 0;
      };
   } // namespace serial
} // namespace hardware

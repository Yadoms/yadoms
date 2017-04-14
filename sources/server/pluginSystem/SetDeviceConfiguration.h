#pragma once
#include <shared/plugin/yPluginApi/ISetDeviceConfiguration.h>

namespace pluginSystem
{
   //-----------------------------------------------------
   ///\brief The ISetDeviceConfiguration implementation
   //-----------------------------------------------------
   class CSetDeviceConfiguration : public shared::plugin::yPluginApi::ISetDeviceConfiguration
   {
   public:
      //-----------------------------------------------------
      ///\brief                     Constructor
      ///\param[in] device          The target device
      ///\param[in] configuration   The device configuration
      //-----------------------------------------------------
      CSetDeviceConfiguration(const std::string& device, const shared::CDataContainer& configuration);

      //-----------------------------------------------------
      ///\brief               Destructor
      //-----------------------------------------------------
      virtual ~CSetDeviceConfiguration();

      // ISetDeviceConfiguration implementation
      const std::string& name() const override;
      const std::string& type() const override;
      const shared::CDataContainer&  configuration() const override;
      // [END] ISetDeviceConfiguration implementation

   private:
      //-----------------------------------------------------
      ///\brief               Device
      //-----------------------------------------------------
      const std::string m_device;

      //-----------------------------------------------------
      ///\brief               Device type
      //-----------------------------------------------------
      const std::string m_deviceType;

      //-----------------------------------------------------
      ///\brief               Configuration
      //-----------------------------------------------------
      const shared::CDataContainer m_configuration;
   };
} // namespace pluginSystem	



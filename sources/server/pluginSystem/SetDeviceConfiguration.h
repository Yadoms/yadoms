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
      CSetDeviceConfiguration(const std::string& device, const shared::CDataContainerSharedPtr& configuration);

      //-----------------------------------------------------
      ///\brief               Destructor
      //-----------------------------------------------------
      virtual ~CSetDeviceConfiguration();

      // ISetDeviceConfiguration implementation
      const std::string& name() const override;
      const shared::CDataContainerSharedPtr&  configuration() const override;
      // [END] ISetDeviceConfiguration implementation

   private:
      //-----------------------------------------------------
      ///\brief               Device
      //-----------------------------------------------------
      const std::string m_device;

      //-----------------------------------------------------
      ///\brief               Configuration
      //-----------------------------------------------------
      const shared::CDataContainerSharedPtr m_configuration;
   };
} // namespace pluginSystem	



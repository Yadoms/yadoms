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
      CSetDeviceConfiguration(const std::string& device, const boost::shared_ptr<shared::CDataContainer>& configuration);

      //-----------------------------------------------------
      ///\brief               Destructor
      //-----------------------------------------------------
      virtual ~CSetDeviceConfiguration();

      // ISetDeviceConfiguration implementation
      const std::string& name() const override;
      const boost::shared_ptr<shared::CDataContainer>&  configuration() const override;
      // [END] ISetDeviceConfiguration implementation

   private:
      //-----------------------------------------------------
      ///\brief               Device
      //-----------------------------------------------------
      const std::string m_device;

      //-----------------------------------------------------
      ///\brief               Configuration
      //-----------------------------------------------------
      const boost::shared_ptr<shared::CDataContainer> m_configuration;
   };
} // namespace pluginSystem	



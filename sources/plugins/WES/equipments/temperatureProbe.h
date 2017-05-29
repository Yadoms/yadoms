#pragma once

#include <shared/plugin/yPluginApi/IYPluginApi.h>
#include "IEquipment.h"
#include "../IWESConfiguration.h"
#include "masterDeviceConfiguration.h"

// Shortcut to yPluginApi namespace
namespace yApi = shared::plugin::yPluginApi;

namespace equipments
{
   //-----------------------------------------------------
   ///\brief WES equipment
   //-----------------------------------------------------
   class CtemperatureProbe : public IEquipment
   {
   public:
      //-----------------------------------------------------
      ///\brief                          Constructor
      ///\param[in]   api               Yadoms API
      ///\param[in] device              The device name
      ///\param[in] deviceConfiguration The device configuration
      ///\param[in] pluginConfiguration The plugin configuration
      //-----------------------------------------------------
      CtemperatureProbe(boost::shared_ptr<yApi::IYPluginApi> api,
                        const std::string& device,
                        const shared::CDataContainer& deviceConfiguration,
                        const boost::shared_ptr<IWESConfiguration> pluginConfiguration);

      // IExtension implementation
      std::string getDeviceName() const override;
      std::string getDeviceType() const override;
      bool isMasterDevice() const override;
      void updateFromDevice( boost::shared_ptr<yApi::IYPluginApi> api,
                            const boost::shared_ptr<IWESConfiguration> pluginConfiguration,
                             bool forceHistorization = false) override;
      void updateConfiguration(boost::shared_ptr<yApi::IYPluginApi> api,
                               const shared::CDataContainer& newConfiguration) override;
      void sendCommand(boost::shared_ptr<yApi::IYPluginApi> api,
                       const std::string& keyword,
                       boost::shared_ptr<const yApi::IDeviceCommand> command) override;
      void remove(boost::shared_ptr<yApi::IYPluginApi> api) override;
      specificHistorizers::EWESdeviceStatus getStatus() const override;
      // [END] IExtension implementation

      //-----------------------------------------------------
      ///\brief                     Destructor
      //-----------------------------------------------------
      virtual ~CtemperatureProbe();

   private:

      //-----------------------------------------------------
      ///\brief                     The device name
      //-----------------------------------------------------
      std::string m_deviceName;

      //-----------------------------------------------------
      ///\brief                     The device type
      //-----------------------------------------------------
      std::string m_deviceType;

      //--------------------------------------------------------------
      /// \brief	status of the device
      //--------------------------------------------------------------
      boost::shared_ptr<specificHistorizers::CdeviceStatus> m_deviceStatus;

      //--------------------------------------------------------------
      /// \brief	                  The device configuration
      //--------------------------------------------------------------
      CmasterDeviceConfiguration m_configuration;

      //--------------------------------------------------------------
      /// \brief	                  The keyword of this device
      //--------------------------------------------------------------
      boost::shared_ptr<yApi::historization::CTemperature> m_temperature;
   };
} // namespace equipments
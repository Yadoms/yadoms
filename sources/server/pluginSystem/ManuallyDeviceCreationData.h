#pragma once
#include <shared/plugin/yPluginApi/IManuallyDeviceCreationData.h>
#include <shared/DataContainer.h>


namespace pluginSystem
{
   //-----------------------------------------------------
   ///\brief The IDeviceCommand implementation
   //-----------------------------------------------------
   class CManuallyDeviceCreationData : public shared::plugin::yPluginApi::IManuallyDeviceCreationData
   {
   public:
      //-----------------------------------------------------
      ///\brief                        Constructor
      ///\param[in] deviceName         The device name to create
      ///\param[in] deviceName         The device model to create
      ///\param[in] configuration      The creation configuration of the device 
      //-----------------------------------------------------
      CManuallyDeviceCreationData(const std::string& deviceName, const std::string& deviceModel, const shared::CDataContainer & configuration);

      //-----------------------------------------------------
      ///\brief               Destructor
      //-----------------------------------------------------
      virtual ~CManuallyDeviceCreationData();

      // IManuallyDeviceCreationData implementation
      const std::string& getDeviceName() const override;
      const std::string& getDeviceModel() const override;
      const shared::CDataContainer & getConfiguration() const override;
      // [END] IManuallyDeviceCreationData implementation

   private:
      //-----------------------------------------------------
      ///\brief               Device
      //-----------------------------------------------------
      std::string m_deviceName;

      //-----------------------------------------------------
      ///\brief               Device model
      //-----------------------------------------------------
      std::string m_deviceModel;

      //-----------------------------------------------------
      ///\brief               Device creation parameters
      //-----------------------------------------------------
      shared::CDataContainer m_configuration;
   };  
} // namespace pluginSystem	
	
	
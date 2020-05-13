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
      ///\param[in] deviceType         The device type to create
      ///\param[in] configuration      The creation configuration of the device 
      //-----------------------------------------------------
      CManuallyDeviceCreationData(const std::string& deviceName, const std::string& deviceType, const boost::shared_ptr<shared::CDataContainer>& configuration);

      //-----------------------------------------------------
      ///\brief               Destructor
      //-----------------------------------------------------
      virtual ~CManuallyDeviceCreationData();

      // IManuallyDeviceCreationData implementation
      const std::string& getDeviceName() const override;
      const std::string& getDeviceType() const override;
      const boost::shared_ptr<shared::CDataContainer>& getConfiguration() const override;
      // [END] IManuallyDeviceCreationData implementation

   private:
      //-----------------------------------------------------
      ///\brief               Device
      //-----------------------------------------------------
      const std::string m_deviceName;

      //-----------------------------------------------------
      ///\brief               Device type (keep in members, to allow returning reference)
      //-----------------------------------------------------
      const std::string m_deviceType;

      //-----------------------------------------------------
      ///\brief               Device creation parameters
      //-----------------------------------------------------
      const boost::shared_ptr<shared::CDataContainer> m_configuration;
   };  
} // namespace pluginSystem	
	
	
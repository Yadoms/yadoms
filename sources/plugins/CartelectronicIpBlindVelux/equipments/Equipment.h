#pragma once

//#include <shared/plugin/yPluginApi/IYPluginApi.h>
#include "IEquipment.h"
#include "deviceConfiguration.h"
#include "specificHistorizers/VeluxCurtain.h"
#include "../http/httpContext.h"

// Shortcut to yPluginApi namespace
namespace yApi = shared::plugin::yPluginApi;

namespace equipments
{
   //-----------------------------------------------------
   ///\brief WES equipment
   //-----------------------------------------------------
   class CEquipment : public IEquipment
   {
   public:

      //-----------------------------------------------------
      ///\brief                          Constructor from manual creation
      ///\param[in]   api               Yadoms API
      ///\param[in] device              The device name
      ///\param[in] deviceConfiguration The device configuration
      ///\param[in] pluginConfiguration The plugin configuration
      //-----------------------------------------------------
	   CEquipment(boost::shared_ptr<yApi::IYPluginApi> api,
                  const std::string& device,
                  const shared::CDataContainer& deviceConfiguration);

      // IEquipment implementation
      std::string getDeviceName() const override;
      std::string getDeviceType() const override;
      void updateConfiguration(boost::shared_ptr<yApi::IYPluginApi> api,
                               const shared::CDataContainer& newConfiguration,
                               const int refreshEvent) override;
      void sendCommand(boost::shared_ptr<yApi::IYPluginApi> api,
                       const std::string& keyword,
                       boost::shared_ptr<const yApi::IDeviceCommand> command) override;
      specificHistorizers::EdeviceStatus getStatus() const override;
      // [END] IEquipment implementation

      //-----------------------------------------------------
      ///\brief                     Destructor
      //-----------------------------------------------------
      virtual ~CEquipment();

   private:

      //-----------------------------------------------------
      ///\brief                          set the device state
      ///\param[in] newState            the new state
      //-----------------------------------------------------
      void setDeviceState(std::vector<boost::shared_ptr<const yApi::historization::IHistorizable> >& keywordsToHistorize, 
                          specificHistorizers::EdeviceStatus newState) const;

      //-----------------------------------------------------
      ///\brief                     The device name
      //-----------------------------------------------------
      std::string m_deviceName;

      //-----------------------------------------------------
      ///\brief                     The device type
      //-----------------------------------------------------
      std::string m_deviceType;

      //--------------------------------------------------------------
      /// \brief	configuration of the device
      //--------------------------------------------------------------
      CdeviceConfiguration m_configuration;

	  //--------------------------------------------------------------
	  /// \brief	vector of the 2 relays
	  //--------------------------------------------------------------
	  std::vector<boost::shared_ptr<specificHistorizers::CVeluxCurtain>> m_shutters;

      //--------------------------------------------------------------
      /// \brief	status of the device
      //--------------------------------------------------------------
      boost::shared_ptr<specificHistorizers::CdeviceStatus> m_deviceStatus;

	  //--------------------------------------------------------------
	  /// \brief	Context used by the equipment (Basic Auth, ...)
	  //--------------------------------------------------------------
	  http::httpContext m_httpContext;

	  //--------------------------------------------------------------
	  /// \brief	Definition of the number of shutters
	  //--------------------------------------------------------------
	  static const int shuttersQty;
   };
} // namespace equipments
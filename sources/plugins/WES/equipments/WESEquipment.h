#pragma once

#include <shared/plugin/yPluginApi/IYPluginApi.h>
#include "IEquipment.h"
#include "../IWESConfiguration.h"
#include "masterDeviceConfiguration.h"
#include "TIC.h"
#include "WESSubEquipments/Analog.h"
#include "WESSubEquipments/Pulse.h"
#include "WESSubEquipments/Clamp.h"

// Shortcut to yPluginApi namespace
namespace yApi = shared::plugin::yPluginApi;

namespace equipments
{
   //-----------------------------------------------------
   ///\brief WES equipment
   //-----------------------------------------------------
   class CWESEquipment : public IEquipment
   {
   public:

      //-----------------------------------------------------
      ///\brief                          Constructor from manual creation
      ///\param[in]   api               Yadoms API
      ///\param[in] device              The device name
      ///\param[in] deviceConfiguration The device configuration
      ///\param[in] pluginConfiguration The plugin configuration
      //-----------------------------------------------------
      CWESEquipment(boost::shared_ptr<yApi::IYPluginApi> api,
                    const std::string& device,
                    const shared::CDataContainer& deviceConfiguration,
                    const boost::shared_ptr<IWESConfiguration> pluginConfiguration);

      // IEquipment implementation
      std::string getDeviceName() const override;
      std::string getDeviceType() const override;
      void updateFromDevice(boost::shared_ptr<yApi::IYPluginApi> api,
                            const boost::shared_ptr<IWESConfiguration> pluginConfiguration,
                            bool forceHistorization = false) override;
      void updateConfiguration(boost::shared_ptr<yApi::IYPluginApi> api,
                               const shared::CDataContainer& newConfiguration,
                               const int refreshEvent) override;
      void sendCommand(boost::shared_ptr<yApi::IYPluginApi> api,
                       const std::string& keyword,
                       boost::shared_ptr<const yApi::IDeviceCommand> command) override;
      void remove(boost::shared_ptr<yApi::IYPluginApi> api) override;
      specificHistorizers::EWESdeviceStatus getStatus() const override;
      // [END] IEquipment implementation

      //-----------------------------------------------------
      ///\brief                     Destructor
      //-----------------------------------------------------
      virtual ~CWESEquipment();

   private:

      //-----------------------------------------------------
      ///\brief                          Constructor
      ///\param[in] keywordsToHistorize list of keywords to historize
      ///\param[in] keyword             The keyword to update, if any
      ///\param[in] newValue            The new value
      ///\param[in] forceHistorization  force historization if necessary
      //-----------------------------------------------------
      static void updateSwitchValue(std::vector<boost::shared_ptr<const yApi::historization::IHistorizable> >& keywordsToHistorize, 
                             boost::shared_ptr<yApi::historization::CSwitch> keyword, 
                             bool newValue, 
                             bool forceHistorization);

      //-----------------------------------------------------
      ///\brief                          set the device state
      ///\param[in] newState            the new state
      //-----------------------------------------------------
      void setDeviceState(std::vector<boost::shared_ptr<const yApi::historization::IHistorizable> >& keywordsToHistorize, 
                          specificHistorizers::EWESdeviceStatus newState) const;

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
      CmasterDeviceConfiguration m_configuration;

      //--------------------------------------------------------------
      /// \brief	status of the device
      //--------------------------------------------------------------
      boost::shared_ptr<specificHistorizers::CdeviceStatus> m_deviceStatus;

      //--------------------------------------------------------------
      /// \brief	vector of all relays
      //--------------------------------------------------------------
      std::vector<boost::shared_ptr<yApi::historization::CSwitch> > m_relaysList;

      //--------------------------------------------------------------
      /// \brief	vector of inputs
      //--------------------------------------------------------------
      std::vector<boost::shared_ptr<yApi::historization::CSwitch> > m_inputList;

      //--------------------------------------------------------------
      /// \brief  TIC counters
      //--------------------------------------------------------------
      //TODO : To be included to the WES ???
      std::vector<boost::shared_ptr<equipments::CTIC> > m_TICList;

      //--------------------------------------------------------------
      /// \brief	Pulse counters
      //--------------------------------------------------------------
      std::vector<boost::shared_ptr<equipments::subdevices::CPulse> > m_PulseList;

      //--------------------------------------------------------------
      /// \brief	Clamps
      //--------------------------------------------------------------
      std::vector<boost::shared_ptr<equipments::subdevices::CClamp> > m_ClampList;

      //--------------------------------------------------------------
      /// \brief	Analog Values
      //--------------------------------------------------------------
      std::vector<boost::shared_ptr<equipments::subdevices::CAnalog> > m_AnalogList;

      //--------------------------------------------------------------
      /// \brief	WES IP Mapping. Differentiation v1/v2
      //--------------------------------------------------------------

      struct WESIOMapping{
         unsigned char relayQty;
         unsigned char ticQty;
         unsigned char inputQty;
         unsigned char pulseQty;
         unsigned char clampQty;
         unsigned char anaQty;
      };

      static const WESIOMapping WESv1;
      static const WESIOMapping WESv2;

      WESIOMapping m_WESIOMapping;
      int m_version;
   };
} // namespace equipments
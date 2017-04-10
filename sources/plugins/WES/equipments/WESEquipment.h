#pragma once

#include <shared/plugin/yPluginApi/IYPluginApi.h>
#include "IEquipment.h"
#include "masterDeviceConfiguration.h"

// Shortcut to yPluginApi namespace
namespace yApi = shared::plugin::yPluginApi;

#define WES_RELAY_QTY 4
#define WES_TIC_QTY   2
#define WES_PULSE_QTY 4

namespace equipments
{
   //-----------------------------------------------------
   ///\brief WES equipment
   //-----------------------------------------------------
   class CWESEquipment : public IEquipment
   {
   public:
      //-----------------------------------------------------
      ///\brief                     Constructor
      ///\param[in]   api          Yadoms API
      ///\param[in] device         The device name
      //-----------------------------------------------------
      CWESEquipment(boost::shared_ptr<yApi::IYPluginApi> api,
                    const std::string& device,
                    const shared::CDataContainer& deviceConfiguration);

      // IExtension implementation
      std::string getDeviceName() const override;
      std::string getDeviceType() const override;
      bool isMasterDevice() const override;
      void updateFromDevice( boost::shared_ptr<yApi::IYPluginApi> api,
                             bool forceHistorization = false) override;
      shared::CDataContainer buildMessageToDevice(boost::shared_ptr<yApi::IYPluginApi> api, shared::CDataContainer& parameters, boost::shared_ptr<const yApi::IDeviceCommand> command) override;
      // [END] IExtension implementation

      //-----------------------------------------------------
      ///\brief                     Destructor
      //-----------------------------------------------------
      virtual ~CWESEquipment();

   private:

      //-----------------------------------------------------
      ///\brief                     The device name
      //-----------------------------------------------------
      std::string m_deviceName;

      //-----------------------------------------------------
      ///\brief                     The device type
      //-----------------------------------------------------
      std::string m_deviceType;

      CmasterDeviceConfiguration m_configuration;

      //--------------------------------------------------------------
      /// \brief	vector of all relays
      //--------------------------------------------------------------
      std::vector<boost::shared_ptr<yApi::historization::CSwitch> > m_relaysList;

      //--------------------------------------------------------------
      /// \brief	vector of all Digital input
      //--------------------------------------------------------------
      //std::vector<boost::shared_ptr<yApi::historization::CSwitch> > m_DIList;

      //--------------------------------------------------------------
      /// \brief	vector of all Analog input
      //--------------------------------------------------------------
      //std::vector<boost::shared_ptr<specificHistorizers::CAnalog> > m_analogList;

      //--------------------------------------------------------------
      /// \brief  counter TIC 1
      //--------------------------------------------------------------
      std::vector<boost::shared_ptr<yApi::historization::CEnergy> > m_counterTICList;

      //--------------------------------------------------------------
      /// \brief	The keyword Counter 1
      //--------------------------------------------------------------
      std::vector<boost::shared_ptr<yApi::historization::CCounter> > m_PulseCounterList;
   };
} // namespace equipments
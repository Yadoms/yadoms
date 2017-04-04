#pragma once

#include <shared/plugin/yPluginApi/IYPluginApi.h>
#include "IEquipment.h"

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
                    const std::string& device);

      // IExtension implementation
      std::string getDeviceName() const override;
      std::string getDeviceType() const override;
      void updateFromDevice(const std::string& type, 
                            boost::shared_ptr<yApi::IYPluginApi> api, 
                            shared::CDataContainer& values,
                            bool forceHistorization = false) override;
      void historizePendingCommand(boost::shared_ptr<yApi::IYPluginApi> api, boost::shared_ptr<const yApi::IDeviceCommand> command) override;
      void resetPendingCommand() override;
      shared::CDataContainer buildMessageToDevice(boost::shared_ptr<yApi::IYPluginApi> api, shared::CDataContainer& parameters, boost::shared_ptr<const yApi::IDeviceCommand> command) override;
      // [END] IExtension implementation

      //-----------------------------------------------------
      ///\brief                     Destructor
      //-----------------------------------------------------
      virtual ~CWESEquipment();

   private:

      //-----------------------------------------------------
      ///\brief                     Update IOs from different types
      ///\param[in]   api                 Yadoms API
      ///\param[in] values                values received from the WES
      ///\param[in] keywordsList          the list of keywords to be updated
      ///\param[in] ToHistorize           keywords to historize at the end of the function
      ///\param[in] forceHistorization    force the historization of all the keyword list, if necessary (initialization, ...)
      //-----------------------------------------------------
      template<class T1, class T2>
      void updateIOFromDevice(boost::shared_ptr<yApi::IYPluginApi> api,
                              shared::CDataContainer& values,
                              std::vector<boost::shared_ptr<T1> >& keywordsList,
                              std::vector<boost::shared_ptr<const yApi::historization::IHistorizable> >& ToHistorize,
                              bool forceHistorization = false);

      //-----------------------------------------------------
      ///\brief                     The device name
      //-----------------------------------------------------
      std::string m_deviceName;

      //-----------------------------------------------------
      ///\brief                     The device type
      //-----------------------------------------------------
      std::string m_deviceType;

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
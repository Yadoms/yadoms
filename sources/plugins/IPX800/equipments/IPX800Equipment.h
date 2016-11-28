#pragma once

#include <shared/plugin/yPluginApi/IYPluginApi.h>
#include "IEquipment.h"
#include "../specificHistorizers/inputOutput.h"
#include "../specificHistorizers/Analog.h"
#include "../specificHistorizers/counter.h"

// Shortcut to yPluginApi namespace
namespace yApi = shared::plugin::yPluginApi;

#define IPX800_RELAY_QTY 8
#define IPX800_DI_QTY    8
#define IPX800_ANA_QTY   4
#define IPX800_CNT_QTY  16

namespace equipments
{
   //-----------------------------------------------------
   ///\brief IPX800 equipment
   //-----------------------------------------------------
   class CIPX800Equipment : public IEquipment
   {
   public:
      //-----------------------------------------------------
      ///\brief                     Constructor
      ///\param[in] keywordName     Yadoms keyword name
      //-----------------------------------------------------
      CIPX800Equipment(boost::shared_ptr<yApi::IYPluginApi> api,
                       const std::string& device);

      // IExtension implementation
      std::string getDeviceName() const override;
      std::string getDeviceType() const override;
      int getSlot() const override;
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
      virtual ~CIPX800Equipment();

   private:

      //-----------------------------------------------------
      ///\brief                     Update IOs from different types
      //-----------------------------------------------------
      template<class T1, class T2>
      void updateIOFromDevice(boost::shared_ptr<yApi::IYPluginApi> api,
                              shared::CDataContainer& values,
                              std::vector<boost::shared_ptr<T1> >& keywordsList,
                              std::vector<boost::shared_ptr<const yApi::historization::IHistorizable> >& ToHistorize,
                              bool forceHistorization = false);

      void setParameter(const std::string& keywordName,
                        std::vector<boost::shared_ptr<specificHistorizers::CInputOuput> >& keywordsList,
                        boost::shared_ptr<const yApi::IDeviceCommand> command,
                        const std::string& pinNumber,
                        shared::CDataContainer& parameters);

       void setParameter(const std::string& keywordName,
                         std::vector<boost::shared_ptr<specificHistorizers::CCounter> >& keywordsList,
                         boost::shared_ptr<const yApi::IDeviceCommand> command,
                         const std::string& value,
                         shared::CDataContainer& parameters);

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
      std::vector<boost::shared_ptr<specificHistorizers::CInputOuput> > m_relaysList;

      //--------------------------------------------------------------
      /// \brief	vector of all Digital input
      //--------------------------------------------------------------
      std::vector<boost::shared_ptr<specificHistorizers::CInputOuput> > m_DIList;

      //--------------------------------------------------------------
      /// \brief	vector of all Analog input
      //--------------------------------------------------------------
      std::vector<boost::shared_ptr<specificHistorizers::CAnalog> > m_analogList;

      //--------------------------------------------------------------
      /// \brief	vector of all counters
      //--------------------------------------------------------------
      std::vector<boost::shared_ptr<specificHistorizers::CCounter> > m_countersList;

      //-----------------------------------------------------
      ///\brief                     Waiting command
      //-----------------------------------------------------
      boost::shared_ptr<specificHistorizers::CInputOuput> m_pendingIOHistorizer;
      boost::shared_ptr<specificHistorizers::CCounter> m_pendingCounterHistorizer;
   };
} // namespace equipments
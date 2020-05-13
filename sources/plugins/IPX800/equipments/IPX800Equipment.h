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
      ///\param[in]   api          Yadoms API
      ///\param[in] device         The device name
      //-----------------------------------------------------
      CIPX800Equipment(boost::shared_ptr<yApi::IYPluginApi> api,
                       const std::string& device);

      // IExtension implementation
      std::string getDeviceName() const override;
      std::string getDeviceType() const override;
      int getSlot() const override;
      void updateFromDevice(const std::string& type, 
                            boost::shared_ptr<yApi::IYPluginApi> api, 
                            boost::shared_ptr<shared::CDataContainer>& values,
                            bool forceHistorization = false) override;
      void historizePendingCommand(boost::shared_ptr<yApi::IYPluginApi> api, boost::shared_ptr<const yApi::IDeviceCommand> command) override;
      void resetPendingCommand() override;
      boost::shared_ptr<shared::CDataContainer> buildMessageToDevice(boost::shared_ptr<yApi::IYPluginApi> api, boost::shared_ptr<shared::CDataContainer>& parameters, boost::shared_ptr<const yApi::IDeviceCommand> command) override;
      void setNewConfiguration(const boost::shared_ptr<shared::CDataContainer>& newConfiguration) override;
      // [END] IExtension implementation

      //-----------------------------------------------------
      ///\brief                     Destructor
      //-----------------------------------------------------
      virtual ~CIPX800Equipment();

   private:

      //-----------------------------------------------------
      ///\brief                     Update IOs from different types
      ///\param[in]   api                 Yadoms API
      ///\param[in] values                values received from the IPX800
      ///\param[in] keywordsList          the list of keywords to be updated
      ///\param[in] ToHistorize           keywords to historize at the end of the function
      ///\param[in] forceHistorization    force the historization of all the keyword list, if necessary (initialization, ...)
      //-----------------------------------------------------
      template<class T1, class T2>
      void updateIOFromDevice(boost::shared_ptr<yApi::IYPluginApi> api,
                              boost::shared_ptr<shared::CDataContainer>& values,
                              std::vector<boost::shared_ptr<T1> >& keywordsList,
                              std::vector<boost::shared_ptr<const yApi::historization::IHistorizable> >& ToHistorize,
                              bool forceHistorization = false);

      //-----------------------------------------------------
      ///\brief                     Create the parameter to send to the device for a CInputOuput keyword (relay type)
      ///\param[in] keywordName           Name of the keyword to generate (from the Yadoms web client)
      ///\param[in] keywordsList          list of keywords where to find the keyword
      ///\param[in] command               command received from Yadoms client web
      ///\param[in] pinNumber             index Number of the IO into the tab of the IPX800
      ///\param[in] parameters            parameter send to the IPX800
      //-----------------------------------------------------
      void setParameter(const std::string& keywordName,
                        std::vector<boost::shared_ptr<specificHistorizers::CInputOuput> >& keywordsList,
                        boost::shared_ptr<const yApi::IDeviceCommand> command,
                        const std::string& pinNumber,
                        boost::shared_ptr<shared::CDataContainer>& parameters);

      //-----------------------------------------------------
      ///\brief                     Create the parameter to send to the device for a CCounter keyword (couter type)
      ///\param[in] keywordName           Name of the keyword to generate (from the Yadoms web client)
      ///\param[in] keywordsList          list of keywords where to find the keyword
      ///\param[in] command               command received from Yadoms client web
      ///\param[in] pinNumber             index Number of the IO into the tab of the IPX800
      ///\param[in] parameters            parameter send to the IPX800
      //-----------------------------------------------------

       void setParameter(const std::string& keywordName,
                         std::vector<boost::shared_ptr<specificHistorizers::CCounter> >& keywordsList,
                         boost::shared_ptr<const yApi::IDeviceCommand> command,
                         const std::string& value,
                         boost::shared_ptr<shared::CDataContainer>& parameters);

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
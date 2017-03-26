#pragma once
#include "IRfxcomMessage.h"
#include "RFXtrxHelpers.h"
#include <shared/plugin/yPluginApi/IYPluginApi.h>
#include "ISecurity2Subtype.h"

namespace yApi = shared::plugin::yPluginApi;

namespace rfxcomMessages
{
   //--------------------------------------------------------------
   /// \brief	The Security2 protocol support
   //--------------------------------------------------------------
   class CSecurity2 : public IRfxcomMessage
   {
   public:
      //--------------------------------------------------------------
      /// \brief	                        Constructor
      /// \param[in] api                  Yadoms APi context
      /// \param[in] keyword              Keyword concerned by the command
      /// \param[in] command              The command
      /// \param[in] deviceDetails        The device parameters
      /// \throw                          shared::exception::CInvalidParameter if fail to interpret command
      /// \note                           Use this constructor for command (to build RFXCom message)
      //--------------------------------------------------------------
      CSecurity2(boost::shared_ptr<yApi::IYPluginApi> api,
                 const std::string& keyword,
                 const std::string& command,
                 const shared::CDataContainer& deviceDetails);

      //--------------------------------------------------------------
      /// \brief	                        Constructor
      /// \param[in] api                  Yadoms APi context
      /// \param[in] subType              Device subType
      /// \param[in] manuallyDeviceCreationConfiguration The device concfiguration
      /// \throw                          shared::exception::CInvalidParameter or shared::exception::COutOfRange if fail to interpret configuration
      /// \note                           Use this constructor for manually device creation
      //--------------------------------------------------------------
      CSecurity2(boost::shared_ptr<yApi::IYPluginApi> api,
                 unsigned int subType,
                 const shared::CDataContainer& manuallyDeviceCreationConfiguration);

      //--------------------------------------------------------------
      /// \brief	                        Constructor
      /// \param[in] api                  Yadoms APi context
      /// \param[in] rbuf                 The received buffer
      /// \param[in] rbufSize             Message size, received from Rfxcom
      /// \note                           Use this constructor for received messages (to historize received data to Yadoms)
      /// \throw                          shared::exception::CInvalidParameter
      //--------------------------------------------------------------
      CSecurity2(boost::shared_ptr<yApi::IYPluginApi> api,
                 const RBUF& rbuf,
                 size_t rbufSize);

      //--------------------------------------------------------------
      /// \brief	Destructor
      //--------------------------------------------------------------
      virtual ~CSecurity2();

      // IRfxcomMessage implementation
      boost::shared_ptr<std::queue<shared::communication::CByteBuffer> > encode(boost::shared_ptr<ISequenceNumberProvider> seqNumberProvider) const override;
      void historizeData(boost::shared_ptr<yApi::IYPluginApi> api) const override;
      const std::string& getDeviceName() const override;
      // [END] IRfxcomMessage implementation

   protected:
      //--------------------------------------------------------------
      /// \brief	Set and create the subtype
      /// \param[in] subType              Device subType
      //--------------------------------------------------------------
      void createSubType(unsigned char subType);

      //--------------------------------------------------------------
      /// \brief	                        Build the device name
      //--------------------------------------------------------------
      void buildDeviceName();

      //--------------------------------------------------------------
      /// \brief	Declare the device
      /// \param[in] api                  Yadoms APi context
      //--------------------------------------------------------------
      void declare(boost::shared_ptr<yApi::IYPluginApi> api);

   private:
      //--------------------------------------------------------------
      /// \brief	The device sub-type
      //--------------------------------------------------------------
      unsigned char m_subType;

      //--------------------------------------------------------------
      /// \brief	The device name
      //--------------------------------------------------------------
      std::string m_deviceName;

      //--------------------------------------------------------------
      /// \brief	The sub-type management
      //--------------------------------------------------------------
      boost::shared_ptr<ISecurity2Subtype> m_subTypeManager;

      //--------------------------------------------------------------
      /// \brief	The battery level (percent)
      //--------------------------------------------------------------
      boost::shared_ptr<yApi::historization::CBatteryLevel> m_batteryLevel;

      //--------------------------------------------------------------
      /// \brief	The signalPower (percent)
      //--------------------------------------------------------------
      boost::shared_ptr<yApi::historization::CSignalPower> m_signalPower;

      //--------------------------------------------------------------
      /// \brief	The keywords list to historize in one step for better performances
      //--------------------------------------------------------------
      std::vector<boost::shared_ptr<const yApi::historization::IHistorizable> > m_keywords;
   };
} // namespace rfxcomMessages



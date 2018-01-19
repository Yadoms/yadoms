#pragma once
#include "IRfxcomMessage.h"
#include "RFXtrxHelpers.h"
#include <shared/plugin/yPluginApi/IYPluginApi.h>
#include <shared/DataContainer.h>
#include "ILighting5Subtype.h"
#include "../IUnsecuredProtocolFilter.h"

namespace yApi = shared::plugin::yPluginApi;

namespace rfxcomMessages
{
   //--------------------------------------------------------------
   /// \brief	The Lightning5 protocol support
   //--------------------------------------------------------------
   class CLighting5 : public IRfxcomMessage
   {
   public:
      //--------------------------------------------------------------
      /// \brief	                        Constructor
      /// \param[in] api                  Yadoms APi context
      /// \param[in] command              The command
      /// \param[in] deviceDetails        The device parameters
      /// \throw                          shared::exception::CInvalidParameter if fail to interpret command
      /// \note                           Use this constructor for command (to build RFXCom message)
      //--------------------------------------------------------------
      CLighting5(boost::shared_ptr<yApi::IYPluginApi> api,
                 const std::string& command,
                 const shared::CDataContainer& deviceDetails);

      //--------------------------------------------------------------
      /// \brief	                        Constructor
      /// \param[in] api                  Yadoms APi context
      /// \param[in] subType              Device subType
      /// \param[in] name                 Device name
      /// \param[in] manuallyDeviceCreationConfiguration The device concfiguration
      /// \throw                          shared::exception::CInvalidParameter or shared::exception::COutOfRange if fail to interpret configuration
      /// \note                           Use this constructor for manually device creation
      //--------------------------------------------------------------
      CLighting5(boost::shared_ptr<yApi::IYPluginApi> api,
                 unsigned int subType,
                 const std::string& name,
                 const shared::CDataContainer& manuallyDeviceCreationConfiguration);

      //--------------------------------------------------------------
      /// \brief	                        Constructor
      /// \param[in] api                  Yadoms APi context
      /// \param[in] rbuf                 The received buffer
      /// \param[in] rbufSize             Message size, received from Rfxcom
      /// \param[in] messageFilter        The filter for unsecured protocols
      /// \note                           Use this constructor for received messages (to historize received data to Yadoms)
      /// \throw                          shared::exception::CInvalidParameter
      //--------------------------------------------------------------
      CLighting5(boost::shared_ptr<yApi::IYPluginApi> api,
                 const RBUF& rbuf,
                 size_t rbufSize,
                 boost::shared_ptr<IUnsecuredProtocolFilter> messageFilter);

      //--------------------------------------------------------------
      /// \brief	Destructor
      //--------------------------------------------------------------
      virtual ~CLighting5();

      //--------------------------------------------------------------
      /// \brief	Filter
      //--------------------------------------------------------------
      static boost::shared_ptr<IUnsecuredProtocolFilter> createFilter();

      // IRfxcomMessage implementation
      boost::shared_ptr<std::queue<shared::communication::CByteBuffer>> encode(boost::shared_ptr<ISequenceNumber> seqNumberProvider) const override;
      void historizeData(boost::shared_ptr<yApi::IYPluginApi> api) const override;
      const std::string& getDeviceName() const override;
      const std::vector<boost::shared_ptr<const yApi::historization::IHistorizable>>& keywords() override;
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

      void buildDeviceDetails();

   private:
      //--------------------------------------------------------------
      /// \brief	The device sub-type
      //--------------------------------------------------------------
      unsigned char m_subType;

      //--------------------------------------------------------------
      /// \brief	The device id
      //--------------------------------------------------------------
      unsigned int m_id;

      //--------------------------------------------------------------
      /// \brief	The filter for unsecured protocols
      //--------------------------------------------------------------
      boost::shared_ptr<IUnsecuredProtocolFilter> m_messageFilter;

      //--------------------------------------------------------------
      /// \brief	The device unit code
      //--------------------------------------------------------------
      unsigned char m_unitCode;

      //--------------------------------------------------------------
      /// \brief	The device name
      //--------------------------------------------------------------
      std::string m_deviceName;

      shared::CDataContainer m_deviceDetails;

      //--------------------------------------------------------------
      /// \brief	The sub-type management
      //--------------------------------------------------------------
      boost::shared_ptr<ILighting5Subtype> m_subTypeManager;

      //--------------------------------------------------------------
      /// \brief	The signalPower (percent)
      //--------------------------------------------------------------
      boost::shared_ptr<yApi::historization::CSignalPower> m_signalPower;

      //--------------------------------------------------------------
      /// \brief	The keywords list to historize in one step for better performances
      //--------------------------------------------------------------
      std::vector<boost::shared_ptr<const yApi::historization::IHistorizable>> m_keywords;
   };
} // namespace rfxcomMessages



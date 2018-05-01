#pragma once
#include "IRfxcomMessage.h"
#include "RFXtrxHelpers.h"
#include <shared/plugin/yPluginApi/IYPluginApi.h>

namespace yApi = shared::plugin::yPluginApi;

namespace rfxcomMessages
{
   //--------------------------------------------------------------
   /// \brief	The Curtain1 protocol support
   //--------------------------------------------------------------
   class CCurtain1 : public IRfxcomMessage
   {
   public:
      CCurtain1(boost::shared_ptr<yApi::IYPluginApi> api,
                const std::string& command,
                const shared::CDataContainer& deviceDetails);

      CCurtain1(boost::shared_ptr<yApi::IYPluginApi> api,
                unsigned int subType,
                const std::string& name,
                const shared::CDataContainer& manuallyDeviceCreationConfiguration);

      CCurtain1(boost::shared_ptr<yApi::IYPluginApi> api,
                const RBUF& rbuf,
                size_t rbufSize);

      virtual ~CCurtain1();

      // IRfxcomMessage implementation
      boost::shared_ptr<std::queue<shared::communication::CByteBuffer>> encode(boost::shared_ptr<ISequenceNumber> seqNumberProvider) const override;
      void historizeData(boost::shared_ptr<yApi::IYPluginApi> api) const override;
      void filter() const override;
      void declareDevice(boost::shared_ptr<yApi::IYPluginApi> api) const override;
      const std::string& getDeviceName() const override;
      const std::vector<boost::shared_ptr<const yApi::historization::IHistorizable>>& keywords() override;
      // [END] IRfxcomMessage implementation

   protected:
      void buildDeviceName();
      void buildDeviceModel();
      void buildDeviceDetails();

      //--------------------------------------------------------------
      /// \brief	                        Convert Yadoms command to protocol value
      /// \param[in] curtainState         The state from Yadoms
      /// \return                         The value known by the protocol
      /// \throw                          shared::exception::CInvalidParameter if fail to interpret command
      //--------------------------------------------------------------
      static unsigned char toProtocolState(const yApi::historization::CCurtain& curtainState);

   private:
      //--------------------------------------------------------------
      /// \brief	The device sub-type
      //--------------------------------------------------------------
      unsigned char m_subType;

      //--------------------------------------------------------------
      /// \brief	The device house code
      //--------------------------------------------------------------
      unsigned char m_houseCode;

      //--------------------------------------------------------------
      /// \brief	The device unit code
      //--------------------------------------------------------------
      unsigned char m_unitCode;

      //--------------------------------------------------------------
      /// \brief	The device name
      //--------------------------------------------------------------
      std::string m_deviceName;

      //--------------------------------------------------------------
      /// \brief	The device model
      //--------------------------------------------------------------
      std::string m_deviceModel;

      shared::CDataContainer m_deviceDetails;

      //--------------------------------------------------------------
      /// \brief	The keyword associated with state
      //--------------------------------------------------------------
      boost::shared_ptr<yApi::historization::CCurtain> m_state;
   };
} // namespace rfxcomMessages



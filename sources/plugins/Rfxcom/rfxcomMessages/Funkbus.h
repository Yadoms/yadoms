#pragma once
#include "IRfxcomMessage.h"
#include "RFXtrxHelpers.h"
#include <shared/plugin/yPluginApi/IYPluginApi.h>
#include <shared/DataContainer.h>

namespace yApi = shared::plugin::yPluginApi;

namespace rfxcomMessages
{
   //--------------------------------------------------------------
   /// \brief	The Funkbus protocol support
   //--------------------------------------------------------------
   class CFunkbus : public IRfxcomMessage
   {
   public:
      CFunkbus(boost::shared_ptr<yApi::IYPluginApi> api,
               const std::string& command,
               const shared::CDataContainer& deviceDetails);

      CFunkbus(boost::shared_ptr<yApi::IYPluginApi> api,
               unsigned int subType,
               const std::string& name,
               const shared::CDataContainer& manuallyDeviceCreationConfiguration);

      CFunkbus(boost::shared_ptr<yApi::IYPluginApi> api,
               const RBUF& rbuf,
               size_t rbufSize);

      virtual ~CFunkbus();

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

      static unsigned char toProtocolState(const yApi::historization::CSwitch& switchState);
      static bool fromProtocolState(unsigned char protocolState);

   private:
      unsigned char m_subType;
      unsigned char m_groupCode;
      unsigned char m_unitCode;
      unsigned short m_id;

      std::string m_deviceName;
      std::string m_deviceModel;

      shared::CDataContainer m_deviceDetails;

      boost::shared_ptr<yApi::historization::CSwitch> m_state;
      boost::shared_ptr<yApi::historization::CSignalPower> m_signalPower;
      std::vector<boost::shared_ptr<const yApi::historization::IHistorizable>> m_keywords;
   };
} // namespace rfxcomMessages

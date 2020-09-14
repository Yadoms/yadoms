#pragma once

#include "IRfxcomMessage.h"
#include "RFXtrxHelpers.h"
#include <shared/plugin/yPluginApi/IYPluginApi.h>
#include "IFanSubtype.h"

namespace yApi = shared::plugin::yPluginApi;

namespace rfxcomMessages
{
   //--------------------------------------------------------------
   /// \brief	The Fan protocol support
   //--------------------------------------------------------------
   class CFan : public IRfxcomMessage
   {
   public:
      CFan(boost::shared_ptr<yApi::IYPluginApi> api,
           const std::string& keyword,
           const std::string& command,
           const boost::shared_ptr<shared::CDataContainer>& deviceDetails);

      CFan(boost::shared_ptr<yApi::IYPluginApi> api,
           unsigned int subType,
           const std::string& name,
           const boost::shared_ptr<shared::CDataContainer>& manuallyDeviceCreationConfiguration);

      CFan(boost::shared_ptr<yApi::IYPluginApi> api,
           const RBUF& rbuf,
           size_t rbufSize);

      virtual ~CFan();

      // IRfxcomMessage implementation
      boost::shared_ptr<std::queue<shared::communication::CByteBuffer>> encode(boost::shared_ptr<ISequenceNumber> seqNumberProvider) const override;
      void historizeData(boost::shared_ptr<yApi::IYPluginApi> api) const override;
      void filter() const override;
      void declareDevice(boost::shared_ptr<yApi::IYPluginApi> api) const override;
      const std::string& getDeviceName() const override;
      const std::vector<boost::shared_ptr<const yApi::historization::IHistorizable>>& keywords() override;
      // [END] IRfxcomMessage implementation

   protected:
      void createSubType(unsigned char subType);
      void buildDeviceName();
      void buildDeviceDetails();

   private:
      unsigned char m_subType;
      unsigned int m_id;
      std::string m_deviceName;
      boost::shared_ptr<shared::CDataContainer> m_deviceDetails;
      boost::shared_ptr<IFanSubtype> m_subTypeManager;

      //--------------------------------------------------------------
      /// \brief	The keywords list to historize in one step for better performances
      //--------------------------------------------------------------
      std::vector<boost::shared_ptr<const yApi::historization::IHistorizable>> m_keywords;
   };
} // namespace rfxcomMessages



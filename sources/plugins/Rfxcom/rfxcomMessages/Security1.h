#pragma once
#include "IRfxcomMessage.h"
#include "RFXtrxHelpers.h"
#include <shared/plugin/yPluginApi/IYPluginApi.h>
#include "ISecurity1Subtype.h"
#include "../IUnsecuredProtocolFilter.h"

namespace yApi = shared::plugin::yPluginApi;

namespace rfxcomMessages
{
   //--------------------------------------------------------------
   /// \brief	The Security1 protocol support
   //--------------------------------------------------------------
   class CSecurity1 : public IRfxcomMessage
   {
   public:
      CSecurity1(boost::shared_ptr<yApi::IYPluginApi> api,
                 const std::string& keyword,
                 const std::string& command,
                 const shared::CDataContainer& deviceDetails);

      CSecurity1(boost::shared_ptr<yApi::IYPluginApi> api,
                 unsigned int subType,
                 const std::string& name,
                 const shared::CDataContainer& manuallyDeviceCreationConfiguration);

      CSecurity1(boost::shared_ptr<yApi::IYPluginApi> api,
                 const RBUF& rbuf,
                 size_t rbufSize,
                 boost::shared_ptr<IUnsecuredProtocolFilter> messageFilter);

      virtual ~CSecurity1();

      //--------------------------------------------------------------
      /// \brief	Filter
      //--------------------------------------------------------------
      static boost::shared_ptr<IUnsecuredProtocolFilter> createFilter();

      // IRfxcomMessage implementation
      boost::shared_ptr<std::queue<shared::communication::CByteBuffer>> encode(boost::shared_ptr<ISequenceNumber> seqNumberProvider) const override;
      void historizeData(boost::shared_ptr<yApi::IYPluginApi> api) const override;
      void filter() const override;
      void declareDevice(boost::shared_ptr<yApi::IYPluginApi> api) const override;
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
      unsigned long m_id;

      //--------------------------------------------------------------
      /// \brief	The filter for unsecured protocols
      //--------------------------------------------------------------
      boost::shared_ptr<IUnsecuredProtocolFilter> m_messageFilter;

      //--------------------------------------------------------------
      /// \brief	The device name
      //--------------------------------------------------------------
      std::string m_deviceName;

      shared::CDataContainer m_deviceDetails;

      //--------------------------------------------------------------
      /// \brief	The sub-type management
      //--------------------------------------------------------------
      boost::shared_ptr<ISecurity1Subtype> m_subTypeManager;

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
      std::vector<boost::shared_ptr<const yApi::historization::IHistorizable>> m_keywords;
   };
} // namespace rfxcomMessages



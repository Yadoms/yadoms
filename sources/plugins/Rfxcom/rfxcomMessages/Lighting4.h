#pragma once
#include "IRfxcomMessage.h"
#include "RFXtrxHelpers.h"
#include <shared/plugin/yPluginApi/IYPluginApi.h>
#include <shared/DataContainer.h>
#include "../IUnsecuredProtocolFilter.h"

namespace yApi = shared::plugin::yPluginApi;

namespace rfxcomMessages
{
   //--------------------------------------------------------------
   /// \brief	The Lightning4 protocol support
   //--------------------------------------------------------------
   class CLighting4 : public IRfxcomMessage
   {
   public:
      CLighting4(boost::shared_ptr<yApi::IYPluginApi> api,
                 const std::string& command,
                 const boost::shared_ptr<shared::CDataContainer>& deviceDetails);

      CLighting4(boost::shared_ptr<yApi::IYPluginApi> api,
                 unsigned int subType,
                 const std::string& name,
                 const boost::shared_ptr<shared::CDataContainer>& manuallyDeviceCreationConfiguration);

      CLighting4(boost::shared_ptr<yApi::IYPluginApi> api,
                 const RBUF& rbuf,
                 size_t rbufSize,
                 boost::shared_ptr<IUnsecuredProtocolFilter> messageFilter);

      virtual ~CLighting4();

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
      /// \brief	                        Build the device name
      //--------------------------------------------------------------
      void buildDeviceName();

      //--------------------------------------------------------------
      /// \brief	                        Build the sensor model
      //--------------------------------------------------------------
      void buildDeviceModel();

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
      /// \brief	The device name
      //--------------------------------------------------------------
      std::string m_deviceName;

      //--------------------------------------------------------------
      /// \brief	The device model
      //--------------------------------------------------------------
      std::string m_deviceModel;

      boost::shared_ptr<shared::CDataContainer> m_deviceDetails;

      //--------------------------------------------------------------
      /// \brief	The keyword
      //--------------------------------------------------------------
      boost::shared_ptr<yApi::historization::CEvent> m_keyword;

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

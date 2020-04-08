#pragma once

#include "IRfxcomMessage.h"
#include "RFXtrxHelpers.h"
#include <shared/plugin/yPluginApi/IYPluginApi.h>

namespace yApi = shared::plugin::yPluginApi;

namespace rfxcomMessages
{
   //--------------------------------------------------------------
   /// \brief	The FS20 protocol support
   //--------------------------------------------------------------
   class CFS20 : public IRfxcomMessage
   {
   public:
      CFS20(boost::shared_ptr<yApi::IYPluginApi> api,
            const std::string& command,
            const shared::CDataContainerSharedPtr& deviceDetails);

      CFS20(boost::shared_ptr<yApi::IYPluginApi> api,
            unsigned int subType,
            const std::string& name,
            const shared::CDataContainerSharedPtr& manuallyDeviceCreationConfiguration);

      CFS20(boost::shared_ptr<yApi::IYPluginApi> api,
            const RBUF& rbuf,
            size_t rbufSize);

      virtual ~CFS20();

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
      /// \brief	The device house code (8 numbers from 1-4, 11111111 to 44444444)
      //--------------------------------------------------------------
      std::string m_houseCode;

      //--------------------------------------------------------------
      /// \brief	The device group address (2 numbers from 1-4, 11 to 44)
      //--------------------------------------------------------------
      std::string m_groupAddress;

      //--------------------------------------------------------------
      /// \brief	The device sub-address (2 numbers from 1-4, 11 to 44)
      //--------------------------------------------------------------
      std::string m_subAddress;

      //--------------------------------------------------------------
      /// \brief	The device name
      //--------------------------------------------------------------
      std::string m_deviceName;

      //--------------------------------------------------------------
      /// \brief	The device model
      //--------------------------------------------------------------
      std::string m_deviceModel;

      shared::CDataContainerSharedPtr m_deviceDetails;

      //--------------------------------------------------------------
      /// \brief	The keyword associated with state
      //--------------------------------------------------------------
      boost::shared_ptr<yApi::historization::CDimmable> m_state;

      //--------------------------------------------------------------
      /// \brief	The keyword associated with signalPower
      //--------------------------------------------------------------
      boost::shared_ptr<yApi::historization::CSignalPower> m_signalPower;

      //--------------------------------------------------------------
      /// \brief	The keywords list to historize in one step for better performances
      //--------------------------------------------------------------
      std::vector<boost::shared_ptr<const yApi::historization::IHistorizable>> m_keywords;
   };
} // namespace rfxcomMessages

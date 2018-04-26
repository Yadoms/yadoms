#pragma once
#include "IRfxcomMessage.h"
#include "RFXtrxHelpers.h"
#include <shared/plugin/yPluginApi/IYPluginApi.h>
#include <shared/DataContainer.h>
#include "IThermostat3Subtype.h"

namespace yApi = shared::plugin::yPluginApi;

namespace rfxcomMessages
{
   //--------------------------------------------------------------
   /// \brief	The Thermostat3 protocol support
   //--------------------------------------------------------------
   class CThermostat3 : public IRfxcomMessage
   {
   public:
      CThermostat3(boost::shared_ptr<yApi::IYPluginApi> api,
                   const std::string& keyword,
                   const std::string& command,
                   const shared::CDataContainer& deviceDetails);

      CThermostat3(boost::shared_ptr<yApi::IYPluginApi> api,
                   unsigned int subType,
                   const std::string& name,
                   const shared::CDataContainer& manuallyDeviceCreationConfiguration);

      CThermostat3(boost::shared_ptr<yApi::IYPluginApi> api,
                   const RBUF& rbuf,
                   size_t rbufSize);

      virtual ~CThermostat3();

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
      /// \brief	Global initialization method
      /// \param[in] api                  Yadoms APi context
      //--------------------------------------------------------------
      void Init(boost::shared_ptr<yApi::IYPluginApi> api);

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
      /// \brief	The device unit code
      //--------------------------------------------------------------
      unsigned int m_unitCode;

      //--------------------------------------------------------------
      /// \brief	The device name
      //--------------------------------------------------------------
      std::string m_deviceName;

      shared::CDataContainer m_deviceDetails;

      //--------------------------------------------------------------
      /// \brief	The sub-type management
      //--------------------------------------------------------------
      boost::shared_ptr<IThermostat3Subtype> m_subTypeManager;

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



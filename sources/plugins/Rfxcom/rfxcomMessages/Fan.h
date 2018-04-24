#pragma once

#include "IRfxcomMessage.h"
#include "RFXtrxHelpers.h"
#include <shared/plugin/yPluginApi/IYPluginApi.h>

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
           const shared::CDataContainer& deviceDetails);

      CFan(boost::shared_ptr<yApi::IYPluginApi> api,
           unsigned int subType,
           const std::string& name,
           const shared::CDataContainer& manuallyDeviceCreationConfiguration);

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
      //--------------------------------------------------------------
      /// \brief	                        Build the device name
      //--------------------------------------------------------------
      void buildDeviceName();

      //--------------------------------------------------------------
      /// \brief	                        Build the sensor model
      //--------------------------------------------------------------
      void buildDeviceModel();

      void buildDeviceDetails();

      //--------------------------------------------------------------
      /// \brief	                        Convert Yadoms command to protocol value
      /// \return                         The value known by the protocol
      /// \throw                          shared::exception::CInvalidParameter if fail to interpret command
      //--------------------------------------------------------------
      unsigned char toProtocolState() const;

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
      /// \brief	Flag for command
      //--------------------------------------------------------------
      std::string m_cmd;

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
      /// \brief	The keyword associated with light
      //--------------------------------------------------------------
      boost::shared_ptr<yApi::historization::CSwitch> m_light;

      //--------------------------------------------------------------
      /// \brief	The keyword associated with fan (off = speed down, on = speed up)
      //--------------------------------------------------------------
      boost::shared_ptr<yApi::historization::CSwitch> m_fan;

      //--------------------------------------------------------------
      /// \brief	The keywords associated with SEAV TXS4 remote
      //--------------------------------------------------------------
      boost::shared_ptr<yApi::historization::CEvent> m_t1;
      boost::shared_ptr<yApi::historization::CEvent> m_t2;
      boost::shared_ptr<yApi::historization::CEvent> m_t3;
      boost::shared_ptr<yApi::historization::CEvent> m_t4;

      //--------------------------------------------------------------
      /// \brief	The keywords list to historize in one step for better performances
      //--------------------------------------------------------------
      std::vector<boost::shared_ptr<const yApi::historization::IHistorizable>> m_keywords;
   };
} // namespace rfxcomMessages



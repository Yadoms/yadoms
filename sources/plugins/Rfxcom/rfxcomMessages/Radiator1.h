#pragma once
#include "IRfxcomMessage.h"
#include "RFXtrxHelpers.h"
#include <shared/plugin/yPluginApi/IYPluginApi.h>

namespace yApi = shared::plugin::yPluginApi;

namespace rfxcomMessages
{
   //--------------------------------------------------------------
   /// \brief	The Radiator1 protocol support
   //--------------------------------------------------------------
   class CRadiator1 : public IRfxcomMessage
   {
   public:
      CRadiator1(boost::shared_ptr<yApi::IYPluginApi> api,
                 const std::string& keyword,
                 const std::string& command, const boost::shared_ptr<shared::CDataContainer>& deviceDetails);

      CRadiator1(boost::shared_ptr<yApi::IYPluginApi> api,
                 unsigned int subType,
                 const std::string& name,
                 const boost::shared_ptr<shared::CDataContainer>& manuallyDeviceCreationConfiguration);

      CRadiator1(boost::shared_ptr<yApi::IYPluginApi> api,
                 const RBUF& rbuf,
                 size_t rbufSize);

      virtual ~CRadiator1();

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
      /// \param[in] radiator1            The radiator1 RBUF to fill
      /// \throw                          shared::exception::CInvalidParameter if fail to interpret command
      //--------------------------------------------------------------
      void toProtocolState(RBUF& radiator1) const;

   private:
      //--------------------------------------------------------------
      /// \brief	The device sub-type
      //--------------------------------------------------------------
      unsigned char m_subType;

      //--------------------------------------------------------------
      /// \brief	The device unit code
      //--------------------------------------------------------------
      unsigned char m_unitCode;

      //--------------------------------------------------------------
      /// \brief	The device id
      //--------------------------------------------------------------
      unsigned int m_id;

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
      /// \brief	The command type
      //--------------------------------------------------------------
      bool m_dayNightCmd;

      //--------------------------------------------------------------
      /// \brief	The keyword associated with day/night state
      //--------------------------------------------------------------
      boost::shared_ptr<yApi::historization::CSwitch> m_day;

      //--------------------------------------------------------------
      /// \brief	The keyword associated with temperature set point
      //--------------------------------------------------------------
      boost::shared_ptr<yApi::historization::CTemperature> m_setPoint;

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



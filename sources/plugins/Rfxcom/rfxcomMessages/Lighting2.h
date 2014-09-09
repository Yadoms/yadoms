#pragma once

#include "IRfxcomMessage.h"
#include "RFXtrxHelpers.h"
#include <shared/plugin/yadomsApi/IYadomsApi.h>
#include <shared/DataContainer.h>

namespace yApi = shared::plugin::yadomsApi;

namespace rfxcomMessages
{
   //--------------------------------------------------------------
   /// \brief	The Lightning1 protocol support
   //--------------------------------------------------------------
   class CLighting2 : public IRfxcomMessage
   {
   public:
      //--------------------------------------------------------------
      /// \brief	                        Constructor
      /// \param[in] command              The command
      /// \param[in] deviceParameters     The device parameters
      /// \throw                          shared::exception::CInvalidParameter if fail to interpret command
      /// \note                           Use this constructor for command (to build RFXCom message)
      //--------------------------------------------------------------
      CLighting2(const shared::CDataContainer& command, const shared::CDataContainer& deviceParameters);

      //--------------------------------------------------------------
      /// \brief	                        Constructor
      /// \param[in] rbuf                 The received buffer
      /// \param[in] seqNumberProvider    The sequence number provider
      /// \note                           Use this constructor for received messages (to historize received data to Yadoms)
      /// \throw                          shared::exception::CInvalidParameter
      //--------------------------------------------------------------
      CLighting2(const RBUF& rbuf, boost::shared_ptr<const ISequenceNumberProvider> seqNumberProvider);

      //--------------------------------------------------------------
      /// \brief	Destructor
      //--------------------------------------------------------------
      virtual ~CLighting2();

      // IRfxcomMessage implementation
      virtual const CByteBuffer encode(boost::shared_ptr<ISequenceNumberProvider> seqNumberProvider) const;
      virtual void historizeData(boost::shared_ptr<yApi::IYadomsApi> context) const;
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

      //--------------------------------------------------------------
      /// \brief	                        Convert Yadoms command to protocol value
      /// \param[in] yadomsCommand        The command from Yadoms
      /// \param[out] state               The state known by the protocol
      /// \param[out] level               The level known by the protocol (if state is light2_sSetLevel)
      /// \throw                          shared::exception::CInvalidParameter if fail to interpret command
      //--------------------------------------------------------------
      static void toProtocolState(const shared::CDataContainer& yadomsCommand, unsigned char& state, unsigned char& level);
      
      //--------------------------------------------------------------
      /// \brief	                        Convert protocol value to Yadoms state
      /// \param[in] protocolState        The state known by the protocol
      /// \param[in] protocolLevel        The level known by the protocol
      /// \return                         The command for Yadoms
      /// \throw                          shared::exception::CInvalidParameter if fail to interpret command
      //--------------------------------------------------------------
      static std::string toYadomsState(unsigned char protocolState, unsigned char protocolLevel);

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
      /// \brief	The device id
      //--------------------------------------------------------------
      unsigned int m_id;

      //--------------------------------------------------------------
      /// \brief	The device unit code
      //--------------------------------------------------------------
      unsigned char m_unitCode;

      //--------------------------------------------------------------
      /// \brief	The state
      //--------------------------------------------------------------
      unsigned char m_state;

      //--------------------------------------------------------------
      /// \brief	The dim level
      //--------------------------------------------------------------
      unsigned char m_level;

      //--------------------------------------------------------------
      /// \brief	The RSSI (received messages only)
      //--------------------------------------------------------------
      unsigned char m_rssi;

      //--------------------------------------------------------------
      /// \brief	The device name
      //--------------------------------------------------------------
      std::string m_deviceName;

      //--------------------------------------------------------------
      /// \brief	The device model
      //--------------------------------------------------------------
      std::string m_deviceModel;
   };
} // namespace rfxcomMessages

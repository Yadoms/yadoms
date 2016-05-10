#pragma once

#include "IRfxcomMessage.h"
#include "RFXtrxHelpers.h"
#include <shared/plugin/yPluginApi/IYPluginApi.h>
#include <shared/DataContainer.h>
#include "ICartelectronicSubtype.h"

namespace yApi = shared::plugin::yPluginApi;

namespace rfxcomMessages
{
   //--------------------------------------------------------------
   /// \brief	The Chime protocol support
   //--------------------------------------------------------------
   class CCartelectronic : public IRfxcomMessage
   {
   public:

      //--------------------------------------------------------------
      /// \brief	                        Constructor
      /// \param[in] context              Yadoms APi context
      /// \param[in] rbuf                 The received buffer
      /// \param[in] rbufSize             Message size, received from Rfxcom
      /// \param[in] seqNumberProvider    The sequence number provider
      /// \note                           Use this constructor for received messages (to historize received data to Yadoms)
      /// \throw                          shared::exception::CInvalidParameter
      //--------------------------------------------------------------
      CCartelectronic(boost::shared_ptr<yApi::IYPluginApi> context, const RBUF& rbuf, size_t rbufSize, boost::shared_ptr<const ISequenceNumberProvider> seqNumberProvider);

      //--------------------------------------------------------------
      /// \brief	Destructor
      //--------------------------------------------------------------
      virtual ~CCartelectronic();

      // IRfxcomMessage implementation
      virtual boost::shared_ptr<std::queue<shared::communication::CByteBuffer> > encode(boost::shared_ptr<ISequenceNumberProvider> seqNumberProvider) const;
      virtual void historizeData(boost::shared_ptr<yApi::IYPluginApi> context) const;
      virtual const std::string& getDeviceName() const;
      // [END] IRfxcomMessage implementation

   protected:
      //--------------------------------------------------------------
      /// \brief	Set and create the subtype
      /// \param[in] subType              Device subType
      //--------------------------------------------------------------
      void createSubType( unsigned char subType, const RBUF& rbuf, size_t rbufSize );

      //--------------------------------------------------------------
      /// \brief	Declare the device
      /// \param[in] context              Yadoms APi context
      //--------------------------------------------------------------
      void declare(boost::shared_ptr<yApi::IYPluginApi> context);

      //--------------------------------------------------------------
      /// \brief	                        Get device model
      /// \return                         The device model
      //--------------------------------------------------------------
	  std::string getModel() const;

   private:
      //--------------------------------------------------------------
      /// \brief	The device sub-type
      //--------------------------------------------------------------
      unsigned char m_subType;

      //--------------------------------------------------------------
      /// \brief	The device id
      //--------------------------------------------------------------
      std::string m_id;

      //--------------------------------------------------------------
      /// \brief	The sub-type management
      //--------------------------------------------------------------
      boost::shared_ptr<ICartelectronicSubtype> m_subTypeManager;

      //--------------------------------------------------------------
      /// \brief	The keyword associated with rssi
      //--------------------------------------------------------------
      boost::shared_ptr<yApi::historization::CRssi> m_rssi;

      //--------------------------------------------------------------
      /// \brief	The battery level (percent)
      //--------------------------------------------------------------
      boost::shared_ptr<yApi::historization::CBatteryLevel> m_batteryLevel;
   };
} // namespace rfxcomMessages

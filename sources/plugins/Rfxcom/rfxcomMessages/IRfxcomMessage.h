#pragma once

#include <shared/plugin/yPluginApi/IYPluginApi.h>
#include <shared/communication/Buffer.hpp>
#include "../ISequenceNumber.h"

namespace yApi = shared::plugin::yPluginApi;

namespace rfxcomMessages
{
   //--------------------------------------------------------------
   /// \brief	The RFXCom message converter interface
   //--------------------------------------------------------------
   class IRfxcomMessage
   {
   public:
      //--------------------------------------------------------------
      /// \brief	Destructor
      //--------------------------------------------------------------
      virtual ~IRfxcomMessage()
      {
      }

      //--------------------------------------------------------------
      /// \brief	                        Encode messages for serial link
      /// \param[in] seqNumberProvider    The sequence number provider
      /// \return                         Buffers (a command can be parted in several messages)
      //--------------------------------------------------------------
      virtual boost::shared_ptr<std::queue<shared::communication::CByteBuffer>> encode(boost::shared_ptr<ISequenceNumber> seqNumberProvider) const = 0;

      //--------------------------------------------------------------
      /// \brief	                        Historize message data to Yadoms
      /// \param[in] api                  Yadoms APi context
      //--------------------------------------------------------------
      virtual void historizeData(boost::shared_ptr<yApi::IYPluginApi> api) const = 0;

      //--------------------------------------------------------------
      /// \brief	                        Filter new device if necessary
      /// \throw                          CMessageFilteredException if message need to be filtered
      //--------------------------------------------------------------
      virtual void filter() const = 0;

      //--------------------------------------------------------------
      /// \brief	                        Declare the device
      /// \param[in] api                  Yadoms APi context
      //--------------------------------------------------------------
      virtual void declareDevice(boost::shared_ptr<yApi::IYPluginApi> api) const = 0;

      //--------------------------------------------------------------
      /// \brief	                        Returns the device name
      /// \return                         Device name
      //--------------------------------------------------------------
      virtual const std::string& getDeviceName() const = 0;

      //--------------------------------------------------------------
      /// \brief	                        Get keywords
      /// \return                         Read-only keyword list, or NoKeywords if empty
      //--------------------------------------------------------------
      static const std::vector<boost::shared_ptr<const yApi::historization::IHistorizable>> NoKeywords;
      virtual const std::vector<boost::shared_ptr<const yApi::historization::IHistorizable>>& keywords() = 0;
   };
}

#pragma once

#include <shared/plugin/yPluginApi/IYPluginApi.h>
#include <shared/communication/Buffer.hpp>
#include "ISequenceNumberProvider.h"

namespace yApi = shared::plugin::yPluginApi;

namespace LinkyMessages
{
   //--------------------------------------------------------------
   /// \brief	The Linky message converter interface
   //--------------------------------------------------------------
   class ILinkyMessage
   {
   public:
      //--------------------------------------------------------------
      /// \brief	Destructor
      //--------------------------------------------------------------
      virtual ~ILinkyMessage()
      {
      }

      //--------------------------------------------------------------
      /// \brief	                        Encode messages for serial link
      /// \param[in] seqNumberProvider    The sequence number provider
      /// \return                         Buffers (a command can be parted in several messages)
      //--------------------------------------------------------------
      virtual boost::shared_ptr<std::queue<const shared::communication::CByteBuffer> > encode(boost::shared_ptr<ISequenceNumberProvider> seqNumberProvider) const = 0;

      //--------------------------------------------------------------
      /// \brief	                        Historize message data to Yadoms
      /// \param[in] api                  Yadoms APi context
      //--------------------------------------------------------------
      virtual void historizeData(boost::shared_ptr<yApi::IYPluginApi> api) const = 0;

      //--------------------------------------------------------------
      /// \brief	                        Returns the device name
      /// \return                         Device name
      //--------------------------------------------------------------
      virtual const std::string& getDeviceName() const = 0;

      //--------------------------------------------------------------
      /// \brief	                        Set keyword state from default value
      //--------------------------------------------------------------
      virtual void reset() = 0;
   };
}
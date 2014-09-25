#pragma once

#include <shared/plugin/yadomsApi/IYadomsApi.h>
#include <shared/communication/Buffer.hpp>
#include "../ISequenceNumberProvider.h"

namespace yApi = shared::plugin::yadomsApi;

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
      virtual ~IRfxcomMessage() {}

      //--------------------------------------------------------------
      /// \brief	                        Encode message for serial link
      /// \param[in] seqNumberProvider    The sequence number provider
      /// \return                         Buffer
      //--------------------------------------------------------------
      virtual const shared::communication::CByteBuffer encode(boost::shared_ptr<ISequenceNumberProvider> seqNumberProvider) const = 0;

      //--------------------------------------------------------------
      /// \brief	                        Historize message data to Yadoms
      /// \param[in] context              Yadoms APi context
      //--------------------------------------------------------------
      virtual void historizeData(boost::shared_ptr<yApi::IYadomsApi> context) const = 0;
   };
}

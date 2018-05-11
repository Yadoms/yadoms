#pragma once

#include <shared/plugin/yPluginApi/IYPluginApi.h>
#include <shared/communication/Buffer.hpp>

namespace yApi = shared::plugin::yPluginApi;

namespace nitramMessages
{
   //--------------------------------------------------------------
   /// \brief	The Nitram message converter interface
   //--------------------------------------------------------------
   class INitramMessage
   {
   public:
      //--------------------------------------------------------------
      /// \brief	Destructor
      //--------------------------------------------------------------
      virtual ~INitramMessage()
      {}

      virtual void decode(boost::shared_ptr<yApi::IYPluginApi> api,
                          const shared::communication::CByteBuffer& buffer) = 0;

      //--------------------------------------------------------------
      /// \brief	                        Encode messages for serial link
      /// \param[in] seqNumberProvider    The sequence number provider
      /// \return                         Buffers (a command can be parted in several messages)
      //--------------------------------------------------------------
      virtual boost::shared_ptr<shared::communication::CByteBuffer> encode() const = 0;

      virtual void historizeData(boost::shared_ptr<yApi::IYPluginApi> api) = 0;

      virtual bool onlyAck() const = 0;

      //--------------------------------------------------------------
      /// \brief	                        Get keywords
      /// \return                         Read-only keyword list, or NoKeywords if empty
      //--------------------------------------------------------------
      static const std::vector<boost::shared_ptr<const yApi::historization::IHistorizable>> NoKeywords;
      virtual const std::vector<boost::shared_ptr<const yApi::historization::IHistorizable>>& keywords() const = 0;
   };
}
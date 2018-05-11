#pragma once
#include "INitramMessage.h"

namespace nitramMessages
{
   //--------------------------------------------------------------
   /// \brief	This class manage an acknoledge from Nitram UPS
   //--------------------------------------------------------------
   class CAck : public INitramMessage
   {
   public:
      //--------------------------------------------------------------
      /// \brief	                           Constructor
      //--------------------------------------------------------------
      CAck();

      //--------------------------------------------------------------
      /// \brief	                           Destructor
      //--------------------------------------------------------------
      virtual ~CAck();

      // INitramMessage implementation
      void decode(boost::shared_ptr<yApi::IYPluginApi> api,
                  const shared::communication::CByteBuffer& buffer) override;
      boost::shared_ptr<shared::communication::CByteBuffer> encode() const override;
      const std::vector<boost::shared_ptr<const yApi::historization::IHistorizable>>& keywords() const override;
      void historizeData(boost::shared_ptr<yApi::IYPluginApi> api) override;
      bool onlyAck() const override;
      // [END] INitramMessage implementation

   private:
   };
} // namespace rfxcomMessages



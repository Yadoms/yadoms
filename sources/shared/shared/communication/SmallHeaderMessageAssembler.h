#pragma once
#include "IMessageAssembler.h"

namespace shared
{
   namespace communication
   {
      //--------------------------------------------------------------
      /// \brief	Implementation of a message cutter with a small header
      ///  (2 bytes). Number of parts is limited to 255
      //--------------------------------------------------------------
      class SmallHeaderMessageAssembler : public IMessageAssembler
      {
      public:
         explicit SmallHeaderMessageAssembler(size_t maxPartSize);

         virtual ~SmallHeaderMessageAssembler() = default;

         // IMessageAssembler Implementation
         void appendPart(boost::shared_ptr<const unsigned char[]> message,
                         size_t messageSize) override;
         bool isCompleted() const override;
         boost::shared_ptr<const unsigned char[]> message() const override;
         size_t messageSize() const override;
         // [END] IMessageAssembler Implementation

      private:
         void allocFullMessage(size_t partCount);

         const size_t m_maxPartSize;
         boost::shared_ptr<unsigned char[]> m_fullMessage;
         size_t m_fullMessageSize;
         size_t m_fullMessageIndex;
         bool m_completed;
      };
   }
} // namespace shared::communication

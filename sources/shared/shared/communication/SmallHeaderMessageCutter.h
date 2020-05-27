#pragma once
#include "IMessageCutter.h"

namespace shared
{
   namespace communication
   {
      //--------------------------------------------------------------
      /// \brief	Implementation of a message cutter with a small header
      ///  (2 bytes). Number of parts is limited to 255
      //--------------------------------------------------------------
      class SmallHeaderMessageCutter : public IMessageCutter
      {
      public:
         SmallHeaderMessageCutter(size_t maxPartSize,
                                  size_t maxMessageCount);

         virtual ~SmallHeaderMessageCutter();

         // IMessageCutter Implementation
         boost::shared_ptr<std::vector<boost::shared_ptr<IMessageCutterPart>>> cut(boost::shared_ptr<unsigned char[]> message,
                                                                                   size_t messageSize) const override;
         // [END] IMessageCutter Implementation

      private:
         size_t m_maxUsefulPartSize;
         size_t m_maxUsefulSize;
      };
   }
} // namespace shared::communication



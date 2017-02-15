#include "stdafx.h"
#include "SmallHeaderMessageCutter.h"
#include "SmallHeaderMessageCutterPart.h"
#include <shared/Log.h>

namespace shared
{
   namespace communication
   {
      SmallHeaderMessageCutter::SmallHeaderMessageCutter(size_t maxPartSize,
                                                         size_t maxMessageCount)
         : m_maxUsefulPartSize(maxPartSize - SmallHeaderMessageCutterPart::headerSize()),
           m_maxUsefulSize(m_maxUsefulPartSize * maxMessageCount)
      {
      }

      SmallHeaderMessageCutter::~SmallHeaderMessageCutter()
      {
      }

      boost::shared_ptr<std::vector<boost::shared_ptr<IMessageCutterPart>>> SmallHeaderMessageCutter::cut(boost::shared_ptr<unsigned char[]> message,
                                                                                                          size_t messageSize) const
      {
         if (messageSize > m_maxUsefulSize)
         {
            YADOMS_LOG(error) << "CIpcAdapter::send : message is too big (" << messageSize << " bytes) ==> ignored";
            return boost::shared_ptr<std::vector<boost::shared_ptr<IMessageCutterPart>>>();
         }

         const auto nbParts = static_cast<unsigned char>(messageSize / m_maxUsefulPartSize + (messageSize % m_maxUsefulPartSize ? 1 : 0));
         auto parts = boost::make_shared<std::vector<boost::shared_ptr<IMessageCutterPart>>>();
         for (unsigned char idxPart = 0; idxPart < nbParts; ++idxPart)
         {
            const auto serializedPartOffset = static_cast<size_t>(idxPart * m_maxUsefulPartSize);

            parts->push_back(boost::make_shared<SmallHeaderMessageCutterPart>(idxPart,
                                                                              nbParts,
                                                                              &message.get()[serializedPartOffset],
                                                                              messageSize - serializedPartOffset > m_maxUsefulPartSize ? m_maxUsefulPartSize : messageSize - serializedPartOffset));
         }

         return parts;
      }
   }
} // namespace shared::communication



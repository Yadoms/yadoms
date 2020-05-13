#include "stdafx.h"
#include "Message.h"
#include <shared/DataContainer.h>
#include "MessageFormatter.h"


namespace shared
{
   namespace plugin
   {
      namespace yPluginApi
      {
         namespace historization
         {
            CMessage::CMessage(const std::string& keywordName,
                               const std::string& associatedRecipientField,
                               const EKeywordAccessMode& accessMode,
                               const EHistoryDepth& historyDepth)
               : m_keywordName(keywordName),
                 m_associatedRecipientField(associatedRecipientField),
                 m_accessMode(accessMode),
                 m_historyDepth(historyDepth)
            {
            }

            const std::string& CMessage::getKeyword() const
            {
               return m_keywordName;
            }

            const CStandardCapacity& CMessage::getCapacity() const
            {
               return CStandardCapacities::Message();
            }

            const EKeywordAccessMode& CMessage::getAccessMode() const
            {
               return m_accessMode;
            }

            void CMessage::setCommand(const std::string& yadomsCommand)
            {
               m_content = boost::make_shared<CMessageFormatter>(yadomsCommand);
            }

            void CMessage::set(int from, int to, const std::string& body)
            {
               m_content = boost::make_shared<CMessageFormatter>(from, to, body);
            }

            std::string CMessage::formatValue() const
            {
               return !m_content ? std::string() : m_content->formatValue();
            }

            int CMessage::from() const
            {
               return !m_content ? 0 : m_content->from();
            }

            int CMessage::to() const
            {
               return !m_content ? 0 : m_content->to();
            }

            const std::string& CMessage::body() const
            {
               static const std::string EmptyString;
               return !m_content ? EmptyString : m_content->body();
            }

            const EMeasureType& CMessage::getMeasureType() const
            {
               static const auto MeasureType(EMeasureType::kAbsolute);
               return MeasureType;
            }

            CDataContainerSharedPtr CMessage::getTypeInfo() const
            {
               CDataContainerSharedPtr info = new_CDataContainerSharedPtr();
               info->set("associatedRecipientField", m_associatedRecipientField);
               return info;
            }

            const EHistoryDepth& CMessage::getHistoryDepth() const
            {
               return m_historyDepth;
            }
         }
      }
   }
} // namespace shared::plugin::yPluginApi::historization

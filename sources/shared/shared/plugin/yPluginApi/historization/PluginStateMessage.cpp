#include "stdafx.h"
#include "PluginStateMessage.h"
#include "../StandardCapacity.h"
#include "../StandardUnits.h"


namespace shared
{
   namespace plugin
   {
      namespace yPluginApi
      {
         namespace historization
         {
            DECLARE_CAPACITY(PluginStateMessageCapacity, "pluginStateMessage", CStandardUnits::NoUnit(), EKeywordDataType::kJson);

            CPluginStateMessage::CPluginStateMessage(const std::string& keywordName,
                                                     const EKeywordAccessMode& accessMode)
               : CSingleHistorizableData<boost::shared_ptr<CDataContainer>>(keywordName,
                                                         PluginStateMessageCapacity(),
                                                         accessMode)
            {
            }

            void CPluginStateMessage::setMessage(const std::string& messageId)
            {
               setMessage(messageId,
                          std::string());
            }

            void CPluginStateMessage::setMessage(const std::string& messageId,
                                                 const std::string& messageData)
            {
               auto dc = CDataContainer::make();
               dc->set("messageId", messageId);
               dc->set("messageData", messageData);
               set(dc);
            }

            void CPluginStateMessage::setMessage(const std::string& messageId,
                                                 const std::string& messageDataParam,
                                                 const std::string& messageDataValue)
            {
               std::map<std::string, std::string> mp;
               mp[messageDataParam] = messageDataValue;
               setMessage(messageId, mp);
            }

            void CPluginStateMessage::setMessage(const std::string& messageId,
                                                 const std::map<std::string,
                                                                std::string>& messageDataParameters)
            {
               //convert map to dataContainer
               CDataContainer dc;
               for (const auto& messageDataParameter : messageDataParameters)
                  dc.set(messageDataParameter.first, messageDataParameter.second);

               const auto dcSerialized = dc.serialize(); //use variable to allow use of reference parameter
               setMessage(messageId, dcSerialized);
            }


            std::string CPluginStateMessage::getMessageId() const
            {
               return get()->getWithDefault("messageId",
                                           std::string());
            }

            std::string CPluginStateMessage::getMessageData() const
            {
               return get()->getWithDefault("messageData",
                                           std::string());
            }
         }
      }
   }
} // namespace shared::plugin::yPluginApi::historization

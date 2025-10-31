#include "stdafx.h"
#include "DeviceStateMessage.h"
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
            DECLARE_CAPACITY(DeviceStateMessageCapacity, "deviceStateMessage", CStandardUnits::NoUnit(), EKeywordDataType::kJson);

            CDeviceStateMessage::CDeviceStateMessage(const std::string& keywordName,
                                                     const EKeywordAccessMode& accessMode,
                                                     const EHistoryDepth& historyDepth)
               : CSingleHistorizableData<boost::shared_ptr<CDataContainer>>(keywordName,
                                                         DeviceStateMessageCapacity(),
                                                         accessMode,
                                                         shared::CDataContainer::make(),
                                                         EMeasureType::kAbsolute,
                                                         typeInfo::CEmptyTypeInfo::Empty,
                                                         historyDepth)
            {
            }

            void CDeviceStateMessage::setMessage(const std::string& messageId)
            {
               setMessage(messageId, std::string());
            }

            void CDeviceStateMessage::setMessage(const std::string& messageId,
                                                 const std::string& messageData)
            {
               auto dc = shared::CDataContainer::make();
               dc->set("messageId", messageId);
               dc->set("messageData", messageData);
               set(dc);
            }

            void CDeviceStateMessage::setMessage(const std::string& messageId,
                                                 const std::string& messageDataParam,
                                                 const std::string& messageDataValue)
            {
               std::map<std::string, std::string> mp;
               mp[messageDataParam] = messageDataValue;
               setMessage(messageId, mp);
            }

            void CDeviceStateMessage::setMessage(const std::string& messageId,
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


            std::string CDeviceStateMessage::getMessageId() const
            {
               return get()->getWithDefault("messageId",
                                           std::string());
            }

            std::string CDeviceStateMessage::getMessageData() const
            {
               return get()->getWithDefault("messageData",
                                           std::string());
            }
         }
      }
   }
} // namespace shared::plugin::yPluginApi::historization

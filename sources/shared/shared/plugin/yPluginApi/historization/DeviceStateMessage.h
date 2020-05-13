#pragma once
#include "SingleHistorizableData.hpp"
#include <shared/DataContainer.h>

namespace shared
{
   namespace plugin
   {
      namespace yPluginApi
      {
         namespace historization
         {
            //-----------------------------------------------------
            ///\brief The plugin state historizable object
            //-----------------------------------------------------
            class CDeviceStateMessage : public CSingleHistorizableData<CDataContainerSharedPtr>
            {
            public:
               //-----------------------------------------------------
               ///\brief                     Constructor
               ///\param[in] keywordName     Yadoms keyword name
               ///\param[in] accessMode      The access mode
               ///\param[in] historyDepth    The history depth policy
               //-----------------------------------------------------
               explicit CDeviceStateMessage(const std::string& keywordName,
                                            const EKeywordAccessMode& accessMode = EKeywordAccessMode::kGet,
                                            const EHistoryDepth& historyDepth = EHistoryDepth::kDefault);

               virtual ~CDeviceStateMessage() = default;

               //-----------------------------------------------------
               ///\brief                     Set message
               ///\param[in] messageId       The message (can be i18n code)
               //-----------------------------------------------------
               void setMessage(const std::string& messageId);

               //-----------------------------------------------------
               ///\brief                     Set message
               ///\param[in] messageId       The message (can be i18n code)
               ///\param[in] accessMode      The message data (can be used to customize i18n code)
               //-----------------------------------------------------
               void setMessage(const std::string& messageId,
                               const std::string& messageData);

               //-----------------------------------------------------
               ///\brief                        Set message
               ///\param[in] messageId          The message (can be i18n code)
               ///\param[in] messageDataParam   The message data parameter
               ///\param[in] messageDataValue   The message data value 
               //-----------------------------------------------------
               void setMessage(const std::string& messageId,
                               const std::string& messageDataParam,
                               const std::string& messageDataValue);

               //-----------------------------------------------------
               ///\brief                              Set message
               ///\param[in] messageId                The message (can be i18n code)
               ///\param[in] messageDataParameters    The message data parameters (map of <param, value>)
               //-----------------------------------------------------
               void setMessage(const std::string& messageId,
                               const std::map<std::string, std::string>& messageDataParameters);

               //-----------------------------------------------------
               ///\brief                     Get the messageId
               ///\return                    The messageId
               //-----------------------------------------------------
               std::string getMessageId() const;

               //-----------------------------------------------------
               ///\brief                     Get the messageData
               ///\return                    The messageData
               //-----------------------------------------------------
               std::string getMessageData() const;
            };
         }
      }
   }
} // namespace shared::plugin::yPluginApi::historization

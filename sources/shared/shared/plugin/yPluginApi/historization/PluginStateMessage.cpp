#include "stdafx.h"
#include "PluginStateMessage.h"
#include "../StandardCapacity.h"
#include "../StandardUnits.h"


namespace shared { namespace plugin { namespace yPluginApi { namespace historization
{
   const shared::plugin::yPluginApi::CStandardCapacity& PluginStateMessageCapacity = shared::plugin::yPluginApi::CStandardCapacity("pluginStateMessage_capacity", shared::plugin::yPluginApi::CStandardUnits::NoUnits, shared::plugin::yPluginApi::EKeywordDataType::kJson);

   CPluginStateMessage::CPluginStateMessage(const std::string& keywordName, const EKeywordAccessMode& accessMode)
      :CSingleHistorizableData<shared::CDataContainer>(keywordName, PluginStateMessageCapacity, accessMode)
   {
   }

   CPluginStateMessage::~CPluginStateMessage()
   {
   }

   void CPluginStateMessage::setMessage(const std::string & messageId)
   {
      setMessage(messageId, shared::CStringExtension::EmptyString);
   }
   
   void CPluginStateMessage::setMessage(const std::string & messageId, const std::string & messageData)
   {
      shared::CDataContainer dc;
      dc.set("messageId", messageId);
      dc.set("messageData", messageData);
      set(dc);
   }

   void CPluginStateMessage::setMessage(const std::string & messageId, const std::string & messageDataParam, const std::string & messageDataValue)
   {
      std::map<std::string, std::string> mp;
      mp[messageDataParam] = messageDataValue;
      setMessage(messageId, mp);
   }

   void CPluginStateMessage::setMessage(const std::string & messageId, const std::map<std::string, std::string> & messageDataParameters)
   {
      //convert map to dataContainer
      shared::CDataContainer dc;
      std::map<std::string, std::string>::const_iterator i;
      for(i= messageDataParameters.begin(); i!= messageDataParameters.end(); ++i)
         dc.set(i->first, i->second);

      std::string dcSerialized = dc.serialize(); //use variable to allow use of reference parameter
      setMessage(messageId, dcSerialized);
   }


   const std::string CPluginStateMessage::getMessageId() const
   {
      return get().getWithDefault("messageId", shared::CStringExtension::EmptyString);
   }

   const std::string CPluginStateMessage::getMessageData() const
   {
      return get().getWithDefault("messageData", shared::CStringExtension::EmptyString);
   }

} } } } // namespace shared::plugin::yPluginApi::historization

